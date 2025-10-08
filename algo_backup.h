#ifndef MYTINYSTL_ALGO_H
#define MYTINYSTL_ALGO_H

#include <cstddef>
#include <ctime>
#include <functional>

#include "algobase.h"
#include "memory.h"
#include "functional.h"

namespace mystl {

// ============================================================================
// 非修改性算法
// ============================================================================

/**
 * @brief 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param unary_pred 一元谓词
 * @return 如果所有元素都满足谓词返回 true，否则返回 false
 */
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (!unary_pred(*first))
            return false;
    }
    return true;
}

/**
 * @brief 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param unary_pred 一元谓词
 * @return 如果存在元素满足谓词返回 true，否则返回 false
 */
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*first))
            return true;
    }
    return false;
}

/**
 * @brief 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param unary_pred 一元谓词
 * @return 如果所有元素都不满足谓词返回 true，否则返回 false
 */
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*first))
            return false;
    }
    return true;
}

/**
 * @brief 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要比较的值
 * @return 等于 value 的元素个数
 */
template <class InputIter, class T>
typename iterator_traits<InputIter>::difference_type
count(InputIter first, InputIter last, const T& value) {
    typename iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value)
            ++n;
    }
    return n;
}

/**
 * @brief 对[first, last)区间内的每个元素执行 pred 操作，返回结果为 true 的个数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 谓词函数
 * @return 满足谓词的元素个数
 */
template <class InputIter, class UnaryPredicate>
typename iterator_traits<InputIter>::difference_type
count_if(InputIter first, InputIter last, UnaryPredicate pred) {
    typename iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred(*first))
            ++n;
    }
    return n;
}

/**
 * @brief 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @return 指向找到元素的迭代器，如果未找到返回 last
 */
template <class InputIter, class T>
InputIter find(InputIter first, InputIter last, const T& value) {
    while (first != last && *first != value)
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令 pred 为 true 的元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 谓词函数
 * @return 指向找到元素的迭代器，如果未找到返回 last
 */
template <class InputIter, class UnaryPredicate>
InputIter find_if(InputIter first, InputIter last, UnaryPredicate pred) {
    while (first != last && !pred(*first))
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令 pred 为 false 的元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 谓词函数
 * @return 指向找到元素的迭代器，如果未找到返回 last
 */
template <class InputIter, class UnaryPredicate>
InputIter find_if_not(InputIter first, InputIter last, UnaryPredicate pred) {
    while (first != last && pred(*first))
        ++first;
    return first;
}

/**
 * @brief 在[first1, last1)中查找[first2, last2)的首次出现点
 * @param first1 第一个序列的起始迭代器
 * @param last1 第一个序列的结束迭代器
 * @param first2 第二个序列的起始迭代器
 * @param last2 第二个序列的结束迭代器
 * @return 指向首次出现点的迭代器，如果未找到返回 last1
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2) {
    auto d1 = mystl::distance(first1, last1);
    auto d2 = mystl::distance(first2, last2);
    if (d1 < d2) {
        return last1;
    }
    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2) {
        if (current1 == last1) {
            return last1;
        }
        if (*current1 == *current2) {
            ++current1;
            ++current2;
        } else {
            if (d1 == d2) {
                return last1;
            } else {
                current1 = ++first1;
                current2 = first2;
                --d1;
            }
        }
    }
    return first1;
}

/**
 * @brief 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param count 连续元素的个数
 * @param value 要查找的值
 * @return 指向子序列起始处的迭代器，如果未找到返回 last
 */
template <class ForwardIter, class Size, class T>
ForwardIter search_n(ForwardIter first, ForwardIter last, Size count, const T& value) {
    if (count <= 0) {
        return first;
    } else {
        first = mystl::find(first, last, value);
        while (first != last) {
            auto m = count;
            auto next = first;
            ++next;
            while (m != 1 && next != last && *next == value) {
                ++next;
                --m;
            }
            if (m == 1) {
                return first;
            }
            if (next == last) {
                return last;
            }
            first = mystl::find(++next, last, value);
        }
        return last;
    }
}

/**
 * @brief 使用函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param f 函数对象
 * @return 函数对象 f
 */
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

/**
 * @brief 找出第一对匹配的相邻元素，缺省使用 operator== 比较
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @return 指向这对元素第一个的迭代器，如果未找到返回 last
 */
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
    if (first == last) {
        return last;
    }
    auto next = first;
    ++next;
    for (; next != last; ++first, ++next) {
        if (*first == *next)
            return first;
    }
    return last;
}

/**
 * @brief 找出第一对匹配的相邻元素，使用给定的二元函数 pred 比较
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 二元谓词
 * @return 指向这对元素第一个的迭代器，如果未找到返回 last
 */
template <class ForwardIter, class BinaryPredicate>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, BinaryPredicate pred) {
    if (first == last) {
        return last;
    }
    auto next = first;
    ++next;
    for (; next != last; ++first, ++next) {
        if (pred(*first, *next))
            return first;
    }
    return last;
}

