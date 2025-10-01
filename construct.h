#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

#include <new>
#include <type_traits>
#include "type_traits.h"
#include "exceptdef.h"

namespace mystl {

// ============================================================================
// 对象构造和析构函数
// ============================================================================

/**
 * @brief 构造单个对象
 * @tparam T 对象类型
 * @tparam Args 构造参数类型
 * @param ptr 对象指针
 * @param args 构造参数
 */
template<typename T, typename... Args>
void construct(T* ptr, Args&&... args) {
    if (ptr != nullptr) {
        new(ptr) T(std::forward<Args>(args)...);
    }
}

/**
 * @brief 析构单个对象
 * @tparam T 对象类型
 * @param ptr 对象指针
 */
template<typename T>
void destroy(T* ptr) {
    if (ptr != nullptr) {
        ptr->~T();
    }
}

// 注意：destroy(ForwardIterator, ForwardIterator) 已在 algobase.h 中定义

/**
 * @brief 析构多个对象（特化版本 - 平凡析构）
 * @tparam T 对象类型
 * @param first 起始指针
 * @param last 结束指针
 */
template<typename T>
typename std::enable_if<std::is_trivially_destructible<T>::value>::type
destroy(T* first, T* last) {
    // 平凡析构函数，什么都不做
}

/**
 * @brief 析构多个对象（特化版本 - 非平凡析构）
 * @tparam T 对象类型
 * @param first 起始指针
 * @param last 结束指针
 */
template<typename T>
typename std::enable_if<!std::is_trivially_destructible<T>::value>::type
destroy(T* first, T* last) {
    for (; first != last; ++first) {
        first->~T();
    }
}

// ============================================================================
// 批量构造函数
// ============================================================================

/**
 * @brief 批量构造对象（默认构造）
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template<typename ForwardIterator>
void construct_range(ForwardIterator first, ForwardIterator last) {
    for (; first != last; ++first) {
        construct(&*first);
    }
}

/**
 * @brief 批量构造对象（拷贝构造）
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 对象类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 拷贝值
 */
template<typename ForwardIterator, typename T>
void construct_range(ForwardIterator first, ForwardIterator last, const T& value) {
    for (; first != last; ++first) {
        construct(&*first, value);
    }
}

/**
 * @brief 批量构造对象（移动构造）
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 对象类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 移动值
 */
template<typename ForwardIterator, typename T>
void construct_range(ForwardIterator first, ForwardIterator last, T&& value) {
    for (; first != last; ++first) {
        construct(&*first, std::forward<T>(value));
    }
}

// ============================================================================
// 异常安全的构造函数
// ============================================================================

/**
 * @brief 异常安全的构造函数
 * @tparam T 对象类型
 * @tparam Args 构造参数类型
 * @param ptr 对象指针
 * @param args 构造参数
 * @return 构造是否成功
 */
template<typename T, typename... Args>
bool construct_safe(T* ptr, Args&&... args) {
    if (ptr == nullptr) {
        return false;
    }
    
    try {
        new(ptr) T(std::forward<Args>(args)...);
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * @brief 异常安全的批量构造
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam Args 构造参数类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param args 构造参数
 * @return 构造成功的对象数量
 */
template<typename ForwardIterator, typename... Args>
typename std::iterator_traits<ForwardIterator>::difference_type
construct_safe_range(ForwardIterator first, ForwardIterator last, Args&&... args) {
    typename std::iterator_traits<ForwardIterator>::difference_type count = 0;
    
    for (; first != last; ++first) {
        if (construct_safe(&*first, std::forward<Args>(args)...)) {
            ++count;
        } else {
            // 构造失败，析构已构造的对象
            destroy_range(first - count, first);
            break;
        }
    }
    
    return count;
}

// ============================================================================
// 工具函数
// ============================================================================

/**
 * @brief 检查对象是否已构造
 * @tparam T 对象类型
 * @param ptr 对象指针
 * @return 是否已构造
 */
template<typename T>
bool is_constructed(T* ptr) {
    return ptr != nullptr;
}

/**
 * @brief 获取对象大小
 * @tparam T 对象类型
 * @return 对象大小
 */
template<typename T>
constexpr std::size_t object_size() {
    return sizeof(T);
}

/**
 * @brief 获取对象对齐
 * @tparam T 对象类型
 * @return 对象对齐
 */
template<typename T>
constexpr std::size_t object_align() {
    return alignof(T);
}

// ============================================================================
// 构造和析构的 RAII 包装器
// ============================================================================

/**
 * @brief 构造和析构的 RAII 包装器
 * @tparam T 对象类型
 */
template<typename T>
class construct_guard {
private:
    T* ptr_;
    bool constructed_;

public:
    explicit construct_guard(T* ptr) : ptr_(ptr), constructed_(false) {}
    
    ~construct_guard() {
        if (constructed_) {
            destroy(ptr_);
        }
    }
    
    // 禁止拷贝
    construct_guard(const construct_guard&) = delete;
    construct_guard& operator=(const construct_guard&) = delete;
    
    // 允许移动
    construct_guard(construct_guard&& other) noexcept 
        : ptr_(other.ptr_), constructed_(other.constructed_) {
        other.constructed_ = false;
    }
    
    construct_guard& operator=(construct_guard&& other) noexcept {
        if (this != &other) {
            if (constructed_) {
                destroy(ptr_);
            }
            ptr_ = other.ptr_;
            constructed_ = other.constructed_;
            other.constructed_ = false;
        }
        return *this;
    }
    
    /**
     * @brief 构造对象
     * @tparam Args 构造参数类型
     * @param args 构造参数
     * @return 构造是否成功
     */
    template<typename... Args>
    bool construct(Args&&... args) {
        if (ptr_ != nullptr && !constructed_) {
            try {
                mystl::construct(ptr_, std::forward<Args>(args)...);
                constructed_ = true;
                return true;
            } catch (...) {
                return false;
            }
        }
        return false;
    }
    
    /**
     * @brief 析构对象
     */
    void destroy() {
        if (constructed_) {
            mystl::destroy(ptr_);
            constructed_ = false;
        }
    }
    
    /**
     * @brief 获取对象指针
     * @return 对象指针
     */
    T* get() const {
        return constructed_ ? ptr_ : nullptr;
    }
    
    /**
     * @brief 检查是否已构造
     * @return 是否已构造
     */
    bool is_constructed() const {
        return constructed_;
    }
    
    /**
     * @brief 释放所有权
     * @return 对象指针
     */
    T* release() {
        T* result = constructed_ ? ptr_ : nullptr;
        constructed_ = false;
        return result;
    }
};

} // namespace mystl

#endif // MYTINYSTL_CONSTRUCT_H_



