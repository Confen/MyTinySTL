#ifndef MYTINYSTL_HEAP_ALGO_H
#define MYTINYSTL_HEAP_ALGO_H

#include <cstddef>
#include <algorithm>
#include <functional>

#include "algobase.h"
#include "functional.h"
#include "iterator.h"

namespace mystl {

// ============================================================================
// 堆算法
// ============================================================================

/**
 * @brief 堆排序相关算法
 * 堆是一个完全二叉树，满足父节点的值总是大于（或小于）子节点的值
 */

/**
 * @brief 调整堆，使指定位置满足堆性质
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 * @param comp 比较函数对象
 * @param hole_index 需要调整的位置
 * @param len 堆的长度
 * @param value 要插入的值
 */
template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void push_heap_aux(RandomAccessIterator first, Distance hole_index, Distance top_index, T value, Compare comp) {
    Distance parent = (hole_index - 1) / 2;
    while (hole_index > top_index && comp(*(first + parent), value)) {
        *(first + hole_index) = *(first + parent);
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

/**
 * @brief 调整堆，使指定位置满足堆性质（默认比较器）
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 * @param comp 比较函数对象
 */
template<typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return;
    
    auto len = last - first;
    auto value = *(last - 1);
    push_heap_aux(first, static_cast<typename mystl::iterator_traits<RandomAccessIterator>::difference_type>(len - 1), 
                  static_cast<typename mystl::iterator_traits<RandomAccessIterator>::difference_type>(0), value, comp);
}

/**
 * @brief 调整堆，使指定位置满足堆性质（使用operator<）
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 */
template<typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::push_heap(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

/**
 * @brief 调整堆，移除堆顶元素后重新调整堆
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 * @param comp 比较函数对象
 */
template<typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return;
    
    auto len = last - first;
    mystl::swap(*first, *(last - 1));
    
    // 调整堆
    auto hole_index = 0;
    auto left_child = 2 * hole_index + 1;
    auto right_child = left_child + 1;
    
    while (left_child < len - 1) {
        auto max_child = (right_child < len - 1 && comp(*(first + left_child), *(first + right_child))) 
                        ? right_child : left_child;
        
        if (comp(*(first + max_child), *(first + hole_index))) {
            break;
        }
        
        mystl::swap(*(first + hole_index), *(first + max_child));
        hole_index = max_child;
        left_child = 2 * hole_index + 1;
        right_child = left_child + 1;
    }
}

/**
 * @brief 调整堆，移除堆顶元素后重新调整堆（使用operator<）
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 */
template<typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::pop_heap(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

/**
 * @brief 构建堆
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 * @param comp 比较函数对象
 */
template<typename RandomAccessIterator, typename Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return;
    
    auto len = last - first;
    for (auto i = len / 2 - 1; i >= 0; --i) {
        // 调整以i为根的子树
        auto hole_index = i;
        auto left_child = 2 * hole_index + 1;
        auto right_child = left_child + 1;
        
        while (left_child < len) {
            auto max_child = (right_child < len && comp(*(first + left_child), *(first + right_child))) 
                            ? right_child : left_child;
            
            if (comp(*(first + max_child), *(first + hole_index))) {
                break;
            }
            
            mystl::swap(*(first + hole_index), *(first + max_child));
            hole_index = max_child;
            left_child = 2 * hole_index + 1;
            right_child = left_child + 1;
        }
    }
}

/**
 * @brief 构建堆（使用operator<）
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 */
template<typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::make_heap(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

/**
 * @brief 堆排序
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 * @param comp 比较函数对象
 */
template<typename RandomAccessIterator, typename Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    while (last - first > 1) {
        mystl::pop_heap(first, last, comp);
        --last;
    }
}

/**
 * @brief 堆排序（使用operator<）
 * @param first 堆的起始迭代器
 * @param last 堆的结束迭代器
 */
template<typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::sort_heap(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

/**
 * @brief 检查范围是否为堆
 * @param first 范围的起始迭代器
 * @param last 范围的结束迭代器
 * @param comp 比较函数对象
 * @return 如果是堆返回true，否则返回false
 */
template<typename RandomAccessIterator, typename Compare>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return true;
    
    auto len = last - first;
    for (auto i = 0; i < len / 2; ++i) {
        auto left_child = 2 * i + 1;
        auto right_child = left_child + 1;
        
        if (left_child < len && comp(*(first + i), *(first + left_child))) {
            return false;
        }
        if (right_child < len && comp(*(first + i), *(first + right_child))) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 检查范围是否为堆（使用operator<）
 * @param first 范围的起始迭代器
 * @param last 范围的结束迭代器
 * @return 如果是堆返回true，否则返回false
 */
template<typename RandomAccessIterator>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
    return mystl::is_heap(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

/**
 * @brief 查找范围中第一个不满足堆性质的位置
 * @param first 范围的起始迭代器
 * @param last 范围的结束迭代器
 * @param comp 比较函数对象
 * @return 第一个不满足堆性质的位置
 */
template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return last;
    
    auto len = last - first;
    for (auto i = 0; i < len / 2; ++i) {
        auto left_child = 2 * i + 1;
        auto right_child = left_child + 1;
        
        if (left_child < len && comp(*(first + i), *(first + left_child))) {
            return first + left_child;
        }
        if (right_child < len && comp(*(first + i), *(first + right_child))) {
            return first + right_child;
        }
    }
    return last;
}

/**
 * @brief 查找范围中第一个不满足堆性质的位置（使用operator<）
 * @param first 范围的起始迭代器
 * @param last 范围的结束迭代器
 * @return 第一个不满足堆性质的位置
 */
template<typename RandomAccessIterator>
RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last) {
    return mystl::is_heap_until(first, last, mystl::less<typename mystl::iterator_traits<RandomAccessIterator>::value_type>());
}

} // namespace mystl

#endif // MYTINYSTL_HEAP_ALGO_H