// ============================================================================
// 二分查找算法
// ============================================================================

/**
 * @brief 在[first, last)中查找第一个不小于 value 的元素，返回指向该元素的迭代器
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @return 指向第一个不小于 value 的元素的迭代器
 */
template <class ForwardIter, class T>
ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& value) {
    return lower_bound(first, last, value, less<T>());
}

/**
 * @brief 在[first, last)中查找第一个不小于 value 的元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数
 * @return 指向第一个不小于 value 的元素的迭代器
 */
template <class ForwardIter, class T, class Compared>
ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
    auto len = distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, value)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

/**
 * @brief 在[first, last)中查找第一个大于 value 的元素，返回指向该元素的迭代器
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @return 指向第一个大于 value 的元素的迭代器
 */
template <class ForwardIter, class T>
ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& value) {
    return upper_bound(first, last, value, less<T>());
}

/**
 * @brief 在[first, last)中查找第一个大于 value 的元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数
 * @return 指向第一个大于 value 的元素的迭代器
 */
template <class ForwardIter, class T, class Compared>
ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
    auto len = distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(value, *middle)) {
            len = half;
        } else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

/**
 * @brief 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @return 如果找到返回 true，否则返回 false
 */
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value) {
    auto i = mystl::lower_bound(first, last, value);
    return i != last && !(value < *i);
}

/**
 * @brief 二分查找，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数
 * @return 如果找到返回 true，否则返回 false
 */
template <class ForwardIter, class T, class Compared>
bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
    auto i = mystl::lower_bound(first, last, value, comp);
    return i != last && !comp(value, *i);
}

// ============================================================================
// 生成算法
// ============================================================================

/**
 * @brief 将函数对象 gen 的结果赋值给[first, last)区间内的元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param gen 生成器函数
 */
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen) {
    for (; first != last; ++first) {
        *first = gen();
    }
}

/**
 * @brief 将函数对象 gen 的结果赋值给[first, first + n)区间内的元素
 * @param first 起始迭代器
 * @param n 元素个数
 * @param gen 生成器函数
 * @return 指向最后一个生成元素的下一个位置的迭代器
 */
template <class OutputIter, class Size, class Generator>
OutputIter generate_n(OutputIter first, Size n, Generator gen) {
    for (; n > 0; --n, ++first) {
        *first = gen();
    }
    return first;
}

// ============================================================================
// 排序相关算法
// ============================================================================

/**
 * @brief 检查[first, last)区间内的元素是否已经排序
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @return 如果已排序返回 true，否则返回 false
 */
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last) {
    return is_sorted(first, last, less<typename iterator_traits<ForwardIter>::value_type>());
}

/**
 * @brief 检查[first, last)区间内的元素是否已经排序，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 * @return 如果已排序返回 true，否则返回 false
 */
template <class ForwardIter, class Compared>
bool is_sorted(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last) {
        return true;
    }
    auto next = first;
    ++next;
    for (; next != last; ++first, ++next) {
        if (comp(*next, *first))
            return false;
    }
    return true;
}

/**
 * @brief 返回一个迭代器，指向序列中最大的元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @return 指向最大元素的迭代器
 */
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last) {
    if (first == last) {
        return last;
    }
    auto result = first;
    while (++first != last) {
        if (*result < *first) {
            result = first;
        }
    }
    return result;
}

/**
 * @brief 返回一个迭代器，指向序列中最大的元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 * @return 指向最大元素的迭代器
 */
template <class ForwardIter, class Compared>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last) {
        return last;
    }
    auto result = first;
    while (++first != last) {
        if (comp(*result, *first)) {
            result = first;
        }
    }
    return result;
}

