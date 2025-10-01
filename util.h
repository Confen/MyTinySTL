/**
 * @file util.h
 * @brief MyTinySTL 工具函数系统
 * @author MyTinySTL
 * @date 2024
 */

#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>
#include "type_traits.h"
#include "exceptdef.h"

namespace mystl {

// ============================================================================
// pair 模板类
// ============================================================================

/**
 * @brief pair 模板类
 * @tparam T1 第一个元素的类型
 * @tparam T2 第二个元素的类型
 */
template<typename T1, typename T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    
    T1 first;
    T2 second;
    
    // 默认构造函数
    pair() : first(), second() {}
    
    // 拷贝构造函数
    pair(const pair&) = default;
    
    // 移动构造函数
    pair(pair&&) = default;
    
    // 参数构造函数
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    
    // 模板构造函数
    template<typename U1, typename U2>
    pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
    
    // 模板移动构造函数
    template<typename U1, typename U2>
    pair(pair<U1, U2>&& p) : first(std::move(p.first)), second(std::move(p.second)) {}
    
    // 拷贝赋值操作符
    pair& operator=(const pair&) = default;
    
    // 移动赋值操作符
    pair& operator=(pair&&) = default;
    
    // 模板赋值操作符
    template<typename U1, typename U2>
    pair& operator=(const pair<U1, U2>& p) {
        first = p.first;
        second = p.second;
        return *this;
    }
    
    // 模板移动赋值操作符
    template<typename U1, typename U2>
    pair& operator=(pair<U1, U2>&& p) {
        first = std::move(p.first);
        second = std::move(p.second);
        return *this;
    }
    
    // swap 函数
    void swap(pair& p) noexcept {
        using std::swap;
        swap(first, p.first);
        swap(second, p.second);
    }
};

// ============================================================================
// pair 比较操作符
// ============================================================================

template<typename T1, typename T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template<typename T1, typename T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs == rhs);
}

template<typename T1, typename T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template<typename T1, typename T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(rhs < lhs);
}

template<typename T1, typename T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return rhs < lhs;
}

template<typename T1, typename T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs < rhs);
}

// ============================================================================
// make_pair 函数
// ============================================================================

/**
 * @brief 创建 pair 对象
 * @tparam T1 第一个元素的类型
 * @tparam T2 第二个元素的类型
 * @param t1 第一个元素
 * @param t2 第二个元素
 * @return pair<T1, T2> 对象
 */
template<typename T1, typename T2>
pair<typename std::decay<T1>::type, typename std::decay<T2>::type>
make_pair(T1&& t1, T2&& t2) {
    return pair<typename std::decay<T1>::type, typename std::decay<T2>::type>(
        std::forward<T1>(t1), std::forward<T2>(t2));
}

// ============================================================================
// 移动语义函数
// ============================================================================

/**
 * @brief 移动语义函数
 * @tparam T 类型
 * @param t 要移动的对象
 * @return T&& 右值引用
 */
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

/**
 * @brief 完美转发函数
 * @tparam T 类型
 * @param t 要转发的对象
 * @return T&& 转发引用
 */
template<typename T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, 
                  "template argument substituting T is an lvalue reference type");
    return static_cast<T&&>(t);
}

// ============================================================================
// swap 函数
// ============================================================================

/**
 * @brief 交换两个对象
 * @tparam T 类型
 * @param a 第一个对象
 * @param b 第二个对象
 */
template<typename T>
void swap(T& a, T& b) noexcept {
    T temp = mystl::move(a);
    a = mystl::move(b);
    b = mystl::move(temp);
}

/**
 * @brief 交换两个数组
 * @tparam T 类型
 * @tparam N 数组大小
 * @param a 第一个数组
 * @param b 第二个数组
 */
template<typename T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))) {
    for (size_t i = 0; i < N; ++i) {
        swap(a[i], b[i]);
    }
}

// ============================================================================
// 比较函数
// ============================================================================

/**
 * @brief 返回两个值中的较大值
 * @tparam T 类型
 * @param a 第一个值
 * @param b 第二个值
 * @return const T& 较大值的引用
 */
