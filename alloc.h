#ifndef MYTINYSTL_ALLOC_H_
#define MYTINYSTL_ALLOC_H_

#include <cstddef>
#include <cstdlib>
#include <new>
#include <mutex>
#include <memory>
#include "exceptdef.h"

namespace mystl {

// ============================================================================
// 内存池配置
// ============================================================================

// 内存池配置常量
static const size_t ALIGN = 8;                    // 对齐字节数
static const size_t MAX_BYTES = 128;              // 最大字节数
static const size_t NFREELISTS = MAX_BYTES / ALIGN; // 自由链表数量

// ============================================================================
// 内存池类
// ============================================================================

/**
 * @brief 内存池分配器
 */
class alloc {
private:
    // 自由链表节点
    union obj {
        union obj* next;  // 指向下一个节点
        char data[1];     // 数据区域
    };

    // 自由链表数组
    static obj* free_list[NFREELISTS];

    // 内存池状态
    static char* start_free;  // 内存池起始位置
    static char* end_free;    // 内存池结束位置
    static size_t heap_size;  // 堆大小

    // 线程安全
    static std::mutex mutex_;

private:
    // 将字节数向上舍入到8的倍数
    static size_t round_up(size_t bytes) {
        return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
    }

    // 根据字节数获取自由链表索引
    static size_t free_list_index(size_t bytes) {
        return ((bytes + ALIGN - 1) / ALIGN - 1);
    }

    // 重新填充自由链表
    static void* refill(size_t n);

    // 分配大块内存
    static char* chunk_alloc(size_t size, int& nobjs);

public:
    // 分配内存
    static void* allocate(size_t n);

    // 释放内存
    static void deallocate(void* p, size_t n);

    // 重新分配内存
    static void* reallocate(void* p, size_t old_sz, size_t new_sz);

    // 获取内存池状态
    static size_t get_heap_size() { return heap_size; }
    static size_t get_free_list_size(size_t index);
    static void print_memory_pool_status();
};

// ============================================================================
// 内存池实现
// ============================================================================

// 静态成员定义
alloc::obj* alloc::free_list[NFREELISTS] = {0};
char* alloc::start_free = 0;
char* alloc::end_free = 0;
size_t alloc::heap_size = 0;
std::mutex alloc::mutex_;

void* alloc::allocate(size_t n) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (n > MAX_BYTES) {
        // 大块内存直接使用 malloc
        return std::malloc(n);
    }

    // 获取自由链表索引
    size_t index = free_list_index(n);
    obj* result = free_list[index];

    if (result == nullptr) {
        // 自由链表为空，重新填充
        return refill(round_up(n));
    }

    // 从自由链表中取出一个节点
    free_list[index] = result->next;
    return result;
}

void alloc::deallocate(void* p, size_t n) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (p == nullptr) {
        return;
    }

    if (n > MAX_BYTES) {
        // 大块内存直接使用 free
        std::free(p);
        return;
    }

    // 将内存块放回自由链表
    obj* q = static_cast<obj*>(p);
    size_t index = free_list_index(n);
    q->next = free_list[index];
    free_list[index] = q;
}

void* alloc::reallocate(void* p, size_t old_sz, size_t new_sz) {
    if (p == nullptr) {
        return allocate(new_sz);
    }

    if (new_sz == 0) {
        deallocate(p, old_sz);
        return nullptr;
    }

    if (old_sz == new_sz) {
        return p;
    }

    // 分配新内存
    void* new_p = allocate(new_sz);
    if (new_p == nullptr) {
        return nullptr;
    }

    // 拷贝数据
    size_t copy_sz = (old_sz < new_sz) ? old_sz : new_sz;
    std::memcpy(new_p, p, copy_sz);

    // 释放旧内存
    deallocate(p, old_sz);

    return new_p;
}

void* alloc::refill(size_t n) {
    int nobjs = 20;  // 尝试获取20个对象
    char* chunk = chunk_alloc(n, nobjs);

    if (nobjs == 1) {
        return chunk;
    }

    // 将剩余的内存块加入自由链表
    obj* result = reinterpret_cast<obj*>(chunk);
    obj* current = result;
    obj* next;

    for (int i = 1; i < nobjs; ++i) {
        next = reinterpret_cast<obj*>(chunk + i * n);
        current->next = next;
        current = next;
    }

    current->next = nullptr;

    // 将第一个节点加入自由链表
    size_t index = free_list_index(n);
    free_list[index] = result->next;

    return result;
}