/**
 * @brief 返回一个迭代器，指向序列中最小的元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @return 指向最小元素的迭代器
 */
template <class ForwardIter>
ForwardIter min_element(ForwardIter first, ForwardIter last) {
    if (first == last) {
        return last;
    }
    auto result = first;
    while (++first != last) {
        if (*first < *result) {
            result = first;
        }
    }
    return result;
}

/**
 * @brief 返回一个迭代器，指向序列中最小的元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 * @return 指向最小元素的迭代器
 */
template <class ForwardIter, class Compared>
ForwardIter min_element(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last) {
        return last;
    }
    auto result = first;
    while (++first != last) {
        if (comp(*first, *result)) {
            result = first;
        }
    }
    return result;
}

// ============================================================================
// 变换算法
// ============================================================================

/**
 * @brief 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param unary_op 一元操作函数
 * @return 指向输出序列最后一个元素的下一个位置的迭代器
 */
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter first, InputIter last, OutputIter result, UnaryOperation unary_op) {
    for (; first != last; ++first, ++result) {
        *result = unary_op(*first);
    }
    return result;
}

/**
 * @brief 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)和[first2, last2)的相同位置
 * @param first1 第一个输入序列的起始迭代器
 * @param last1 第一个输入序列的结束迭代器
 * @param first2 第二个输入序列的起始迭代器
 * @param result 输出序列的起始迭代器
 * @param binary_op 二元操作函数
 * @return 指向输出序列最后一个元素的下一个位置的迭代器
 */
template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
OutputIter transform(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                     OutputIter result, BinaryOperation binary_op) {
    for (; first1 != last1; ++first1, ++first2, ++result) {
        *result = binary_op(*first1, *first2);
    }
    return result;
}

// ============================================================================
// 反转算法
// ============================================================================

/**
 * @brief 将[first, last)区间内的元素反转
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class BidirectionalIter>
void reverse(BidirectionalIter first, BidirectionalIter last) {
    while (first != last && first != --last) {
        mystl::iter_swap(first++, last);
    }
}

/**
 * @brief 行为类似 reverse，但将结果复制到 result 所指的容器中
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return 指向输出序列最后一个元素的下一个位置的迭代器
 */
template <class BidirectionalIter, class OutputIter>
OutputIter reverse_copy(BidirectionalIter first, BidirectionalIter last, OutputIter result) {
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}

// ============================================================================
// 随机算法
// ============================================================================

/**
 * @brief 将[first, last)内的元素次序随机重排
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void random_shuffle(RandomIter first, RandomIter last) {
    if (first == last) {
        return;
    }
    srand((unsigned)time(0));
    for (auto i = first + 1; i != last; ++i) {
        mystl::iter_swap(i, first + (rand() % (i - first + 1)));
    }
}

/**
 * @brief 将[first, last)内的元素次序随机重排，使用给定的随机数生成器
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param rng 随机数生成器
 */
template <class RandomIter, class RandomNumberGenerator>
void random_shuffle(RandomIter first, RandomIter last, RandomNumberGenerator& rng) {
    if (first == last) {
        return;
    }
    for (auto i = first + 1; i != last; ++i) {
        mystl::iter_swap(i, first + rng(i - first + 1));
    }
}

// ============================================================================
// 修改性算法
// ============================================================================

// 注意：copy, copy_backward, move, move_backward, fill, fill_n, swap_ranges
// 等函数已在 algobase.h 中定义，这里不再重复定义

// ============================================================================
// 排序算法
// ============================================================================

/**
 * @brief 对[first, last)区间内的元素进行排序
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void sort(RandomIter first, RandomIter last) {
    sort(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 对[first, last)区间内的元素进行排序，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void sort(RandomIter first, RandomIter last, Compared comp) {
    if (first == last) {
        return;
    }
    
    // 简单的快速排序实现
    auto pivot = partition(first, last, comp);
    sort(first, pivot, comp);
    sort(pivot + 1, last, comp);
}

/**
 * @brief 对[first, last)区间内的元素进行稳定排序
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void stable_sort(RandomIter first, RandomIter last) {
    stable_sort(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 对[first, last)区间内的元素进行稳定排序，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void stable_sort(RandomIter first, RandomIter last, Compared comp) {
    if (first == last) {
        return;
    }
    
    // 简单的归并排序实现
    auto mid = first + (last - first) / 2;
    stable_sort(first, mid, comp);
    stable_sort(mid, last, comp);
    inplace_merge(first, mid, last, comp);
}

/**
 * @brief 对[first, last)区间内的元素进行部分排序
 * @param first 起始迭代器
 * @param middle 部分排序的结束位置
 * @param last 结束迭代器
 */
