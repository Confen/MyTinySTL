#ifndef MYTINYSTL_ALLOCATOR_H_
#define MYTINYSTL_ALLOCATOR_H_

#include <cstddef>
#include <new>
#include <type_traits>
#include "construct.h"
#include "exceptdef.h"

namespace mystl {

// ============================================================================
// 标准分配器
// ============================================================================

/**
 * @brief 标准分配器模板类
 * @tparam T 分配的对象类型
 */
template<typename T>
class allocator {
public:
    // 类型定义
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    // 分配器特征
    template<typename U>
    struct rebind {
        typedef allocator<U> other;
    };

public:
    // 构造函数和析构函数
    allocator() noexcept = default;
    allocator(const allocator&) noexcept = default;
    template<typename U>
    allocator(const allocator<U>&) noexcept {}
    ~allocator() noexcept = default;

    // 赋值操作符
    allocator& operator=(const allocator&) = default;

    // 地址获取
    pointer address(reference x) const noexcept {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept {
        return &x;
    }

    // 内存分配
    pointer allocate(size_type n, const void* hint = 0) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        
        size_type total_size = n * sizeof(T);
        pointer result = static_cast<pointer>(::operator new(total_size));
        
        if (result == nullptr) {
            throw std::bad_alloc();
        }
        
        return result;
    }

    // 内存释放
    void deallocate(pointer p, size_type n) {
        if (p != nullptr) {
            ::operator delete(p);
        }
    }

    // 对象构造
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        mystl::construct(p, std::forward<Args>(args)...);
    }

    // 对象析构
    template<typename U>
    void destroy(U* p) {
        mystl::destroy(p);
    }

    // 最大分配大小
    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    // 比较操作
    template<typename U>
    bool operator==(const allocator<U>&) const noexcept {
        return true;
    }

    template<typename U>
    bool operator!=(const allocator<U>&) const noexcept {
        return false;
    }
};

// ============================================================================
// 特化分配器
// ============================================================================

/**
 * @brief void 类型特化分配器
 */
template<>
class allocator<void> {
public:
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;

    template<typename U>
    struct rebind {
        typedef allocator<U> other;
    };
};

// ============================================================================
// 分配器工具函数
// ============================================================================

/**
 * @brief 分配器特征
 * @tparam Alloc 分配器类型
 */
template<typename Alloc>
struct allocator_traits {
    typedef Alloc allocator_type;
    typedef typename Alloc::value_type value_type;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_pointer const_pointer;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_reference const_reference;
    typedef typename Alloc::size_type size_type;
    typedef typename Alloc::difference_type difference_type;

    // 分配内存
    static pointer allocate(allocator_type& a, size_type n) {
        return a.allocate(n);
    }

    static pointer allocate(allocator_type& a, size_type n, const_pointer hint) {
        return a.allocate(n, hint);
    }

    // 释放内存
    static void deallocate(allocator_type& a, pointer p, size_type n) {
        a.deallocate(p, n);
    }

    // 构造对象
    template<typename T, typename... Args>
    static void construct(allocator_type& a, T* p, Args&&... args) {
        a.construct(p, std::forward<Args>(args)...);
    }

    // 析构对象
    template<typename T>
    static void destroy(allocator_type& a, T* p) {
        a.destroy(p);
    }

    // 最大分配大小
    static size_type max_size(const allocator_type& a) {
        return a.max_size();
    }

    // 获取地址
    static pointer address(allocator_type& a, reference x) {
        return a.address(x);
    }

    static const_pointer address(const allocator_type& a, const_reference x) {
        return a.address(x);
    }
};

// ============================================================================
// 分配器选择器
// ============================================================================

/**
 * @brief 分配器选择器
 * @tparam T 对象类型
 * @tparam Alloc 分配器类型
 */
template<typename T, typename Alloc = allocator<T>>
class allocator_selector {
public:
    typedef Alloc allocator_type;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

private:
    allocator_type alloc_;

public:
    // 构造函数
    allocator_selector() = default;
    explicit allocator_selector(const allocator_type& a) : alloc_(a) {}
    template<typename U>
    allocator_selector(const allocator_selector<U, typename Alloc::template rebind<U>::other>& other)
        : alloc_(other.get_allocator()) {}

    // 获取分配器
    allocator_type get_allocator() const {
        return alloc_;
    }

    // 分配内存
    pointer allocate(size_type n) {
        return alloc_.allocate(n);
    }

    pointer allocate(size_type n, const_pointer hint) {
        return alloc_.allocate(n, hint);
    }

    // 释放内存
    void deallocate(pointer p, size_type n) {
        alloc_.deallocate(p, n);
    }

    // 构造对象
    template<typename... Args>
    void construct(pointer p, Args&&... args) {
        alloc_.construct(p, std::forward<Args>(args)...);
    }

    // 析构对象
    void destroy(pointer p) {
        alloc_.destroy(p);
    }

    // 最大分配大小
    size_type max_size() const {
        return alloc_.max_size();
    }
};

// ============================================================================
// 分配器比较
// ============================================================================

/**
 * @brief 分配器比较
 * @tparam T1 第一个分配器类型
 * @tparam T2 第二个分配器类型
 */
template<typename T1, typename T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
    return true;
}

template<typename T1, typename T2>
bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept {
    return false;
}

// ============================================================================
// 分配器工具函数
// ============================================================================

/**
 * @brief 创建分配器
 * @tparam T 对象类型
 * @return 分配器实例
 */
template<typename T>
allocator<T> make_allocator() {
    return allocator<T>();
}

/**
 * @brief 分配器交换
 * @tparam T 对象类型
 * @param a 第一个分配器
 * @param b 第二个分配器
 */
template<typename T>
void swap(allocator<T>& a, allocator<T>& b) noexcept {
    // 分配器是无状态的，交换是空操作
}

// ============================================================================
// 分配器异常
// ============================================================================

/**
 * @brief 分配器异常类
 */
class allocator_exception : public mystl_exception {
public:
    explicit allocator_exception(const char* msg) : mystl_exception(msg) {}
    explicit allocator_exception(const std::string& msg) : mystl_exception(msg.c_str()) {}
};

/**
 * @brief 内存不足异常
 */
class bad_alloc : public allocator_exception {
public:
    bad_alloc() : allocator_exception("bad_alloc: memory allocation failed") {}
    explicit bad_alloc(const char* msg) : allocator_exception(msg) {}
};

// ============================================================================
// 分配器调试工具
// ============================================================================

/**
 * @brief 分配器调试信息
 * @tparam T 对象类型
 */
template<typename T>
class allocator_debug {
private:
    static size_t allocation_count_;
    static size_t deallocation_count_;
    static size_t total_allocated_;

public:
    static void record_allocation(size_t size) {
        ++allocation_count_;
        total_allocated_ += size;
    }

    static void record_deallocation(size_t size) {
        ++deallocation_count_;
        total_allocated_ -= size;
    }

    static size_t get_allocation_count() {
        return allocation_count_;
    }

    static size_t get_deallocation_count() {
        return deallocation_count_;
    }

    static size_t get_total_allocated() {
        return total_allocated_;
    }

    static void reset() {
        allocation_count_ = 0;
        deallocation_count_ = 0;
        total_allocated_ = 0;
    }
};

// 静态成员定义
template<typename T>
size_t allocator_debug<T>::allocation_count_ = 0;

template<typename T>
size_t allocator_debug<T>::deallocation_count_ = 0;

template<typename T>
size_t allocator_debug<T>::total_allocated_ = 0;

} // namespace mystl

#endif // MYTINYSTL_ALLOCATOR_H_



