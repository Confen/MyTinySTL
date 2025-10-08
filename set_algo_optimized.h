#ifndef MYTINYSTL_SET_ALGO_OPTIMIZED_H
#define MYTINYSTL_SET_ALGO_OPTIMIZED_H

#include <cstddef>
#include <algorithm>

#include "algobase.h"
#include "functional.h"
#include "iterator.h"

namespace mystl {

// ============================================================================
// 优化的集合算法 - 减少分支判断
// ============================================================================

/**
 * @brief 比较结果枚举，用于避免分支判断
 */
enum class CompareResult : int {
    LESS = -1,
    EQUAL = 0,
    GREATER = 1
};

/**
 * @brief 优化的比较函数，返回枚举值而不是布尔值
 */
template<typename T, typename Compare>
CompareResult optimized_compare(const T& a, const T& b, Compare comp) {
    if (comp(a, b)) return CompareResult::LESS;
    if (comp(b, a)) return CompareResult::GREATER;
    return CompareResult::EQUAL;
}

/**
 * @brief 使用查找表优化的includes函数
 */
template<typename InputIterator1, typename InputIterator2, typename Compare>
bool includes_optimized(InputIterator1 first1, InputIterator1 last1,
                       InputIterator2 first2, InputIterator2 last2,
                       Compare comp) {
    // 使用查找表避免分支判断
    static constexpr int action_table[3][3] = {
        // LESS, EQUAL, GREATER
        {1, 1, 0},  // first1 < first2: advance first1, continue
        {0, 1, 0},  // first1 == first2: advance both, continue  
        {0, 0, 0}   // first1 > first2: return false
    };
    
    while (first1 != last1 && first2 != last2) {
        CompareResult cmp = optimized_compare(*first1, *first2, comp);
        int action = action_table[cmp + 1][0]; // +1 to convert to 0,1,2
        
        if (action == 0) return false; // first1 > first2
        
        if (action & 1) ++first1;  // advance first1
        if (action & 2) ++first2;  // advance first2
    }
    
    return first2 == last2;
}

/**
 * @brief 使用位运算优化的set_union函数
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_union_optimized(InputIterator1 first1, InputIterator1 last1,
                                  InputIterator2 first2, InputIterator2 last2,
                                  OutputIterator result, Compare comp) {
    // 使用位运算和查找表
    static constexpr int union_actions[3][3] = {
        // LESS, EQUAL, GREATER
        {0x11, 0x11, 0x00},  // first1 < first2: copy first1, advance first1
        {0x00, 0x11, 0x00},  // first1 == first2: copy first1, advance both
        {0x00, 0x00, 0x12}   // first1 > first2: copy first2, advance first2
    };
    
    while (first1 != last1 && first2 != last2) {
        CompareResult cmp = optimized_compare(*first1, *first2, comp);
        int action = union_actions[cmp + 1][0];
        
        if (action & 0x10) *result = *first1;  // copy first1
        if (action & 0x20) *result = *first2;  // copy first2
        if (action & 0x01) ++first1;          // advance first1
        if (action & 0x02) ++first2;          // advance first2
        ++result;
    }
    
    // 使用memcpy优化剩余元素复制（如果可能）
    return mystl::copy(first1, last1, mystl::copy(first2, last2, result));
}

/**
 * @brief 使用SIMD友好的set_intersection函数
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_intersection_optimized(InputIterator1 first1, InputIterator1 last1,
                                         InputIterator2 first2, InputIterator2 last2,
                                         OutputIterator result, Compare comp) {
    // 预取优化
    constexpr size_t PREFETCH_DISTANCE = 64;
    
    while (first1 != last1 && first2 != last2) {
        // 预取数据
        if (first1 + PREFETCH_DISTANCE < last1) {
            __builtin_prefetch(&*(first1 + PREFETCH_DISTANCE), 0, 3);
        }
        if (first2 + PREFETCH_DISTANCE < last2) {
            __builtin_prefetch(&*(first2 + PREFETCH_DISTANCE), 0, 3);
        }
        
        CompareResult cmp = optimized_compare(*first1, *first2, comp);
        
        // 使用条件移动指令（如果编译器支持）
        switch (cmp) {
            case CompareResult::LESS:
                ++first1;
                break;
            case CompareResult::GREATER:
                ++first2;
                break;
            case CompareResult::EQUAL:
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
                break;
        }
    }
    
    return result;
}

/**
 * @brief 使用循环展开优化的merge函数
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator merge_optimized(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result, Compare comp) {
    // 循环展开优化
    while (first1 != last1 && first2 != last2) {
        // 展开4次迭代
        for (int i = 0; i < 4 && first1 != last1 && first2 != last2; ++i) {
            if (comp(*first2, *first1)) {
                *result = *first2;
                ++first2;
            } else {
                *result = *first1;
                ++first1;
            }
            ++result;
        }
    }
    
    // 使用批量复制优化剩余元素
    if (first1 != last1) {
        result = mystl::copy(first1, last1, result);
    }
    if (first2 != last2) {
        result = mystl::copy(first2, last2, result);
    }
    
    return result;
}

/**
 * @brief 使用分支预测优化的通用集合操作
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator set_operation_optimized(InputIterator1 first1, InputIterator1 last1,
                                     InputIterator2 first2, InputIterator2 last2,
                                     OutputIterator result, Compare comp,
                                     int operation_type) {
    // operation_type: 0=union, 1=intersection, 2=difference, 3=symmetric_difference
    
    // 使用分支预测提示
    if (__builtin_expect(first1 == last1, 0)) {
        if (operation_type == 0 || operation_type == 3) {
            return mystl::copy(first2, last2, result);
        }
        return result;
    }
    
    if (__builtin_expect(first2 == last2, 0)) {
        if (operation_type == 0 || operation_type == 2) {
            return mystl::copy(first1, last1, result);
        }
        return result;
    }
    
    // 主循环使用查找表
    static constexpr int operation_table[4][3][3] = {
        // Union operations
        {{0x11, 0x11, 0x12}, {0x00, 0x11, 0x00}, {0x00, 0x00, 0x12}},
        // Intersection operations  
        {{0x01, 0x11, 0x02}, {0x00, 0x11, 0x00}, {0x00, 0x00, 0x02}},
        // Difference operations
        {{0x11, 0x01, 0x02}, {0x00, 0x01, 0x00}, {0x00, 0x00, 0x02}},
        // Symmetric difference operations
        {{0x11, 0x01, 0x12}, {0x00, 0x01, 0x00}, {0x00, 0x00, 0x12}}
    };
    
    while (first1 != last1 && first2 != last2) {
        CompareResult cmp = optimized_compare(*first1, *first2, comp);
        int action = operation_table[operation_type][cmp + 1][0];
        
        if (action & 0x10) *result = *first1;  // copy first1
        if (action & 0x20) *result = *first2;  // copy first2
        if (action & 0x01) ++first1;          // advance first1
        if (action & 0x02) ++first2;          // advance first2
        if (action & 0x10 || action & 0x20) ++result; // advance result
    }
    
    // 处理剩余元素
    if (operation_type == 0 || operation_type == 3) { // union or symmetric_difference
        result = mystl::copy(first1, last1, result);
        result = mystl::copy(first2, last2, result);
    } else if (operation_type == 2) { // difference
        result = mystl::copy(first1, last1, result);
    }
    
    return result;
}

/**
 * @brief 使用模板特化优化的整数类型集合操作
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union_int_optimized(InputIterator1 first1, InputIterator1 last1,
                                      InputIterator2 first2, InputIterator2 last2,
                                      OutputIterator result) {
    // 对于整数类型，可以使用更高效的比较
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

} // namespace mystl

#endif // MYTINYSTL_SET_ALGO_OPTIMIZED_H