template <class RandomIter>
void partial_sort(RandomIter first, RandomIter middle, RandomIter last) {
    partial_sort(first, middle, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 对[first, last)区间内的元素进行部分排序，使用给定的比较函数
 * @param first 起始迭代器
 * @param middle 部分排序的结束位置
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void partial_sort(RandomIter first, RandomIter middle, RandomIter last, Compared comp) {
    if (first == middle || middle == last) {
        return;
    }
    
    // 简单的堆排序实现
    make_heap(first, last, comp);
    for (auto i = last; i != middle; --i) {
        mystl::iter_swap(first, i - 1);
        adjust_heap(first, middle - first, 0, comp);
    }
}

// ============================================================================
// 分区算法
// ============================================================================

/**
 * @brief 将[first, last)区间内的元素按照 pred 进行分区
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 谓词函数
 * @return 指向第一个不满足谓词的元素的迭代器
 */
template <class ForwardIter, class UnaryPredicate>
ForwardIter partition(ForwardIter first, ForwardIter last, UnaryPredicate pred) {
    if (first == last) {
        return first;
    }
    
    auto pivot = first;
    auto i = first;
    ++i;
    
    while (i != last) {
        if (pred(*i)) {
            ++pivot;
            mystl::iter_swap(pivot, i);
        }
        ++i;
    }
    
    mystl::iter_swap(first, pivot);
    return pivot + 1;
}

/**
 * @brief 检查[first, last)区间内的元素是否已经按照 pred 进行分区
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param pred 谓词函数
 * @return 如果已分区返回 true，否则返回 false
 */
template <class ForwardIter, class UnaryPredicate>
bool is_partitioned(ForwardIter first, ForwardIter last, UnaryPredicate pred) {
    for (; first != last; ++first) {
        if (!pred(*first)) {
            break;
        }
    }
    for (; first != last; ++first) {
        if (pred(*first)) {
            return false;
        }
    }
    return true;
}

// 堆算法已移至 heap_algo.h

/**
 * @brief 将[first, last)区间内的元素构建成堆
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void make_heap(RandomIter first, RandomIter last) {
    make_heap(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 将[first, last)区间内的元素构建成堆，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void make_heap(RandomIter first, RandomIter last, Compared comp) {
    if (first == last || first + 1 == last) {
        return;
    }
    
    auto len = last - first;
    for (auto i = len / 2 - 1; i >= 0; --i) {
        adjust_heap(first, len, i, comp);
    }
}

/**
 * @brief 调整堆
 * @param first 起始迭代器
 * @param len 堆的长度
 * @param hole 需要调整的位置
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void adjust_heap(RandomIter first, typename iterator_traits<RandomIter>::difference_type len,
                 typename iterator_traits<RandomIter>::difference_type hole, Compared comp) {
    auto value = *(first + hole);
    auto child = hole * 2 + 1;
    
    while (child < len) {
        if (child + 1 < len && comp(*(first + child), *(first + child + 1))) {
            ++child;
        }
        
        if (comp(value, *(first + child))) {
            *(first + hole) = *(first + child);
            hole = child;
            child = hole * 2 + 1;
        } else {
            break;
        }
    }
    
    *(first + hole) = value;
}

/**
 * @brief 向堆中插入元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void push_heap(RandomIter first, RandomIter last) {
    push_heap(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 向堆中插入元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void push_heap(RandomIter first, RandomIter last, Compared comp) {
    if (first == last || first + 1 == last) {
        return;
    }
    
    auto len = last - first;
    auto hole = len - 1;
    auto value = *(first + hole);
    
    while (hole > 0) {
        auto parent = (hole - 1) / 2;
        if (comp(*(first + parent), value)) {
            *(first + hole) = *(first + parent);
            hole = parent;
        } else {
            break;
        }
    }
    
    *(first + hole) = value;
}

/**
 * @brief 从堆中移除最大元素
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last) {
    pop_heap(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 从堆中移除最大元素，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void pop_heap(RandomIter first, RandomIter last, Compared comp) {
    if (first == last || first + 1 == last) {
        return;
    }
    
    mystl::iter_swap(first, last - 1);
    adjust_heap(first, last - first - 1, 0, comp);
}

/**
 * @brief 对堆进行排序
 * @param first 起始迭代器
 * @param last 结束迭代器
 */
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last) {
    sort_heap(first, last, less<typename iterator_traits<RandomIter>::value_type>());
}

/**
 * @brief 对堆进行排序，使用给定的比较函数
 * @param first 起始迭代器
 * @param last 结束迭代器
 * @param comp 比较函数
 */
template <class RandomIter, class Compared>
void sort_heap(RandomIter first, RandomIter last, Compared comp) {
    while (first != last) {
        pop_heap(first, last, comp);
        --last;
    }
}

// ============================================================================
// 合并算法
// ============================================================================

/**
 * @brief 合并两个已排序的序列
 * @param first1 第一个序列的起始迭代器
 * @param last1 第一个序列的结束迭代器
 * @param first2 第二个序列的起始迭代器
 * @param last2 第二个序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return 指向输出序列最后一个元素的下一个位置的迭代器
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result) {
    return merge(first1, last1, first2, last2, result, less<typename iterator_traits<InputIter1>::value_type>());
}

/**
 * @brief 合并两个已排序的序列，使用给定的比较函数
 * @param first1 第一个序列的起始迭代器
 * @param last1 第一个序列的结束迭代器
 * @param first2 第二个序列的起始迭代器
 * @param last2 第二个序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 比较函数
 * @return 指向输出序列最后一个元素的下一个位置的迭代器
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compared comp) {
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
    
    return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

/**
 * @brief 原地合并两个已排序的序列
 * @param first 第一个序列的起始迭代器
 * @param middle 第一个序列的结束迭代器，也是第二个序列的起始迭代器
 * @param last 第二个序列的结束迭代器
 */
template <class BidirectionalIter>
void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last) {
    inplace_merge(first, middle, last, less<typename iterator_traits<BidirectionalIter>::value_type>());
}