template<typename T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/**
 * @brief 使用比较器返回两个值中的较大值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param a 第一个值
 * @param b 第二个值
 * @param comp 比较器
 * @return const T& 较大值的引用
 */
template<typename T, typename Compare>
const T& max(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

/**
 * @brief 返回两个值中的较小值
 * @tparam T 类型
 * @param a 第一个值
 * @param b 第二个值
 * @return const T& 较小值的引用
 */
template<typename T>
const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

/**
 * @brief 使用比较器返回两个值中的较小值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param a 第一个值
 * @param b 第二个值
 * @param comp 比较器
 * @return const T& 较小值的引用
 */
template<typename T, typename Compare>
const T& min(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? b : a;
}

/**
 * @brief 返回两个值中的较小值和较大值
 * @tparam T 类型
 * @param a 第一个值
 * @param b 第二个值
 * @return pair<const T&, const T&> 包含较小值和较大值的 pair
 */
template<typename T>
pair<const T&, const T&> minmax(const T& a, const T& b) {
    return (b < a) ? pair<const T&, const T&>(b, a) : pair<const T&, const T&>(a, b);
}

/**
 * @brief 使用比较器返回两个值中的较小值和较大值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param a 第一个值
 * @param b 第二个值
 * @param comp 比较器
 * @return pair<const T&, const T&> 包含较小值和较大值的 pair
 */
template<typename T, typename Compare>
pair<const T&, const T&> minmax(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? pair<const T&, const T&>(b, a) : pair<const T&, const T&>(a, b);
}

// ============================================================================
// 初始化列表支持
// ============================================================================

/**
 * @brief 返回初始化列表中的最大值
 * @tparam T 类型
 * @param ilist 初始化列表
 * @return T 最大值
 */
template<typename T>
T max(std::initializer_list<T> ilist) {
    return *std::max_element(ilist.begin(), ilist.end());
}

/**
 * @brief 使用比较器返回初始化列表中的最大值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param ilist 初始化列表
 * @param comp 比较器
 * @return T 最大值
 */
template<typename T, typename Compare>
T max(std::initializer_list<T> ilist, Compare comp) {
    return *std::max_element(ilist.begin(), ilist.end(), comp);
}

/**
 * @brief 返回初始化列表中的最小值
 * @tparam T 类型
 * @param ilist 初始化列表
 * @return T 最小值
 */
template<typename T>
T min(std::initializer_list<T> ilist) {
    return *std::min_element(ilist.begin(), ilist.end());
}

/**
 * @brief 使用比较器返回初始化列表中的最小值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param ilist 初始化列表
 * @param comp 比较器
 * @return T 最小值
 */
template<typename T, typename Compare>
T min(std::initializer_list<T> ilist, Compare comp) {
    return *std::min_element(ilist.begin(), ilist.end(), comp);
}

/**
 * @brief 返回初始化列表中的最小值和最大值
 * @tparam T 类型
 * @param ilist 初始化列表
 * @return pair<T, T> 包含最小值和最大值的 pair
 */
template<typename T>
pair<T, T> minmax(std::initializer_list<T> ilist) {
    auto result = std::minmax_element(ilist.begin(), ilist.end());
    return pair<T, T>(*result.first, *result.second);
}

/**
 * @brief 使用比较器返回初始化列表中的最小值和最大值
 * @tparam T 类型
 * @tparam Compare 比较器类型
 * @param ilist 初始化列表
 * @param comp 比较器
 * @return pair<T, T> 包含最小值和最大值的 pair
 */
template<typename T, typename Compare>
pair<T, T> minmax(std::initializer_list<T> ilist, Compare comp) {
    auto result = std::minmax_element(ilist.begin(), ilist.end(), comp);
    return pair<T, T>(*result.first, *result.second);
}

// ============================================================================
// 其他工具函数
// ============================================================================

// swap 函数已在上面定义，这里不再重复

} // namespace mystl

#endif // MYTINYSTL_UTIL_H_
