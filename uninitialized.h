#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_

#include <memory>
#include <type_traits>
#include "construct.h"
#include "type_traits.h"
#include "exceptdef.h"

namespace mystl {

// ============================================================================
// 未初始化存储管理函数
// ============================================================================

/**
 * @brief 未初始化拷贝
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 输入起始迭代器
 * @param last 输入结束迭代器
 * @param result 输出起始迭代器
 * @return 输出结束迭代器
 */
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    ForwardIterator current = result;
    
    try {
        for (; first != last; ++first, ++current) {
            construct(&*current, *first);
        }
        return current;
    } catch (...) {
        // 异常安全：析构已构造的对象
        destroy(result, current);
        throw;
    }
}

/**
 * @brief 未初始化拷贝（特化版本 - 平凡拷贝）
 * @tparam T 对象类型
 * @param first 输入起始指针
 * @param last 输入结束指针
 * @param result 输出起始指针
 * @return 输出结束指针
 */
template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T*>::type
uninitialized_copy(T* first, T* last, T* result) {
    if (first != last) {
        std::memcpy(result, first, (last - first) * sizeof(T));
    }
    return result + (last - first);
}

/**
 * @brief 未初始化移动
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 输入起始迭代器
 * @param last 输入结束迭代器
 * @param result 输出起始迭代器
 * @return 输出结束迭代器
 */
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result) {
    ForwardIterator current = result;
    
    try {
        for (; first != last; ++first, ++current) {
            construct(&*current, std::move(*first));
        }
        return current;
    } catch (...) {
        // 异常安全：析构已构造的对象
        destroy(result, current);
        throw;
    }
}

/**
 * @brief 未初始化填充
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 对象类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 填充值
 */
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    ForwardIterator current = first;
    
    try {
        for (; current != last; ++current) {
            construct(&*current, value);
        }
    } catch (...) {
        // 异常安全：析构已构造的对象
        destroy(first, current);
        throw;
    }
}

/**
 * @brief 未初始化填充（特化版本 - 平凡类型）
 * @tparam T 对象类型
 * @param first 起始指针
 * @param last 结束指针
 * @param value 填充值
 */
template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value>::type
uninitialized_fill(T* first, T* last, const T& value) {
    if (first != last) {
        std::fill(first, last, value);
    }
}

/**
 * @brief 未初始化填充 n 个对象
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam Size 大小类型
 * @tparam T 对象类型
 * @param first 起始迭代器
 * @param n 对象数量
 * @param value 填充值
 * @return 填充结束迭代器
 */
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
    ForwardIterator current = first;
    
    try {
        for (; n > 0; --n, ++current) {
            construct(&*current, value);
        }
        return current;
    } catch (...) {
        // 异常安全：析构已构造的对象
        destroy(first, current);
        throw;
    }
}

/**
 * @brief 未初始化填充 n 个对象（特化版本 - 平凡类型）
 * @tparam T 对象类型
 * @tparam Size 大小类型
 * @param first 起始指针
 * @param n 对象数量
 * @param value 填充值
 * @return 填充结束指针
 */
template<typename T, typename Size>
typename std::enable_if<std::is_trivially_copyable<T>::value, T*>::type
uninitialized_fill_n(T* first, Size n, const T& value) {
    if (n > 0) {
        std::fill_n(first, n, value);
    }
    return first + n;
}

// ============================================================================
// 未初始化存储的异常安全版本
// ============================================================================

/**
 * @brief 异常安全的未初始化拷贝
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 输入起始迭代器
 * @param last 输入结束迭代器
 * @param result 输出起始迭代器
 * @return 拷贝成功的对象数量
 */
template<typename InputIterator, typename ForwardIterator>
typename std::iterator_traits<ForwardIterator>::difference_type
uninitialized_copy_safe(InputIterator first, InputIterator last, ForwardIterator result) {
    typename std::iterator_traits<ForwardIterator>::difference_type count = 0;
    ForwardIterator current = result;
    
    for (; first != last; ++first, ++current) {
        if (construct_safe(&*current, *first)) {
            ++count;
        } else {
            // 构造失败，析构已构造的对象
            destroy(result, current);
            break;
        }
    }
    
    return count;
}

/**
 * @brief 异常安全的未初始化填充
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 对象类型
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 填充值
 * @return 填充成功的对象数量
 */
