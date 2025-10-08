#ifndef MYTINYSTL_FUNCTIONAL_H
#define MYTINYSTL_FUNCTIONAL_H

#include <cstddef>

namespace mystl {

// ============================================================================
// 函数对象基类
// ============================================================================

/**
 * @brief 一元函数对象基类
 */
template<typename Arg, typename Result>
struct unary_function {
    typedef Arg    argument_type;
    typedef Result result_type;
};

/**
 * @brief 二元函数对象基类
 */
template<typename Arg1, typename Arg2, typename Result>
struct binary_function {
    typedef Arg1   first_argument_type;
    typedef Arg2   second_argument_type;
    typedef Result result_type;
};

// ============================================================================
// 算术函数对象
// ============================================================================

/**
 * @brief 加法函数对象
 */
template<typename T>
struct plus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x + y;
    }
};

/**
 * @brief 减法函数对象
 */
template<typename T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x - y;
    }
};

/**
 * @brief 乘法函数对象
 */
template<typename T>
struct multiplies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x * y;
    }
};

/**
 * @brief 除法函数对象
 */
template<typename T>
struct divides : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x / y;
    }
};

/**
 * @brief 取模函数对象
 */
template<typename T>
struct modulus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x % y;
    }
};

/**
 * @brief 取反函数对象
 */
template<typename T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x) const {
        return -x;
    }
};

// ============================================================================
// 比较函数对象
// ============================================================================

/**
 * @brief 等于比较函数对象
 */
template<typename T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x == y;
    }
};

/**
 * @brief 不等于比较函数对象
 */
template<typename T>
struct not_equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x != y;
    }
};

/**
 * @brief 大于比较函数对象
 */
template<typename T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x > y;
    }
};

/**
 * @brief 小于比较函数对象
 */
template<typename T>
struct less : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x < y;
    }
};

/**
 * @brief 大于等于比较函数对象
 */
template<typename T>
struct greater_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x >= y;
    }
};

/**
 * @brief 小于等于比较函数对象
 */
template<typename T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x <= y;
    }
};

// ============================================================================
// 逻辑函数对象
// ============================================================================

/**
 * @brief 逻辑与函数对象
 */
template<typename T>
struct logical_and : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x && y;
    }
};

/**
 * @brief 逻辑或函数对象
 */
template<typename T>
struct logical_or : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x || y;
    }
};

/**
 * @brief 逻辑非函数对象
 */
template<typename T>
struct logical_not : public unary_function<T, bool> {
    bool operator()(const T& x) const {
        return !x;
    }
};

// ============================================================================
// 位运算函数对象
// ============================================================================

/**
 * @brief 位与函数对象
 */
template<typename T>
struct bit_and : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x & y;
    }
};

/**
 * @brief 位或函数对象
 */
template<typename T>
struct bit_or : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x | y;
    }
};

/**
 * @brief 位异或函数对象
 */
template<typename T>
struct bit_xor : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x ^ y;
    }
};

// ============================================================================
// 其他函数对象
// ============================================================================

/**
 * @brief 恒等函数对象
 */
template<typename T>
struct identity : public unary_function<T, T> {
    const T& operator()(const T& x) const {
        return x;
    }
};

/**
 * @brief 选择第一个参数
 */
template<typename Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
    const typename Pair::first_type& operator()(const Pair& x) const {
        return x.first;
    }
};

/**
 * @brief 选择第二个参数
 */
template<typename Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type& operator()(const Pair& x) const {
        return x.second;
    }
};

} // namespace mystl

#endif // MYTINYSTL_FUNCTIONAL_H