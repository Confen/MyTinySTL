#ifndef MYTINYSTL_MEMORY_H_
#define MYTINYSTL_MEMORY_H_

#include <cstddef>
#include <memory>
#include <type_traits>

#include "allocator.h"
#include "alloc.h"
#include "util.h"

namespace mystl {

// ============================================================================
// 智能指针
// ============================================================================

/**
 * @brief 简单智能指针实现
 * @tparam T 指针类型
 */
template<typename T>
class unique_ptr {
private:
    T* ptr_;

public:
    typedef T element_type;

    // 构造函数
    explicit unique_ptr(T* p = nullptr) noexcept : ptr_(p) {}
    
    // 禁用拷贝构造和赋值
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // 移动构造和赋值
    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.release()) {}
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            reset(other.release());
        }
        return *this;
    }

    // 析构函数
    ~unique_ptr() {
        if (ptr_) {
            delete ptr_;
        }
    }

    // 获取指针
    T* get() const noexcept { return ptr_; }
    T* release() noexcept {  //返回的对象的原始指针，智能指针不再管理这个对象，智能指针变为空
        T* old = ptr_;
        ptr_ = nullptr;
        return old;
    }

    // 重置指针
    void reset(T* p = nullptr) noexcept {
        if (ptr_) {
            delete ptr_;
        }
        ptr_ = p;
    }

    // 操作符重载
    T& operator*() const { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    // 比较操作
    bool operator==(const unique_ptr& other) const noexcept {
        return ptr_ == other.ptr_;
    }
    bool operator!=(const unique_ptr& other) const noexcept {
        return ptr_ != other.ptr_;
    }
};

/**
 * @brief 创建 unique_ptr
 * @tparam T 类型
 * @tparam Args 构造函数参数类型
 * @param args 构造函数参数
 * @return unique_ptr 实例
 */
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(mystl::forward<Args>(args)...));
}

// ============================================================================
// 内存工具函数
// ============================================================================

/**
 * @brief 获取对象地址
 * @tparam T 对象类型
 * @param obj 对象引用
 * @return 对象地址
 */
template<typename T>
T* addressof(T& obj) noexcept {
    return reinterpret_cast<T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(obj)
        )
    );
}

/**
 * @brief 获取对象地址（const版本）
 * @tparam T 对象类型
 * @param obj 对象引用
 * @return 对象地址
 */
template<typename T>
const T* addressof(const T& obj) noexcept {
    return reinterpret_cast<const T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(obj)
        )
    );
}

/**
 * @brief 对齐内存分配
 * @param size 分配大小
 * @param alignment 对齐要求
 * @return 对齐的内存指针
 */
inline void* aligned_alloc(size_t size, size_t alignment) {
    // 简单的对齐分配实现
    size_t total_size = size + alignment - 1;
    void* ptr = std::malloc(total_size);
    if (ptr == nullptr) {
        return nullptr;
    }
    
    // 计算对齐地址
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t aligned_addr = (addr + alignment - 1) & ~(alignment - 1);
    
    return reinterpret_cast<void*>(aligned_addr);
}

/**
 * @brief 对齐内存释放
 * @param ptr 内存指针
 */
inline void aligned_free(void* ptr) {
    std::free(ptr);
}

// ============================================================================
// 内存管理工具
// ============================================================================

/**
 * @brief 内存池管理器
 */
class memory_manager {
private:
    static size_t total_allocated_;
    static size_t total_deallocated_;
    static size_t peak_usage_;

public:
    static void record_allocation(size_t size) {
        total_allocated_ += size;
        if (total_allocated_ - total_deallocated_ > peak_usage_) {
            peak_usage_ = total_allocated_ - total_deallocated_;
        }
    }

    static void record_deallocation(size_t size) {
        total_deallocated_ += size;
    }

    static size_t get_total_allocated() { return total_allocated_; }
    static size_t get_total_deallocated() { return total_deallocated_; }
    static size_t get_current_usage() { return total_allocated_ - total_deallocated_; }
    static size_t get_peak_usage() { return peak_usage_; }

    static void reset() {
        total_allocated_ = 0;
        total_deallocated_ = 0;
        peak_usage_ = 0;
    }

    static void print_status() {
        std::cout << "=== 内存管理器状态 ===" << std::endl;
        std::cout << "总分配: " << total_allocated_ << " 字节" << std::endl;
        std::cout << "总释放: " << total_deallocated_ << " 字节" << std::endl;
        std::cout << "当前使用: " << get_current_usage() << " 字节" << std::endl;
        std::cout << "峰值使用: " << peak_usage_ << " 字节" << std::endl;
    }
};

// 静态成员定义
size_t memory_manager::total_allocated_ = 0;
size_t memory_manager::total_deallocated_ = 0;
size_t memory_manager::peak_usage_ = 0;

// ============================================================================
// 内存优化工具
// ============================================================================

/**
 * @brief 内存对齐计算
 * @param size 原始大小
 * @param alignment 对齐要求
 * @return 对齐后的大小
 */