template<typename ForwardIterator, typename T>
typename std::iterator_traits<ForwardIterator>::difference_type
uninitialized_fill_safe(ForwardIterator first, ForwardIterator last, const T& value) {
    typename std::iterator_traits<ForwardIterator>::difference_type count = 0;
    ForwardIterator current = first;
    
    for (; current != last; ++current) {
        if (construct_safe(&*current, value)) {
            ++count;
        } else {
            // 构造失败，析构已构造的对象
            destroy(first, current);
            break;
        }
    }
    
    return count;
}

// ============================================================================
// 未初始化存储的工具函数
// ============================================================================

/**
 * @brief 检查未初始化存储是否有效
 * @tparam T 对象类型
 * @param ptr 指针
 * @param n 对象数量
 * @return 是否有效
 */
template<typename T>
bool is_valid_uninitialized_storage(T* ptr, std::size_t n) {
    return ptr != nullptr && n > 0;
}

/**
 * @brief 获取未初始化存储大小
 * @tparam T 对象类型
 * @param n 对象数量
 * @return 存储大小
 */
template<typename T>
constexpr std::size_t uninitialized_storage_size(std::size_t n) {
    return n * sizeof(T);
}

/**
 * @brief 获取未初始化存储对齐
 * @tparam T 对象类型
 * @return 存储对齐
 */
template<typename T>
constexpr std::size_t uninitialized_storage_align() {
    return alignof(T);
}

// ============================================================================
// 未初始化存储的 RAII 包装器
// ============================================================================

/**
 * @brief 未初始化存储的 RAII 包装器
 * @tparam T 对象类型
 */
template<typename T>
class uninitialized_storage {
private:
    T* data_;
    std::size_t size_;
    std::size_t constructed_;

public:
    explicit uninitialized_storage(std::size_t n) 
        : data_(static_cast<T*>(std::malloc(n * sizeof(T)))), size_(n), constructed_(0) {
        if (data_ == nullptr) {
            throw std::bad_alloc();
        }
    }
    
    ~uninitialized_storage() {
        if (data_ != nullptr) {
            // 析构已构造的对象
            for (std::size_t i = 0; i < constructed_; ++i) {
                data_[i].~T();
            }
            std::free(data_);
        }
    }
    
    // 禁止拷贝
    uninitialized_storage(const uninitialized_storage&) = delete;
    uninitialized_storage& operator=(const uninitialized_storage&) = delete;
    
    // 允许移动
    uninitialized_storage(uninitialized_storage&& other) noexcept 
        : data_(other.data_), size_(other.size_), constructed_(other.constructed_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.constructed_ = 0;
    }
    
    uninitialized_storage& operator=(uninitialized_storage&& other) noexcept {
        if (this != &other) {
            // 析构当前对象
            if (data_ != nullptr) {
                for (std::size_t i = 0; i < constructed_; ++i) {
                    data_[i].~T();
                }
                std::free(data_);
            }
            
            data_ = other.data_;
            size_ = other.size_;
            constructed_ = other.constructed_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.constructed_ = 0;
        }
        return *this;
    }
    
    /**
     * @brief 构造对象
     * @tparam Args 构造参数类型
     * @param index 对象索引
     * @param args 构造参数
     * @return 构造是否成功
     */
    template<typename... Args>
    bool construct(std::size_t index, Args&&... args) {
        if (index < size_ && index == constructed_) {
            try {
                mystl::construct(data_ + index, std::forward<Args>(args)...);
                ++constructed_;
                return true;
            } catch (...) {
                return false;
            }
        }
        return false;
    }
    
    /**
     * @brief 获取对象指针
     * @param index 对象索引
     * @return 对象指针
     */
    T* get(std::size_t index) const {
        return (index < constructed_) ? data_ + index : nullptr;
    }
    
    /**
     * @brief 获取数据指针
     * @return 数据指针
     */
    T* data() const {
        return data_;
    }
    
    /**
     * @brief 获取大小
     * @return 大小
     */
    std::size_t size() const {
        return size_;
    }
    
    /**
     * @brief 获取已构造数量
     * @return 已构造数量
     */
    std::size_t constructed_count() const {
        return constructed_;
    }
    
    /**
     * @brief 检查是否已满
     * @return 是否已满
     */
    bool is_full() const {
        return constructed_ == size_;
    }
    
    /**
     * @brief 检查是否为空
     * @return 是否为空
     */
    bool is_empty() const {
        return constructed_ == 0;
    }
};

} // namespace mystl

#endif // MYTINYSTL_UNINITIALIZED_H_



