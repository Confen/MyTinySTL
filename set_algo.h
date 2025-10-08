#ifndef MYTINYSTL_SET_ALGO_H
#define MYTINYSTL_SET_ALGO_H

#include <cstddef>
#include <algorithm>

#include "algobase.h"
#include "functional.h"
#include "iterator.h"

namespace mystl {

// ============================================================================
// 集合算法
// ============================================================================

/**
 * @brief 检查第一个有序范围是否包含第二个有序范围的所有元素
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param comp 比较函数对象
 * @return 如果第一个范围包含第二个范围的所有元素返回true，否则返回false
 */
template<typename InputIterator1, typename InputIterator2, typename Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2,
              Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            return false;
        } else if (comp(*first1, *first2)) {
            ++first1;
        } else {
            ++first1;
            ++first2;
        }
    }
    return first2 == last2;
}

/**
 * @brief 检查第一个有序范围是否包含第二个有序范围的所有元素（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @return 如果第一个范围包含第二个范围的所有元素返回true，否则返回false
 */
template<typename InputIterator1, typename InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2) {
    return mystl::includes(first1, last1, first2, last2, 
                          mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

/**
 * @brief 计算两个有序范围的并集
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @param comp 比较函数对象
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    
    // 复制剩余元素
    while (first1 != last1) {
        *result = *first1;
        ++first1;
        ++result;
    }
    while (first2 != last2) {
        *result = *first2;
        ++first2;
        ++result;
    }
    
    return result;
}

/**
 * @brief 计算两个有序范围的并集（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        OutputIterator result) {
    return mystl::set_union(first1, last1, first2, last2, result,
                           mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

/**
 * @brief 计算两个有序范围的交集
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @param comp 比较函数对象
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            ++first1;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

/**
 * @brief 计算两个有序范围的交集（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result) {
    return mystl::set_intersection(first1, last1, first2, last2, result,
                                  mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

/**
 * @brief 计算两个有序范围的差集
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @param comp 比较函数对象
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    
    // 复制第一个范围的剩余元素
    while (first1 != last1) {
        *result = *first1;
        ++first1;
        ++result;
    }
    
    return result;
}

/**
 * @brief 计算两个有序范围的差集（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result) {
    return mystl::set_difference(first1, last1, first2, last2, result,
                                mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

/**
 * @brief 计算两个有序范围的对称差集
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @param comp 比较函数对象
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                       InputIterator2 first2, InputIterator2 last2,
                                       OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    
    // 复制剩余元素
    while (first1 != last1) {
        *result = *first1;
        ++first1;
        ++result;
    }
    while (first2 != last2) {
        *result = *first2;
        ++first2;
        ++result;
    }
    
    return result;
}

/**
 * @brief 计算两个有序范围的对称差集（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                       InputIterator2 first2, InputIterator2 last2,
                                       OutputIterator result) {
    return mystl::set_symmetric_difference(first1, last1, first2, last2, result,
                                          mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

/**
 * @brief 合并两个有序范围
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @param comp 比较函数对象
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, InputIterator2 last2,
                    OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    
    // 复制剩余元素
    while (first1 != last1) {
        *result = *first1;
        ++first1;
        ++result;
    }
    while (first2 != last2) {
        *result = *first2;
        ++first2;
        ++result;
    }
    
    return result;
}

/**
 * @brief 合并两个有序范围（使用operator<）
 * @param first1 第一个范围的开始
 * @param last1 第一个范围的结束
 * @param first2 第二个范围的开始
 * @param last2 第二个范围的结束
 * @param result 结果范围的开始
 * @return 结果范围的结束迭代器
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, InputIterator2 last2,
                    OutputIterator result) {
    return mystl::merge(first1, last1, first2, last2, result,
                       mystl::less<typename mystl::iterator_traits<InputIterator1>::value_type>());
}

} // namespace mystl

#endif // MYTINYSTL_SET_ALGO_H