/**
 * @brief 原地合并两个已排序的序列，使用给定的比较函数
 * @param first 第一个序列的起始迭代器
 * @param middle 第一个序列的结束迭代器，也是第二个序列的起始迭代器
 * @param last 第二个序列的结束迭代器
 * @param comp 比较函数
 */
template <class BidirectionalIter, class Compared>
void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Compared comp) {
    if (first == middle || middle == last) {
        return;
    }
    
    // 简单的原地合并实现
    auto len1 = middle - first;
    auto len2 = last - middle;
    
    if (len1 < len2) {
        // 交换两个序列
        mystl::swap_ranges(first, middle, middle);
        inplace_merge(middle, middle + len1, last, comp);
    } else {
        // 递归合并
        auto mid1 = first + len1 / 2;
        auto mid2 = mystl::lower_bound(middle, last, *mid1, comp);
        auto mid3 = mid1 + (mid2 - middle);
        
        rotate(mid1, middle, mid2);
        inplace_merge(first, mid1, mid3, comp);
        inplace_merge(mid3, mid2, last, comp);
    }
}

// ============================================================================
// 旋转算法
// ============================================================================

/**
 * @brief 将[first, middle)和[middle, last)两个区间交换位置
 * @param first 起始迭代器
 * @param middle 中间迭代器
 * @param last 结束迭代器
 * @return 指向原来 middle 位置的迭代器
 */
template <class ForwardIter>
ForwardIter rotate(ForwardIter first, ForwardIter middle, ForwardIter last) {
    if (first == middle) {
        return last;
    }
    if (middle == last) {
        return first;
    }
    
    auto next = middle;
    do {
        mystl::iter_swap(first, next);
        ++first;
        ++next;
        if (first == middle) {
            middle = next;
        }
    } while (next != last);
    
    auto result = first;
    next = middle;
    while (next != last) {
        mystl::iter_swap(first, next);
        ++first;
        ++next;
        if (first == middle) {
            middle = next;
        } else if (next == last) {
            next = middle;
        }
    }
    
    return result;
}

} // namespace mystl

#endif // !MYTINYSTL_ALGO_H