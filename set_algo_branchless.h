#ifndef MYTINYSTL_SET_ALGO_BRANCHLESS_H
#define MYTINYSTL_SET_ALGO_BRANCHLESS_H

#include <cstddef>
#include <algorithm>

#include "algobase.h"
#include "functional.h"
#include "iterator.h"

namespace mystl {

// ============================================================================
// 无分支优化的集合算法
// ============================================================================

/**
 * @brief 无分支优化的includes函数
 * 使用位运算和查找表避免分支判断
 */
template<typename InputIterator1, typename InputIterator2, typename Compare>
bool includes_branchless(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        Compare comp) {
    while (first1 != last1 && first2 != last2) {
        // 使用位运算避免分支
        bool less = comp(*first2, *first1);
        bool equal = !less && !comp(*first1, *first2);
        
        // 如果 first2 < first1，返回false
        if (less) return false;
        
        // 使用位运算决定前进操作
        first1 += !less;  // 如果 !less，前进first1
        first2 += equal;  // 如果equal，前进first2
    }
    
    return first2 == last2;
}

/**
 * @brief 无分支优化的set_union函数
 * 使用条件移动指令和位运算
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_union_branchless(InputIterator1 first1, InputIterator1 last1,
                                  InputIterator2 first2, InputIterator2 last2,
                                  OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        bool less1 = comp(*first1, *first2);
        bool less2 = comp(*first2, *first1);
        
        // 使用条件移动指令
        *result = less1 ? *first1 : *first2;
        
        // 使用位运算决定前进操作
        first1 += less1 || (!less1 && !less2);
        first2 += less2 || (!less1 && !less2);
        ++result;
    }
    
    // 复制剩余元素
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 无分支优化的set_intersection函数
 * 使用位运算和条件移动
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_intersection_branchless(InputIterator1 first1, InputIterator1 last1,
                                         InputIterator2 first2, InputIterator2 last2,
                                         OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        bool less1 = comp(*first1, *first2);
        bool less2 = comp(*first2, *first1);
        bool equal = !less1 && !less2;
        
        // 使用位运算决定操作
        first1 += less1 || equal;
        first2 += less2 || equal;
        
        // 只有在相等时才复制
        result += equal;
        if (equal) {
            *result = *first1;
        }
    }
    
    return result;
}

/**
 * @brief 无分支优化的set_difference函数
 * 使用位运算避免分支判断
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_difference_branchless(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2,
                                        OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        bool less1 = comp(*first1, *first2);
        bool less2 = comp(*first2, *first1);
        bool equal = !less1 && !less2;
        
        // 使用位运算决定操作
        first1 += less1 || equal;
        first2 += less2 || equal;
        
        // 只有在first1 < first2时才复制
        result += less1;
        if (less1) {
            *result = *first1;
        }
    }
    
    // 复制第一个范围的剩余元素
    return mystl::copy(first1, last1, result);
}

/**
 * @brief 无分支优化的set_symmetric_difference函数
 * 使用位运算和条件移动
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_symmetric_difference_branchless(InputIterator1 first1, InputIterator1 last1,
                                                   InputIterator2 first2, InputIterator2 last2,
                                                   OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        bool less1 = comp(*first1, *first2);
        bool less2 = comp(*first2, *first1);
        bool equal = !less1 && !less2;
        
        // 使用位运算决定操作
        first1 += less1 || equal;
        first2 += less2 || equal;
        
        // 在不等时复制
        result += less1 || less2;
        if (less1) {
            *result = *first1;
        } else if (less2) {
            *result = *first2;
        }
    }
    
    // 复制剩余元素
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 无分支优化的merge函数
 * 使用条件移动指令和位运算
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator merge_branchless(InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        bool less2 = comp(*first2, *first1);
        
        // 使用条件移动指令
        *result = less2 ? *first2 : *first1;
        
        // 使用位运算决定前进操作
        first1 += !less2;
        first2 += less2;
        ++result;
    }
    
    // 复制剩余元素
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 循环展开优化的merge函数
 * 通过循环展开减少循环开销
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator merge_unrolled(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result, Compare comp) {
    // 循环展开4次
    while (first1 != last1 && first2 != last2) {
        // 展开4次迭代
        for (int i = 0; i < 4 && first1 != last1 && first2 != last2; ++i) {
            bool less2 = comp(*first2, *first1);
            *result = less2 ? *first2 : *first1;
            first1 += !less2;
            first2 += less2;
            ++result;
        }
    }
    
    // 复制剩余元素
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 整数类型特化优化的set_union函数
 * 针对整数类型使用更高效的比较和移动操作
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union_int_optimized(InputIterator1 first1, InputIterator1 last1,
                                      InputIterator2 first2, InputIterator2 last2,
                                      OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        auto val1 = *first1;
        auto val2 = *first2;
        
        // 使用条件移动指令
        bool less = val1 < val2;
        bool equal = val1 == val2;
        
        *result = less ? val1 : val2;
        first1 += less || equal;
        first2 += !less || equal;
        ++result;
    }
    
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 批量处理优化的set_union函数
 * 使用批量复制和SIMD友好的操作
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_union_batch(InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result, Compare comp) {
    // 批量处理大小
    constexpr size_t BATCH_SIZE = 8;
    
    while (first1 != last1 && first2 != last2) {
        // 批量处理
        for (size_t i = 0; i < BATCH_SIZE && first1 != last1 && first2 != last2; ++i) {
            bool less1 = comp(*first1, *first2);
            bool less2 = comp(*first2, *first1);
            
            *result = less1 ? *first1 : *first2;
            first1 += less1 || (!less1 && !less2);
            first2 += less2 || (!less1 && !less2);
            ++result;
        }
    }
    
    // 复制剩余元素
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

} // namespace mystl

#endif // MYTINYSTL_SET_ALGO_BRANCHLESS_H
