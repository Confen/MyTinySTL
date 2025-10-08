#ifndef MYTINYSTL_ALGORITHM_H
#define MYTINYSTL_ALGORITHM_H

/**
 * @file algorithm.h
 * @brief MyTinySTL 算法总入口
 * @author MyTinySTL
 * @date 2025-01-14
 * 
 * 本文件是 MyTinySTL 的算法总入口，整合了所有算法组件：
 * - 算法基础 (algobase.h)
 * - 基本算法 (algo.h)
 * - 堆算法 (heap_algo.h)
 * - 集合算法 (set_algo.h)
 * - 数值算法 (numeric.h)
 * 
 * 使用方法：
 * @code
 * #include "algorithm.h"
 * 
 * int main() {
 *     std::vector<int> vec{1, 2, 3, 4, 5};
 *     
 *     // 使用各种算法
 *     int sum = mystl::accumulate(vec.begin(), vec.end(), 0);
 *     mystl::sort(vec.begin(), vec.end());
 *     // ... 其他算法
 *     
 *     return 0;
 * }
 * @endcode
 */

// ============================================================================
// 包含所有算法头文件
// ============================================================================

// 算法基础 - 必须首先包含
#include "algobase.h"

// 基本算法（非修改和修改算法）
#include "algo.h"

// 堆算法（专门的堆算法实现）
#include "heap_algo.h"

// 集合算法（集合操作和合并算法）
#include "set_algo.h"

// 数值算法（数值计算算法）
#include "numeric.h"

// ============================================================================
// 命名空间说明
// ============================================================================

/**
 * @namespace mystl
 * @brief MyTinySTL 命名空间
 * 
 * 所有 MyTinySTL 的组件都在 mystl 命名空间中，包括：
 * - 容器 (vector, list, map, set 等)
 * - 算法 (sort, find, accumulate 等)
 * - 迭代器 (iterator, iterator_traits 等)
 * - 函数对象 (plus, minus, less 等)
 * - 类型萃取 (type_traits, enable_if 等)
 * 
 * 使用示例：
 * @code
 * mystl::vector<int> vec;
 * mystl::sort(vec.begin(), vec.end());
 * @endcode
 */

// ============================================================================
// 算法组件概览
// ============================================================================

/**
 * @defgroup algorithms 算法组件
 * @{
 * 
 * @defgroup algobase 算法基础
 * @brief 提供算法的基础工具和类型定义
 * @details 包含 iterator_traits, distance, advance 等基础工具
 * 
 * @defgroup algo 基本算法
 * @brief 提供基本的非修改和修改算法
 * @details 包含 find, count, sort, reverse 等基本算法
 * 
 * @defgroup heap_algo 堆算法
 * @brief 提供堆相关的算法
 * @details 包含 make_heap, push_heap, pop_heap, sort_heap 等堆算法
 * 
 * @defgroup set_algo 集合算法
 * @brief 提供集合相关的算法
 * @details 包含 set_union, set_intersection, set_difference 等集合算法
 * 
 * @defgroup numeric 数值算法
 * @brief 提供数值计算相关的算法
 * @details 包含 accumulate, inner_product, partial_sum 等数值算法
 * 
 * @}
 */

// ============================================================================
// 版本信息
// ============================================================================

/**
 * @brief MyTinySTL 算法模块版本
 * @details 当前版本：1.0.0
 * @date 2025-01-14
 * @author MyTinySTL Team
 */
#define MYTINYSTL_ALGORITHM_VERSION "1.0.0"

/**
 * @brief MyTINYSTL 算法模块版本号
 * @details 版本号格式：主版本.次版本.修订版本
 */
#define MYTINYSTL_ALGORITHM_VERSION_MAJOR 1
#define MYTINYSTL_ALGORITHM_VERSION_MINOR 0
#define MYTINYSTL_ALGORITHM_VERSION_PATCH 0

// ============================================================================
// 编译时检查
// ============================================================================

// 检查必要的头文件是否被正确包含
#ifndef MYTINYSTL_ALGOBASE_H
#error "algobase.h must be included before algorithm.h"
#endif

#ifndef MYTINYSTL_ALGO_H
#error "algo.h must be included before algorithm.h"
#endif

#ifndef MYTINYSTL_HEAP_ALGO_H
#error "heap_algo.h must be included before algorithm.h"
#endif

#ifndef MYTINYSTL_SET_ALGO_H
#error "set_algo.h must be included before algorithm.h"
#endif

#ifndef MYTINYSTL_NUMERIC_H
#error "numeric.h must be included before algorithm.h"
#endif

// ============================================================================
// 使用说明
// ============================================================================

/**
 * @page algorithm_usage 算法使用指南
 * 
 * @section quick_start 快速开始
 * 
 * 1. 包含头文件：
 * @code
 * #include "algorithm.h"
 * @endcode
 * 
 * 2. 使用算法：
 * @code
 * std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
 * 
 * // 排序
 * mystl::sort(vec.begin(), vec.end());
 * 
 * // 查找
 * auto it = mystl::find(vec.begin(), vec.end(), 5);
 * 
 * // 累加
 * int sum = mystl::accumulate(vec.begin(), vec.end(), 0);
 * @endcode
 * 
 * @section algorithm_categories 算法分类
 * 
 * ### 非修改算法
 * - find, find_if, find_if_not
 * - count, count_if
 * - all_of, any_of, none_of
 * - for_each
 * 
 * ### 修改算法
 * - sort, stable_sort
 * - reverse, reverse_copy
 * - unique, unique_copy
 * - remove, remove_if
 * - replace, replace_if
 * 
 * ### 堆算法
 * - make_heap, push_heap, pop_heap
 * - sort_heap
 * - is_heap, is_heap_until
 * 
 * ### 集合算法
 * - set_union, set_intersection
 * - set_difference, set_symmetric_difference
 * - includes, merge
 * 
 * ### 数值算法
 * - accumulate, inner_product
 * - partial_sum, adjacent_difference
 * - iota
 * 
 * @section performance_notes 性能说明
 * 
 * MyTinySTL 的算法实现经过优化，性能与标准库相当：
 * - 使用模板元编程优化编译时计算
 * - 针对不同数据类型进行特化
 * - 提供异常安全保证
 * - 支持移动语义
 * 
 * @section exception_safety 异常安全
 * 
 * 所有算法都提供以下异常安全保证：
 * - 基本异常保证：操作要么完全成功，要么对象处于有效状态
 * - 强异常保证：操作要么完全成功，要么对象状态不变
 * - 无异常保证：操作不抛出异常
 * 
 * @section examples 完整示例
 * 
 * @code
 * #include "algorithm.h"
 * #include <vector>
 * #include <iostream>
 * 
 * int main() {
 *     std::vector<int> data{3, 1, 4, 1, 5, 9, 2, 6};
 *     
 *     // 排序
 *     mystl::sort(data.begin(), data.end());
 *     
 *     // 去重
 *     auto new_end = mystl::unique(data.begin(), data.end());
 *     data.erase(new_end, data.end());
 *     
 *     // 计算和
 *     int sum = mystl::accumulate(data.begin(), data.end(), 0);
 *     
 *     std::cout << "排序去重后的数据: ";
 *     for (int x : data) {
 *         std::cout << x << " ";
 *     }
 *     std::cout << "\n总和: " << sum << std::endl;
 *     
 *     return 0;
 * }
 * @endcode
 */

#endif // MYTINYSTL_ALGORITHM_H
