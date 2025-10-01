#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

#include <cstring>
#include <type_traits>
#include <algorithm>

#include "iterator.h"
#include "util.h"
#include "type_traits.h"

namespace mystl {

// ============================================================================
// 算法基础函数
// ============================================================================

// swap 函数已在 util.h 中定义，这里不再重复定义

/**
 * @brief 交换两个迭代器指向的值
 * @param a 第一个迭代器
 * @param b 第二个迭代器
 */
template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        mystl::swap(*first1, *first2);
    }
    return first2;
}

/**
 * @brief 交换两个迭代器范围的值
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 */
template<typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
    mystl::swap(*a, *b);
}

// ============================================================================
// 比较算法
// ============================================================================

/**
 * @brief 比较两个范围是否相等
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @return 如果两个范围相等返回 true，否则返回 false
 */
template<typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 使用自定义比较函数比较两个范围是否相等
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param binary_pred 二元谓词
 * @return 如果两个范围相等返回 true，否则返回 false
 */
template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
           BinaryPredicate binary_pred) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!binary_pred(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 比较两个范围的字典序
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @return 如果第一个范围小于第二个范围返回 true，否则返回 false
 */
template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2) {
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
        if (*first1 < *first2) {
            return true;
        }
        if (*first2 < *first1) {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}

/**
 * @brief 使用自定义比较函数比较两个范围的字典序
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param comp 比较函数
 * @return 如果第一个范围小于第二个范围返回 true，否则返回 false
 */
template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              Compare comp) {
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
        if (comp(*first1, *first2)) {
            return true;
        }
        if (comp(*first2, *first1)) {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}

// ============================================================================
// 拷贝算法
// ============================================================================

/**
 * @brief 拷贝范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

/**
 * @brief 反向拷贝范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result) {
    while (first != last) {
        *(--result) = *(--last);
    }
    return result;
}

/**
 * @brief 移动范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator move(InputIterator first, InputIterator last, OutputIterator result) {
    for (; first != last; ++first, ++result) {
        *result = mystl::move(*first);
    }
    return result;
}

/**
 * @brief 反向移动范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 move_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result) {
    while (first != last) {
        *(--result) = mystl::move(*(--last));
    }
    return result;
}

// ============================================================================
// 填充算法
// ============================================================================

/**
 * @brief 用值填充范围
 * @param first 范围的开始
 * @param last 范围的结束
 * @param value 填充值
 */
template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

/**
 * @brief 用值填充 n 个元素
 * @param first 范围的开始
 * @param n 元素数量
 * @param value 填充值
 * @return 填充后的结束迭代器
 */
template<typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
    for (; n > 0; --n, ++first) {
        *first = value;
    }
    return first;
}

// ============================================================================
// 初始化算法
// ============================================================================

/**
 * @brief 用默认值初始化范围
 * @param first 范围的开始
 * @param last 范围的结束
 */
template<typename ForwardIterator>
void uninitialized_default_construct(ForwardIterator first, ForwardIterator last) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = first;
    try {
        for (; current != last; ++current) {
            ::new (static_cast<void*>(&*current)) value_type;
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

/**
 * @brief 用默认值初始化 n 个元素
 * @param first 范围的开始
 * @param n 元素数量
 * @return 初始化后的结束迭代器
 */
template<typename ForwardIterator, typename Size>
ForwardIterator uninitialized_default_construct_n(ForwardIterator first, Size n) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = first;
    try {
        for (; n > 0; --n, ++current) {
            ::new (static_cast<void*>(&*current)) value_type;
        }
        return current;
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

/**
 * @brief 用值初始化范围
 * @param first 范围的开始
 * @param last 范围的结束
 * @param value 初始化值
 */
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = first;
    try {
        for (; current != last; ++current) {
            ::new (static_cast<void*>(&*current)) value_type(value);
        }
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

/**
 * @brief 用值初始化 n 个元素
 * @param first 范围的开始
 * @param n 元素数量
 * @param value 初始化值
 * @return 初始化后的结束迭代器
 */
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = first;
    try {
        for (; n > 0; --n, ++current) {
            ::new (static_cast<void*>(&*current)) value_type(value);
        }
        return current;
    } catch (...) {
        for (; first != current; ++first) {
            first->~value_type();
        }
        throw;
    }
}

/**
 * @brief 拷贝构造未初始化的范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                   ForwardIterator result) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = result;
    try {
        for (; first != last; ++first, ++current) {
            ::new (static_cast<void*>(&*current)) value_type(*first);
        }
        return current;
    } catch (...) {
        for (; result != current; ++result) {
            result->~value_type();
        }
        throw;
    }
}

/**
 * @brief 移动构造未初始化的范围
 * @param first 源范围的开始
 * @param last 源范围的结束
 * @param result 目标范围的开始
 * @return 目标范围的结束迭代器
 */
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last,
                                   ForwardIterator result) {
    using value_type = typename iterator_traits<ForwardIterator>::value_type;
    ForwardIterator current = result;
    try {
        for (; first != last; ++first, ++current) {
            ::new (static_cast<void*>(&*current)) value_type(mystl::move(*first));
        }
        return current;
    } catch (...) {
        for (; result != current; ++result) {
            result->~value_type();
        }
        throw;
    }
}

// ============================================================================
// 销毁算法
// ============================================================================

/**
 * @brief 销毁范围中的对象
 * @param first 范围的开始
 * @param last 范围的结束
 */
template<typename ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) {
    for (; first != last; ++first) {
        first->~value_type();
    }
}

/**
 * @brief 销毁 n 个对象
 * @param first 范围的开始
 * @param n 对象数量
 * @return 销毁后的结束迭代器
 */
template<typename ForwardIterator, typename Size>
ForwardIterator destroy_n(ForwardIterator first, Size n) {
    for (; n > 0; --n, ++first) {
        first->~value_type();
    }
    return first;
}

// ============================================================================
// 特化版本（针对 POD 类型优化）
// ============================================================================

/**
 * @brief POD 类型的拷贝特化
 */
template<>
inline char* copy(const char* first, const char* last, char* result) {
    std::memmove(result, first, last - first);
    return result + (last - first);
}

template<>
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    std::memmove(result, first, (last - first) * sizeof(wchar_t));
    return result + (last - first);
}

/**
 * @brief POD 类型的填充特化
 */
template<>
inline void fill(char* first, char* last, const char& value) {
    std::memset(first, value, last - first);
}

template<>
inline void fill(wchar_t* first, wchar_t* last, const wchar_t& value) {
    std::wmemset(first, value, last - first);
}

/**
 * @brief POD 类型的未初始化拷贝特化
 */
template<>
inline char* uninitialized_copy(const char* first, const char* last, char* result) {
    std::memmove(result, first, last - first);
    return result + (last - first);
}

template<>
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    std::memmove(result, first, (last - first) * sizeof(wchar_t));
    return result + (last - first);
}

} // namespace mystl

#endif // MYTINYSTL_ALGOBASE_H
