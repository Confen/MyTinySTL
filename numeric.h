#ifndef MYTINYSTL_NUMERIC_H
#define MYTINYSTL_NUMERIC_H

#include "functional.h"
#include "iterator.h"

namespace mystl {

// ============================================================================
// 累加算法 (accumulate)
// ============================================================================

/**
 * @brief 计算范围内元素的累加和
 * @tparam InputIterator 输入迭代器类型
 * @tparam T 累加值类型
 * @param first 范围的开始迭代器
 * @param last 范围的结束迭代器
 * @param init 初始值
 * @return T 累加结果
 */
template<typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init) {
    T result = init;
    for(; first != last; ++first) {
        result += *first;
    }
    return result;
}

/**
 * @brief 使用自定义二元操作计算范围内元素的累加
 * @tparam InputIterator 输入迭代器类型
 * @tparam T 累加值类型
 * @tparam BinaryOperation 二元操作类型
 * @param first 范围的开始迭代器
 * @param last 范围的结束迭代器
 * @param init 初始值
 * @param binary_op 二元操作函数对象
 * @return T 累加结果
 */
template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
    T result = init;
    for(; first != last; ++first) {
        result = binary_op(result, *first);
    }
    return result;
}

// ============================================================================
// 内积算法 (inner_product)
// ============================================================================

/**
 * @brief 计算两个范围的内积
 * @tparam InputIterator1 第一个范围的迭代器类型
 * @tparam InputIterator2 第二个范围的迭代器类型
 * @tparam T 结果类型
 * @param first1 第一个范围的开始迭代器
 * @param last1 第一个范围的结束迭代器
 * @param first2 第二个范围的开始迭代器
 * @param init 初始值
 * @return T 内积结果
 */
template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    T result = init;
    for(; first1 != last1; ++first1, ++first2) {
        result = result + (*first1) * (*first2);
    }
    return result;
}

/**
 * @brief 使用自定义二元操作计算两个范围的内积
 * @tparam InputIterator1 第一个范围的迭代器类型
 * @tparam InputIterator2 第二个范围的迭代器类型
 * @tparam T 结果类型
 * @tparam BinaryOperation1 第一个二元操作类型
 * @tparam BinaryOperation2 第二个二元操作类型
 * @param first1 第一个范围的开始迭代器
 * @param last1 第一个范围的结束迭代器
 * @param first2 第二个范围的开始迭代器
 * @param init 初始值
 * @param binary_op1 第一个二元操作函数对象（累加操作）
 * @param binary_op2 第二个二元操作函数对象（乘法操作）
 * @return T 内积结果
 */
template<typename InputIterator1, typename InputIterator2, typename T, 
         typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init,
                BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
    T result = init;
    for(; first1 != last1; ++first1, ++first2) {
        result = binary_op1(result, binary_op2(*first1, *first2));
    }
    return result;
}

// ============================================================================
// 部分和算法 (partial_sum)
// ============================================================================

/**
 * @brief 计算范围内元素的部分和
 * @tparam InputIterator 输入迭代器类型
 * @tparam OutputIterator 输出迭代器类型
 * @param first 输入范围的开始迭代器
 * @param last 输入范围的结束迭代器
 * @param result 输出范围的开始迭代器
 * @return OutputIterator 输出范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last) {
        return result;
    }
    
    typename mystl::iterator_traits<InputIterator>::value_type sum = *first;
    *result = sum;
    ++result;
    ++first;
    
    for (; first != last; ++first, ++result) {
        sum = sum + *first;
        *result = sum;
    }
    
    return result;
}

/**
 * @brief 使用自定义二元操作计算范围内元素的部分和
 * @tparam InputIterator 输入迭代器类型
 * @tparam OutputIterator 输出迭代器类型
 * @tparam BinaryOperation 二元操作类型
 * @param first 输入范围的开始迭代器
 * @param last 输入范围的结束迭代器
 * @param result 输出范围的开始迭代器
 * @param binary_op 二元操作函数对象
 * @return OutputIterator 输出范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last) {
        return result;
    }
    
    typename mystl::iterator_traits<InputIterator>::value_type sum = *first;
    *result = sum;
    ++result;
    ++first;
    
    for (; first != last; ++first, ++result) {
        sum = binary_op(sum, *first);
        *result = sum;
    }
    
    return result;
}

// ============================================================================
// 相邻差分算法 (adjacent_difference)
// ============================================================================

/**
 * @brief 计算范围内相邻元素的差分
 * @tparam InputIterator 输入迭代器类型
 * @tparam OutputIterator 输出迭代器类型
 * @param first 输入范围的开始迭代器
 * @param last 输入范围的结束迭代器
 * @param result 输出范围的开始迭代器
 * @return OutputIterator 输出范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last) {
        return result;
    }
    
    typename mystl::iterator_traits<InputIterator>::value_type prev = *first;
    *result = prev;
    ++result;
    ++first;
    
    for (; first != last; ++first, ++result) {
        typename mystl::iterator_traits<InputIterator>::value_type curr = *first;
        *result = curr - prev;
        prev = curr;
    }
    
    return result;
}

/**
 * @brief 使用自定义二元操作计算范围内相邻元素的差分
 * @tparam InputIterator 输入迭代器类型
 * @tparam OutputIterator 输出迭代器类型
 * @tparam BinaryOperation 二元操作类型
 * @param first 输入范围的开始迭代器
 * @param last 输入范围的结束迭代器
 * @param result 输出范围的开始迭代器
 * @param binary_op 二元操作函数对象
 * @return OutputIterator 输出范围的结束迭代器
 */
template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last) {
        return result;
    }
    
    typename mystl::iterator_traits<InputIterator>::value_type prev = *first;
    *result = prev;
    ++result;
    ++first;
    
    for (; first != last; ++first, ++result) {
        typename mystl::iterator_traits<InputIterator>::value_type curr = *first;
        *result = binary_op(curr, prev);
        prev = curr;
    }
    
    return result;
}

// ============================================================================
// 序列填充算法 (iota)
// ============================================================================

/**
 * @brief 用递增序列填充范围
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 值类型
 * @param first 范围的开始迭代器
 * @param last 范围的结束迭代器
 * @param value 起始值
 */
template<typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
    for (; first != last; ++first, ++value) {
        *first = value;
    }
}

} // namespace mystl

#endif // MYTINYSTL_NUMERIC_H