char* alloc::chunk_alloc(size_t size, int& nobjs) {
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;

    if (bytes_left >= total_bytes) {
        // 内存池有足够空间
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        // 内存池有部分空间
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    } else {
        // 内存池空间不足，需要重新分配
        size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);

        // 将剩余空间加入自由链表
        if (bytes_left > 0) {
            size_t index = free_list_index(bytes_left);
            obj* obj_ptr = reinterpret_cast<obj*>(start_free);
            obj_ptr->next = free_list[index];
            free_list[index] = obj_ptr;
        }

        // 分配新内存
        start_free = static_cast<char*>(std::malloc(bytes_to_get));
        if (start_free == nullptr) {
            // 分配失败，尝试从自由链表中获取内存
            for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
                size_t index = free_list_index(i);
                obj* obj_ptr = free_list[index];
                if (obj_ptr != nullptr) {
                    free_list[index] = obj_ptr->next;
                    start_free = reinterpret_cast<char*>(obj_ptr);
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs);
                }
            }
            // 所有尝试都失败
            end_free = nullptr;
            throw std::bad_alloc();
        }

        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

size_t alloc::get_free_list_size(size_t index) {
    if (index >= NFREELISTS) {
        return 0;
    }

    size_t count = 0;
    obj* current = free_list[index];
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

void alloc::print_memory_pool_status() {
    std::cout << "=== 内存池状态 ===" << std::endl;
    std::cout << "堆大小: " << heap_size << " 字节" << std::endl;
    std::cout << "内存池起始: " << static_cast<void*>(start_free) << std::endl;
    std::cout << "内存池结束: " << static_cast<void*>(end_free) << std::endl;
    std::cout << "自由链表状态:" << std::endl;
    
    for (size_t i = 0; i < NFREELISTS; ++i) {
        size_t size = (i + 1) * ALIGN;
        size_t count = get_free_list_size(i);
        if (count > 0) {
            std::cout << "  大小 " << size << " 字节: " << count << " 个块" << std::endl;
        }
    }
}

// ============================================================================
// 内存池分配器包装器
// ============================================================================

/**
 * @brief 内存池分配器包装器
 * @tparam T 对象类型
 */
template<typename T>
class pool_allocator {
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    template<typename U>
    struct rebind {
        typedef pool_allocator<U> other;
    };

public:
    pool_allocator() noexcept = default;
    pool_allocator(const pool_allocator&) noexcept = default;
    template<typename U>
    pool_allocator(const pool_allocator<U>&) noexcept {}
    ~pool_allocator() noexcept = default;

    pool_allocator& operator=(const pool_allocator&) = default;

    pointer address(reference x) const noexcept {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept {
        return &x;
    }

    pointer allocate(size_type n, const void* hint = 0) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        
        size_type total_size = n * sizeof(T);
        pointer result = static_cast<pointer>(alloc::allocate(total_size));
        
        if (result == nullptr) {
            throw std::bad_alloc();
        }
        
        return result;
    }

    void deallocate(pointer p, size_type n) {
        if (p != nullptr) {
            alloc::deallocate(p, n * sizeof(T));
        }
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        mystl::construct(p, std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) {
        mystl::destroy(p);
    }

    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    template<typename U>
    bool operator==(const pool_allocator<U>&) const noexcept {
        return true;
    }

    template<typename U>
    bool operator!=(const pool_allocator<U>&) const noexcept {
        return false;
    }
};

// ============================================================================
// 内存池工具函数
// ============================================================================

/**
 * @brief 创建内存池分配器
 * @tparam T 对象类型
 * @return 内存池分配器实例
 */
template<typename T>
pool_allocator<T> make_pool_allocator() {
    return pool_allocator<T>();
}

/**
 * @brief 获取内存池状态
 */
inline void print_memory_pool_status() {
    alloc::print_memory_pool_status();
}

/**
 * @brief 重置内存池
 */
inline void reset_memory_pool() {
    // 注意：这个函数在实际使用中需要谨慎，因为可能影响正在使用的内存
    // 这里只是示例，实际实现需要更复杂的逻辑
}

} // namespace mystl

#endif // MYTINYSTL_ALLOC_H_