inline size_t align_size(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

/**
 * @brief 检查内存对齐
 * @param ptr 内存指针
 * @param alignment 对齐要求
 * @return 是否对齐
 */
inline bool is_aligned(const void* ptr, size_t alignment) {
    return (reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)) == 0;
}

/**
 * @brief 获取内存页大小
 * @return 内存页大小
 */
inline size_t get_page_size() {
    static size_t page_size = 0;
    if (page_size == 0) {
        // 简单的页面大小检测
        page_size = 4096; // 默认4KB
    }
    return page_size;
}

// ============================================================================
// 内存调试工具
// ============================================================================

/**
 * @brief 内存调试信息
 */
class memory_debug {
private:
    static bool debug_enabled_;
    static size_t allocation_count_;
    static size_t deallocation_count_;

public:
    static void enable_debug() { debug_enabled_ = true; }
    static void disable_debug() { debug_enabled_ = false; }
    static bool is_debug_enabled() { return debug_enabled_; }

    static void record_allocation(void* ptr, size_t size) {
        if (debug_enabled_) {
            ++allocation_count_;
            std::cout << "分配内存: " << ptr << ", 大小: " << size << std::endl;
        }
    }

    static void record_deallocation(void* ptr, size_t size) {
        if (debug_enabled_) {
            ++deallocation_count_;
            std::cout << "释放内存: " << ptr << ", 大小: " << size << std::endl;
        }
    }

    static size_t get_allocation_count() { return allocation_count_; }
    static size_t get_deallocation_count() { return deallocation_count_; }

    static void reset() {
        allocation_count_ = 0;
        deallocation_count_ = 0;
    }

    static void print_summary() {
        std::cout << "=== 内存调试摘要 ===" << std::endl;
        std::cout << "分配次数: " << allocation_count_ << std::endl;
        std::cout << "释放次数: " << deallocation_count_ << std::endl;
        std::cout << "未释放次数: " << allocation_count_ - deallocation_count_ << std::endl;
    }
};

// 静态成员定义
bool memory_debug::debug_enabled_ = false;
size_t memory_debug::allocation_count_ = 0;
size_t memory_debug::deallocation_count_ = 0;

// ============================================================================
// 内存泄漏检测
// ============================================================================

/**
 * @brief 内存泄漏检测器
 */
class memory_leak_detector {
private:
    struct allocation_info {
        void* ptr;
        size_t size;
        const char* file;
        int line;
        allocation_info* next;
    };

    static allocation_info* allocations_;
    static size_t total_allocated_;

public:
    static void record_allocation(void* ptr, size_t size, const char* file, int line) {
        allocation_info* info = new allocation_info;
        info->ptr = ptr;
        info->size = size;
        info->file = file;
        info->line = line;
        info->next = allocations_;
        allocations_ = info;
        total_allocated_ += size;
    }

    static void record_deallocation(void* ptr) {
        allocation_info** current = &allocations_;
        while (*current) {
            if ((*current)->ptr == ptr) {
                allocation_info* to_delete = *current;
                *current = (*current)->next;
                total_allocated_ -= to_delete->size;
                delete to_delete;
                return;
            }
            current = &(*current)->next;
        }
    }

    static void check_leaks() {
        if (allocations_ == nullptr) {
            std::cout << "没有检测到内存泄漏" << std::endl;
            return;
        }

        std::cout << "=== 检测到内存泄漏 ===" << std::endl;
        allocation_info* current = allocations_;
        while (current) {
            std::cout << "泄漏: " << current->ptr 
                      << ", 大小: " << current->size 
                      << ", 文件: " << current->file 
                      << ", 行: " << current->line << std::endl;
            current = current->next;
        }
        std::cout << "总泄漏大小: " << total_allocated_ << " 字节" << std::endl;
    }

    static void reset() {
        allocation_info* current = allocations_;
        while (current) {
            allocation_info* next = current->next;
            delete current;
            current = next;
        }
        allocations_ = nullptr;
        total_allocated_ = 0;
    }
};

// 静态成员定义
memory_leak_detector::allocation_info* memory_leak_detector::allocations_ = nullptr;
size_t memory_leak_detector::total_allocated_ = 0;

// ============================================================================
// 内存工具宏
// ============================================================================

#ifdef MYTINYSTL_DEBUG
#define MYTINYSTL_ALLOC(size) \
    ({ \
        void* ptr = std::malloc(size); \
        mystl::memory_debug::record_allocation(ptr, size); \
        mystl::memory_leak_detector::record_allocation(ptr, size, __FILE__, __LINE__); \
        ptr; \
    })

#define MYTINYSTL_FREE(ptr, size) \
    do { \
        mystl::memory_debug::record_deallocation(ptr, size); \
        mystl::memory_leak_detector::record_deallocation(ptr); \
        std::free(ptr); \
    } while(0)
#else
#define MYTINYSTL_ALLOC(size) std::malloc(size)
#define MYTINYSTL_FREE(ptr, size) std::free(ptr)
#endif

} // namespace mystl

#endif // MYTINYSTL_MEMORY_H_
