#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_

#include <type_traits>
#include <utility>
#include <memory>
#include <iterator>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <cfloat>
#include "iterator.h"
namespace mystl {

// ============================================================================
// 基础类型定义
// ============================================================================

/**
 * @brief 整数常量包装器
 * @tparam T 值类型
 * @tparam v 常量值
 * 
 * 用于包装编译期常量，提供统一的接口访问常量值
 */
template<typename T, T v>
struct m_integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = m_integral_constant;
    
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

/**
 * @brief 布尔常量类型别名
 * @tparam B 布尔值
 */
template<bool B>
using m_bool_constant = m_integral_constant<bool, B>;

/**
 * @brief 真类型
 * 表示编译期真值的类型
 */
using m_true_type = m_bool_constant<true>;

/**
 * @brief 假类型
 * 表示编译期假值的类型
 */
using m_false_type = m_bool_constant<false>;

// ============================================================================
// 类型萃取实现
// ============================================================================

/**
 * @brief 判断类型是否为 pair
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_pair : m_false_type {};

template<typename T1, typename T2>
struct is_pair<std::pair<T1, T2> > : m_true_type {};

// ============================================================================
// 使用标准库的 type_traits 作为基础
// ============================================================================

// 基础类型特征
using std::is_void;
using std::is_null_pointer;
using std::is_integral;
using std::is_floating_point;
using std::is_arithmetic;
using std::is_fundamental;
using std::is_compound;
using std::is_reference;
using std::is_pointer;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_const;
using std::is_volatile;
using std::is_array;
using std::is_function;
using std::is_class;
using std::is_union;
using std::is_enum;
using std::is_pod;
using std::is_trivial;
using std::is_standard_layout;
using std::is_empty;
using std::is_polymorphic;
using std::is_abstract;
// is_final 在 C++11 中不可用，跳过
using std::is_signed;
using std::is_unsigned;
using std::is_same;
using std::is_base_of;
using std::is_convertible;
using std::is_constructible;
using std::is_default_constructible;
using std::is_copy_constructible;
using std::is_move_constructible;
using std::is_assignable;
using std::is_copy_assignable;
using std::is_move_assignable;
using std::is_destructible;
using std::is_trivially_constructible;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
using std::is_trivially_assignable;
using std::is_trivially_copy_assignable;
using std::is_trivially_move_assignable;
using std::is_trivially_destructible;
using std::is_nothrow_constructible;
using std::is_nothrow_copy_constructible;
using std::is_nothrow_move_constructible;
using std::is_nothrow_assignable;
using std::is_nothrow_copy_assignable;
using std::is_nothrow_move_assignable;
using std::is_nothrow_destructible;
using std::has_virtual_destructor;
using std::alignment_of;
using std::rank;
using std::extent;
using std::remove_const;
using std::remove_volatile;
using std::remove_cv;
using std::add_const;
// 移除引用修饰符
template<typename T>
struct remove_reference {
    typedef T type;
};

template<typename T>
struct remove_reference<T&> {
    typedef T type;
};

template<typename T>
struct remove_reference<T&&> {
    typedef T type;
};

// 添加左值引用
template<typename T>
struct add_lvalue_reference {
    typedef T& type;
};

template<typename T>
struct add_lvalue_reference<T&> {
    typedef T& type;
};

template<typename T>
struct add_lvalue_reference<T&&> {
    typedef T& type;
};

// 添加右值引用
template<typename T>
struct add_rvalue_reference {
    typedef T&& type;
};

template<typename T>
struct add_rvalue_reference<T&> {
    typedef T&& type;
};

template<typename T>
struct add_rvalue_reference<T&&> {
    typedef T&& type;
};

// 检查是否为左值引用
template<typename T>
struct is_lvalue_reference_mystl : m_false_type {};

template<typename T>
struct is_lvalue_reference_mystl<T&> : m_true_type {};

// 检查是否为右值引用
template<typename T>
struct is_rvalue_reference_mystl : m_false_type {};

template<typename T>
struct is_rvalue_reference_mystl<T&&> : m_true_type {};

// 数值限制
template<typename T>
struct numeric_limits {
    static constexpr bool is_specialized = false;
    static constexpr T min() noexcept { return T(); }
    static constexpr T max() noexcept { return T(); }
};

// 特化版本
template<>
struct numeric_limits<int> {
    static constexpr bool is_specialized = true;
    static constexpr int min() noexcept { return -2147483648; }
    static constexpr int max() noexcept { return 2147483647; }
};

template<>
struct numeric_limits<unsigned int> {
    static constexpr bool is_specialized = true;
    static constexpr unsigned int min() noexcept { return 0; }
    static constexpr unsigned int max() noexcept { return 4294967295U; }
};

template<>
struct numeric_limits<long> {
    static constexpr bool is_specialized = true;
    static constexpr long min() noexcept { return LONG_MIN; }
    static constexpr long max() noexcept { return LONG_MAX; }
};

template<>
struct numeric_limits<unsigned long> {
    static constexpr bool is_specialized = true;
    static constexpr unsigned long min() noexcept { return 0; }
    static constexpr unsigned long max() noexcept { return ULONG_MAX; }
};

template<>
struct numeric_limits<long long> {
    static constexpr bool is_specialized = true;
    static constexpr long long min() noexcept { return LLONG_MIN; }
    static constexpr long long max() noexcept { return LLONG_MAX; }
};

template<>
struct numeric_limits<unsigned long long> {
    static constexpr bool is_specialized = true;
    static constexpr unsigned long long min() noexcept { return 0; }
    static constexpr unsigned long long max() noexcept { return ULLONG_MAX; }
};

template<>
struct numeric_limits<float> {
    static constexpr bool is_specialized = true;
    static constexpr float min() noexcept { return 1.175494e-38f; }
    static constexpr float max() noexcept { return 3.402823e+38f; }
};

template<>
struct numeric_limits<double> {
    static constexpr bool is_specialized = true;
    static constexpr double min() noexcept { return 2.225074e-308; }
    static constexpr double max() noexcept { return 1.797693e+308; }
};

template<>
struct numeric_limits<long double> {
    static constexpr bool is_specialized = true;
    static constexpr long double min() noexcept { return 3.362103e-4932L; }
    static constexpr long double max() noexcept { return 1.189731e+4932L; }
};

using std::add_volatile;
using std::remove_all_extents;
using std::aligned_storage;
using std::aligned_union;
using std::decay;
using std::enable_if;
using std::conditional;
using std::common_type;
using std::underlying_type;
using std::result_of;
using std::integral_constant;
// bool_constant 在 C++11 中不可用，跳过
using std::true_type;
using std::false_type;

// ============================================================================
// 自定义类型萃取实现
// ============================================================================

// 左值引用和右值引用使用标准库实现

// 右值引用使用标准库实现

/**
 * @brief 判断类型是否为函数指针
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_function_pointer : m_false_type {};

template<typename R, typename... Args>
struct is_function_pointer<R(*)(Args...)> : m_true_type {};

// noexcept 版本在 C++11 中可能不支持，暂时注释

/**
 * @brief 判断类型是否为成员函数指针
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_member_function_pointer : m_false_type {};

template<typename R, typename C>
struct is_member_function_pointer<R(C::*)()> : m_true_type {};

template<typename R, typename C, typename... Args>
struct is_member_function_pointer<R(C::*)(Args...)> : m_true_type {};

// noexcept 版本在 C++11 中可能不支持，暂时注释

/**
 * @brief 判断类型是否为成员对象指针
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_member_object_pointer : m_false_type {};

template<typename R, typename C>
struct is_member_object_pointer<R(C::*)> : m_true_type {};

/**
 * @brief 判断类型是否为成员指针
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_member_pointer : m_false_type {};

template<typename T>
struct is_member_pointer<T*> : is_member_function_pointer<T> {};

template<typename T, typename C>
struct is_member_pointer<T C::*> : m_true_type {};

/**
 * @brief 判断类型是否为可调用对象
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_callable_impl {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>()(), m_true_type{});
    
    template<typename>
    static m_false_type test(...);
    
    using type = decltype(test<T>(0));
};

template<typename T>
struct is_callable : is_callable_impl<T>::type {};

/**
 * @brief 判断类型是否为函数对象
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_function_object : m_bool_constant<
    !std::is_function<T>::value &&
    !std::is_reference<T>::value &&
    !std::is_void<T>::value &&
    is_callable<T>::value
> {};

/**
 * @brief 判断类型是否为智能指针
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_smart_pointer : m_false_type {};

template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : m_true_type {};

template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : m_true_type {};

template<typename T>
struct is_smart_pointer<std::weak_ptr<T>> : m_true_type {};

/**
 * @brief 判断类型是否为容器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_container_impl {
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::declval<U>().size(),
        m_true_type{}
    );
    
    template<typename>
    static m_false_type test(...);
    
    using type = decltype(test<T>(0));
};

template<typename T>
struct is_container : is_container_impl<T>::type {};

/**
 * @brief 判断类型是否为迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_iterator_impl {
    template<typename U>
    static auto test(int) -> decltype(
        ++std::declval<U&>(),
        *std::declval<U>(),
        m_true_type{}
    );
    
    template<typename>
    static m_false_type test(...);
    
    using type = decltype(test<T>(0));
};

template<typename T>
struct is_iterator : is_iterator_impl<T>::type {};

/**
 * @brief 判断类型是否为输入迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_input_iterator : m_bool_constant<
    is_iterator<T>::value &&
    std::is_convertible<
        decltype(*std::declval<T>()),
        typename mystl::iterator_traits<T>::value_type
    >::value
> {};

/**
 * @brief 判断类型是否为输出迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_output_iterator : m_bool_constant<
    is_iterator<T>::value &&
    std::is_assignable<
        decltype(*std::declval<T>()),
        typename mystl::iterator_traits<T>::value_type
    >::value
> {};

/**
 * @brief 判断类型是否为前向迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_forward_iterator : m_bool_constant<
    is_input_iterator<T>::value &&
    std::is_default_constructible<T>::value
> {};

/**
 * @brief 判断类型是否为双向迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_bidirectional_iterator : m_bool_constant<
    is_forward_iterator<T>::value &&
    std::is_convertible<
        decltype(--std::declval<T&>()),
        T&
    >::value
> {};

/**
 * @brief 判断类型是否为随机访问迭代器
 * @tparam T 待检查的类型
 */
template<typename T>
struct is_random_access_iterator : m_bool_constant<
    is_bidirectional_iterator<T>::value &&
    std::is_convertible<
        decltype(std::declval<T>() + 1),
        T
    >::value &&
    std::is_convertible<
        decltype(std::declval<T>() - 1),
        T
    >::value &&
    std::is_convertible<
        decltype(std::declval<T>()[0]),
        typename mystl::iterator_traits<T>::reference
    >::value
> {};
} // namespace mystl

#endif // MYTINYSTL_TYPE_TRAITS_H_
