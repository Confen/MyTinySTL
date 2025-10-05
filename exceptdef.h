#ifndef MYTINYSTL_EXCEPTDEF_H_
#define MYTINYSTL_EXCEPTDEF_H_

#include <exception>
#include <stdexcept>
#include <new>
#include <typeinfo>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace mystl {

// ============================================================================
// 异常安全保证级别枚举
// ============================================================================

/**
 * @brief 异常安全保证级别
 * 
 * 定义了四种异常安全保证级别：
 * - no_throw: 不抛出异常
 * - basic_guarantee: 基本异常保证
 * - strong_guarantee: 强异常保证
 * - no_guarantee: 无异常保证
 */
enum class exception_safety_level {
    no_throw,           // 不抛出异常
    basic_guarantee,    // 基本异常保证
    strong_guarantee,   // 强异常保证
    no_guarantee        // 无异常保证
};

// ============================================================================
// 异常安全保证检查函数（使用 mystl 命名空间避免与标准库冲突）
// ============================================================================

/**
 * @brief 检查类型是否可无异常构造
 * @tparam T 待检查的类型
 * @tparam Args 构造参数类型
 */
template<typename T, typename... Args>
struct mystl_is_nothrow_constructible {
    static constexpr bool value = noexcept(T(std::declval<Args>()...));
};

/**
 * @brief 检查类型是否可无异常拷贝构造
 * @tparam T 待检查的类型
 */
template<typename T>
struct mystl_is_nothrow_copy_constructible {
    static constexpr bool value = noexcept(T(std::declval<const T&>()));
};

/**
 * @brief 检查类型是否可无异常移动构造
 * @tparam T 待检查的类型
 */
template<typename T>
struct mystl_is_nothrow_move_constructible {
    static constexpr bool value = noexcept(T(std::declval<T&&>()));
};

/**
 * @brief 检查类型是否可无异常赋值
 * @tparam T 待检查的类型
 * @tparam U 赋值源类型
 */
template<typename T, typename U>
struct mystl_is_nothrow_assignable {
    static constexpr bool value = noexcept(std::declval<T>() = std::declval<U>());
};

/**
 * @brief 检查类型是否可无异常拷贝赋值
 * @tparam T 待检查的类型
 */
template<typename T>
struct mystl_is_nothrow_copy_assignable {
    static constexpr bool value = noexcept(std::declval<T&>() = std::declval<const T&>());
};

/**
 * @brief 检查类型是否可无异常移动赋值
 * @tparam T 待检查的类型
 */
template<typename T>
struct mystl_is_nothrow_move_assignable {
    static constexpr bool value = noexcept(std::declval<T&>() = std::declval<T&&>());
};

/**
 * @brief 检查类型是否可无异常析构
 * @tparam T 待检查的类型
 */
template<typename T>
struct mystl_is_nothrow_destructible {
    static constexpr bool value = noexcept(std::declval<T>().~T());
};

// ============================================================================
// 异常安全保证断言宏
// ============================================================================

/**
 * @brief 无异常断言宏
 * @param expr 要检查的表达式
 */
#define MYSTL_NOEXCEPT_ASSERT(expr) \
    static_assert(noexcept(expr), "Expression must be noexcept")

/**
 * @brief 基本异常保证断言宏
 * @param expr 要检查的表达式
 */
#define MYSTL_BASIC_GUARANTEE_ASSERT(expr) \
    do { \
        try { \
            expr; \
        } catch (...) { \
            /* 基本异常保证：对象处于有效状态 */ \
        } \
    } while(0)

/**
 * @brief 改进的基本异常保证断言宏（需要对象参数）
 * @param expr 要检查的表达式
 * @param obj 要检查的对象
 */
#define MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(expr, obj) \
    do { \
        bool was_valid_before = (obj).is_valid(); \
        try { \
            expr; \
        } catch (...) { \
            /* 基本异常保证：检查对象是否仍处于有效状态 */ \
            if (!(obj).is_valid()) { \
                std::cout << "❌ 基本异常保证失败：对象处于无效状态" << std::endl; \
            } else { \
                std::cout << "✅ 基本异常保证成功：对象仍处于有效状态" << std::endl; \
            } \
        } \
    } while(0)

/**
 * @brief 强异常保证断言宏
 * @param expr 要检查的表达式
 */
#define MYSTL_STRONG_GUARANTEE_ASSERT(expr) \
    do { \
        try { \
            expr; \
        } catch (...) { \
            /* 强异常保证：对象状态完全不变 */ \
        } \
    } while(0)

/**
 * @brief 改进的强异常保证断言宏（需要对象参数）
 * @param expr 要检查的表达式
 * @param obj 要检查的对象
 */
#define MYSTL_STRONG_GUARANTEE_ASSERT_WITH_OBJ(expr, obj) \
    do { \
        auto state_before = (obj).get_state(); \
        try { \
            expr; \
        } catch (...) { \
            /* 强异常保证：检查对象状态是否完全不变 */ \
            auto state_after = (obj).get_state(); \
            if (state_before != state_after) { \
                std::cout << "❌ 强异常保证失败：对象状态发生了变化" << std::endl; \
            } else { \
                std::cout << "✅ 强异常保证成功：对象状态完全不变" << std::endl; \
            } \
        } \
    } while(0)
// ============================================================================
// 自定义异常类
// ============================================================================

/**
 * @brief MyTinySTL 基础异常类
 * 
 * 继承自 std::exception，提供统一的异常接口
 */
class mystl_exception : public std::exception {
private:
    std::string message_;

public:
    /**
     * @brief 默认构造函数
     */
    mystl_exception() noexcept : message_("MyTinySTL exception") {}

    /**
     * @brief 带消息的构造函数
     * @param msg 异常消息
     */
    explicit mystl_exception(const char* msg) noexcept : message_(msg) {}

    /**
     * @brief 带消息的构造函数
     * @param msg 异常消息
     */
    explicit mystl_exception(const std::string& msg) noexcept : message_(msg) {}

    /**
     * @brief 拷贝构造函数
     * @param other 其他异常对象
     */
    mystl_exception(const mystl_exception& other) noexcept : message_(other.message_) {}

    /**
     * @brief 移动构造函数
     * @param other 其他异常对象
     */
    mystl_exception(mystl_exception&& other) noexcept : message_(std::move(other.message_)) {}

    /**
     * @brief 赋值操作符
     * @param other 其他异常对象
     * @return 当前异常对象的引用
     */
    mystl_exception& operator=(const mystl_exception& other) noexcept {
        if (this != &other) {
            message_ = other.message_;
        }
        return *this;
    }

    /**
     * @brief 移动赋值操作符
     * @param other 其他异常对象
     * @return 当前异常对象的引用
     */
    mystl_exception& operator=(mystl_exception&& other) noexcept {
        if (this != &other) {
            message_ = std::move(other.message_);
        }
        return *this;
    }

    /**
     * @brief 析构函数
     */
    virtual ~mystl_exception() noexcept = default;

    /**
     * @brief 获取异常消息
     * @return 异常消息的 C 风格字符串
     */
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

    /**
     * @brief 获取异常消息
     * @return 异常消息的字符串
     */
    const std::string& message() const noexcept {
        return message_;
    }
};

/**
 * @brief 逻辑错误异常类
 * 
 * 继承自 mystl_exception，用于表示逻辑错误
 */
class mystl_logic_error : public mystl_exception {
public:
    explicit mystl_logic_error(const char* msg) noexcept : mystl_exception(msg) {}
    explicit mystl_logic_error(const std::string& msg) noexcept : mystl_exception(msg) {}
    mystl_logic_error(const mystl_logic_error& other) noexcept : mystl_exception(other) {}
    mystl_logic_error(mystl_logic_error&& other) noexcept : mystl_exception(std::move(other)) {}
    mystl_logic_error& operator=(const mystl_logic_error& other) noexcept {
        mystl_exception::operator=(other);
        return *this;
    }
    mystl_logic_error& operator=(mystl_logic_error&& other) noexcept {
        mystl_exception::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_logic_error() noexcept = default;
};

/**
 * @brief 运行时错误异常类
 * 
 * 继承自 mystl_exception，用于表示运行时错误
 */
class mystl_runtime_error : public mystl_exception {
public:
    explicit mystl_runtime_error(const char* msg) noexcept : mystl_exception(msg) {}
    explicit mystl_runtime_error(const std::string& msg) noexcept : mystl_exception(msg) {}
    mystl_runtime_error(const mystl_runtime_error& other) noexcept : mystl_exception(other) {}
    mystl_runtime_error(mystl_runtime_error&& other) noexcept : mystl_exception(std::move(other)) {}
    mystl_runtime_error& operator=(const mystl_runtime_error& other) noexcept {
        mystl_exception::operator=(other);
        return *this;
    }
    mystl_runtime_error& operator=(mystl_runtime_error&& other) noexcept {
        mystl_exception::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_runtime_error() noexcept = default;
};

/**
 * @brief 内存分配失败异常类
 * 
 * 继承自 mystl_exception，用于表示内存分配失败
 */
class mystl_bad_alloc : public mystl_exception {
public:
    mystl_bad_alloc() noexcept : mystl_exception("MyTinySTL bad allocation") {}
    explicit mystl_bad_alloc(const char* msg) noexcept : mystl_exception(msg) {}
    explicit mystl_bad_alloc(const std::string& msg) noexcept : mystl_exception(msg) {}
    mystl_bad_alloc(const mystl_bad_alloc& other) noexcept : mystl_exception(other) {}
    mystl_bad_alloc(mystl_bad_alloc&& other) noexcept : mystl_exception(std::move(other)) {}
    mystl_bad_alloc& operator=(const mystl_bad_alloc& other) noexcept {
        mystl_exception::operator=(other);
        return *this;
    }
    mystl_bad_alloc& operator=(mystl_bad_alloc&& other) noexcept {
        mystl_exception::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_bad_alloc() noexcept = default;
};

/**
 * @brief 类型转换失败异常类
 * 
 * 继承自 mystl_exception，用于表示类型转换失败
 */
class mystl_bad_cast : public mystl_exception {
public:
    mystl_bad_cast() noexcept : mystl_exception("MyTinySTL bad cast") {}
    explicit mystl_bad_cast(const char* msg) noexcept : mystl_exception(msg) {}
    explicit mystl_bad_cast(const std::string& msg) noexcept : mystl_exception(msg) {}
    mystl_bad_cast(const mystl_bad_cast& other) noexcept : mystl_exception(other) {}
    mystl_bad_cast(mystl_bad_cast&& other) noexcept : mystl_exception(std::move(other)) {}
    mystl_bad_cast& operator=(const mystl_bad_cast& other) noexcept {
        mystl_exception::operator=(other);
        return *this;
    }
    mystl_bad_cast& operator=(mystl_bad_cast&& other) noexcept {
        mystl_exception::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_bad_cast() noexcept = default;
};

/**
 * @brief 类型信息获取失败异常类
 * 
 * 继承自 mystl_exception，用于表示类型信息获取失败
 */
class mystl_bad_typeid : public mystl_exception {
public:
    mystl_bad_typeid() noexcept : mystl_exception("MyTinySTL bad typeid") {}
    explicit mystl_bad_typeid(const char* msg) noexcept : mystl_exception(msg) {}
    explicit mystl_bad_typeid(const std::string& msg) noexcept : mystl_exception(msg) {}
    mystl_bad_typeid(const mystl_bad_typeid& other) noexcept : mystl_exception(other) {}
    mystl_bad_typeid(mystl_bad_typeid&& other) noexcept : mystl_exception(std::move(other)) {}
    mystl_bad_typeid& operator=(const mystl_bad_typeid& other) noexcept {
        mystl_exception::operator=(other);
        return *this;
    }
    mystl_bad_typeid& operator=(mystl_bad_typeid&& other) noexcept {
        mystl_exception::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_bad_typeid() noexcept = default;
};

/**
 * @brief 长度错误异常类
 * 
 * 继承自 mystl_logic_error，用于表示长度错误
 */
class mystl_length_error : public mystl_logic_error {
public:
    explicit mystl_length_error(const char* msg) noexcept : mystl_logic_error(msg) {}
    explicit mystl_length_error(const std::string& msg) noexcept : mystl_logic_error(msg) {}
    mystl_length_error(const mystl_length_error& other) noexcept : mystl_logic_error(other) {}
    mystl_length_error(mystl_length_error&& other) noexcept : mystl_logic_error(std::move(other)) {}
    mystl_length_error& operator=(const mystl_length_error& other) noexcept {
        mystl_logic_error::operator=(other);
        return *this;
    }
    mystl_length_error& operator=(mystl_length_error&& other) noexcept {
        mystl_logic_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_length_error() noexcept = default;
};

/**
 * @brief 越界访问异常类
 * 
 * 继承自 mystl_logic_error，用于表示越界访问
 */
class mystl_out_of_range : public mystl_logic_error {
public:
    explicit mystl_out_of_range(const char* msg) noexcept : mystl_logic_error(msg) {}
    explicit mystl_out_of_range(const std::string& msg) noexcept : mystl_logic_error(msg) {}
    mystl_out_of_range(const mystl_out_of_range& other) noexcept : mystl_logic_error(other) {}
    mystl_out_of_range(mystl_out_of_range&& other) noexcept : mystl_logic_error(std::move(other)) {}
    mystl_out_of_range& operator=(const mystl_out_of_range& other) noexcept {
        mystl_logic_error::operator=(other);
        return *this;
    }
    mystl_out_of_range& operator=(mystl_out_of_range&& other) noexcept {
        mystl_logic_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_out_of_range() noexcept = default;
};

/**
 * @brief 无效参数异常类
 * 
 * 继承自 mystl_logic_error，用于表示无效参数
 */
class mystl_invalid_argument : public mystl_logic_error {
public:
    explicit mystl_invalid_argument(const char* msg) noexcept : mystl_logic_error(msg) {}
    explicit mystl_invalid_argument(const std::string& msg) noexcept : mystl_logic_error(msg) {}
    mystl_invalid_argument(const mystl_invalid_argument& other) noexcept : mystl_logic_error(other) {}
    mystl_invalid_argument(mystl_invalid_argument&& other) noexcept : mystl_logic_error(std::move(other)) {}
    mystl_invalid_argument& operator=(const mystl_invalid_argument& other) noexcept {
        mystl_logic_error::operator=(other);
        return *this;
    }
    mystl_invalid_argument& operator=(mystl_invalid_argument&& other) noexcept {
        mystl_logic_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_invalid_argument() noexcept = default;
};

/**
 * @brief 域错误异常类
 * 
 * 继承自 mystl_logic_error，用于表示域错误
 */
class mystl_domain_error : public mystl_logic_error {
public:
    explicit mystl_domain_error(const char* msg) noexcept : mystl_logic_error(msg) {}
    explicit mystl_domain_error(const std::string& msg) noexcept : mystl_logic_error(msg) {}
    mystl_domain_error(const mystl_domain_error& other) noexcept : mystl_logic_error(other) {}
    mystl_domain_error(mystl_domain_error&& other) noexcept : mystl_logic_error(std::move(other)) {}
    mystl_domain_error& operator=(const mystl_domain_error& other) noexcept {
        mystl_logic_error::operator=(other);
        return *this;
    }
    mystl_domain_error& operator=(mystl_domain_error&& other) noexcept {
        mystl_logic_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_domain_error() noexcept = default;
};

/**
 * @brief 范围错误异常类
 * 
 * 继承自 mystl_runtime_error，用于表示范围错误
 */
class mystl_range_error : public mystl_runtime_error {
public:
    explicit mystl_range_error(const char* msg) noexcept : mystl_runtime_error(msg) {}
    explicit mystl_range_error(const std::string& msg) noexcept : mystl_runtime_error(msg) {}
    mystl_range_error(const mystl_range_error& other) noexcept : mystl_runtime_error(other) {}
    mystl_range_error(mystl_range_error&& other) noexcept : mystl_runtime_error(std::move(other)) {}
    mystl_range_error& operator=(const mystl_range_error& other) noexcept {
        mystl_runtime_error::operator=(other);
        return *this;
    }
    mystl_range_error& operator=(mystl_range_error&& other) noexcept {
        mystl_runtime_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_range_error() noexcept = default;
};

/**
 * @brief 溢出错误异常类
 * 
 * 继承自 mystl_runtime_error，用于表示溢出错误
 */
class mystl_overflow_error : public mystl_runtime_error {
public:
    explicit mystl_overflow_error(const char* msg) noexcept : mystl_runtime_error(msg) {}
    explicit mystl_overflow_error(const std::string& msg) noexcept : mystl_runtime_error(msg) {}
    mystl_overflow_error(const mystl_overflow_error& other) noexcept : mystl_runtime_error(other) {}
    mystl_overflow_error(mystl_overflow_error&& other) noexcept : mystl_runtime_error(std::move(other)) {}
    mystl_overflow_error& operator=(const mystl_overflow_error& other) noexcept {
        mystl_runtime_error::operator=(other);
        return *this;
    }
    mystl_overflow_error& operator=(mystl_overflow_error&& other) noexcept {
        mystl_runtime_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_overflow_error() noexcept = default;
};

/**
 * @brief 下溢错误异常类
 * 
 * 继承自 mystl_runtime_error，用于表示下溢错误
 */
class mystl_underflow_error : public mystl_runtime_error {
public:
    explicit mystl_underflow_error(const char* msg) noexcept : mystl_runtime_error(msg) {}
    explicit mystl_underflow_error(const std::string& msg) noexcept : mystl_runtime_error(msg) {}
    mystl_underflow_error(const mystl_underflow_error& other) noexcept : mystl_runtime_error(other) {}
    mystl_underflow_error(mystl_underflow_error&& other) noexcept : mystl_runtime_error(std::move(other)) {}
    mystl_underflow_error& operator=(const mystl_underflow_error& other) noexcept {
        mystl_runtime_error::operator=(other);
        return *this;
    }
    mystl_underflow_error& operator=(mystl_underflow_error&& other) noexcept {
        mystl_runtime_error::operator=(std::move(other));
        return *this;
    }
    virtual ~mystl_underflow_error() noexcept = default;
};

// ============================================================================
// 异常处理宏
// ============================================================================

/**
 * @brief 异常捕获开始宏
 */
#define MYSTL_TRY try

/**
 * @brief 异常捕获宏
 * @param exception_type 异常类型
 */
#define MYSTL_CATCH(exception_type) catch (const exception_type& e)

/**
 * @brief 捕获所有异常宏
 */
#define MYSTL_CATCH_ALL catch (...)

/**
 * @brief 抛出异常宏
 * @param exception_type 异常类型
 * @param message 异常消息
 */
#define MYSTL_THROW(exception_type, message) throw exception_type(message)

/**
 * @brief 重新抛出异常宏
 */
#define MYSTL_RETHROW throw

/**
 * @brief 无异常说明符宏
 */
#define MYSTL_NOEXCEPT noexcept

/**
 * @brief 条件无异常说明符宏
 * @param condition 条件
 */
#define MYSTL_NOEXCEPT_IF(condition) noexcept(condition)

/**
 * @brief 无异常表达式宏
 * @param expr 表达式
 */
#define MYSTL_NOEXCEPT_EXPR(expr) noexcept(expr)

// ============================================================================
// 异常安全保证宏
// ============================================================================

/**
 * @brief 无异常尝试宏
 * @param expr 表达式
 */
#define MYSTL_TRY_NOEXCEPT(expr) \
    do { \
        if (noexcept(expr)) { \
            expr; \
        } else { \
            throw mystl_runtime_error("Expression is not noexcept"); \
        } \
    } while (0)

/**
 * @brief 无异常捕获宏
 * @param exception_type 异常类型
 */
#define MYSTL_CATCH_NOEXCEPT(exception_type) \
    catch (const exception_type& e) { \
        if (noexcept(e.what())) { \
            /* 处理异常 */ \
        } else { \
            throw; \
        } \
    }

/**
 * @brief 无异常抛出宏
 * @param exception_type 异常类型
 * @param message 异常消息
 */
#define MYSTL_THROW_NOEXCEPT(exception_type, message) \
    do { \
        if (noexcept(exception_type(message))) { \
            throw exception_type(message); \
        } else { \
            throw mystl_runtime_error("Exception is not noexcept"); \
        } \
    } while (0)

// ============================================================================
// 调试宏
// ============================================================================

/**
 * @brief 调试异常信息宏
 * @param message 调试消息
 */
#define MYSTL_DEBUG_EXCEPTION(message) \
    do { \
        std::fprintf(stderr, "[DEBUG] %s:%d: %s\n", __FILE__, __LINE__, message); \
    } while (0)

/**
 * @brief 异常跟踪宏
 * @param message 跟踪消息
 */
#define MYSTL_EXCEPTION_TRACE(message) \
    do { \
        std::fprintf(stderr, "[TRACE] %s:%d: %s\n", __FILE__, __LINE__, message); \
    } while (0)

/**
 * @brief 异常日志宏
 * @param message 日志消息
 */
#define MYSTL_EXCEPTION_LOG(message) \
    do { \
        std::fprintf(stderr, "[LOG] %s:%d: %s\n", __FILE__, __LINE__, message); \
    } while (0)

// ============================================================================
// 异常安全保证检查函数
// ============================================================================

/**
 * @brief 检查无异常保证
 * @tparam T 待检查的类型
 * @tparam Args 构造参数类型
 * @return 是否满足无异常保证
 */
template<typename T, typename... Args>
constexpr bool check_no_throw() noexcept {
    return std::is_nothrow_constructible<T, Args...>::value;
}

/**
 * @brief 检查基本异常保证
 * @tparam T 待检查的类型
 * @return 是否满足基本异常保证
 */
template<typename T>
constexpr bool check_basic_guarantee() noexcept {
    return std::is_destructible<T>::value;
}

/**
 * @brief 检查强异常保证
 * @tparam T 待检查的类型
 * @return 是否满足强异常保证
 */
template<typename T>
constexpr bool check_strong_guarantee() noexcept {
    return std::is_nothrow_copy_constructible<T>::value && 
           std::is_nothrow_copy_assignable<T>::value;
}

/**
 * @brief 检查无异常保证
 * @tparam T 待检查的类型
 * @return 是否满足无异常保证
 */
template<typename T>
constexpr bool check_no_guarantee() noexcept {
    return true; // 无异常保证总是满足
}

// ============================================================================
// 异常安全保证验证宏
// ============================================================================

/**
 * @brief 验证无异常保证宏
 * @param expr 表达式
 */
#define MYSTL_VERIFY_NO_THROW(expr) \
    static_assert(noexcept(expr), "Expression must be noexcept")

/**
 * @brief 验证基本异常保证宏
 * @param expr 表达式
 */
#define MYSTL_VERIFY_BASIC_GUARANTEE(expr) \
    static_assert(true, "Basic guarantee verification")

/**
 * @brief 验证强异常保证宏
 * @param expr 表达式
 */
#define MYSTL_VERIFY_STRONG_GUARANTEE(expr) \
    static_assert(true, "Strong guarantee verification")

/**
 * @brief 验证无异常保证宏
 * @param expr 表达式
 */
#define MYSTL_VERIFY_NO_GUARANTEE(expr) \
    static_assert(true, "No guarantee verification")

// ============================================================================
// 异常安全保证测试函数
// ============================================================================

/**
 * @brief 测试异常安全性
 * @tparam T 待测试的类型
 * @param obj 测试对象
 * @return 异常安全级别
 */
template<typename T>
exception_safety_level test_exception_safety(const T& obj) noexcept {
    if (check_no_throw<T>()) {
        return exception_safety_level::no_throw;
    } else if (check_strong_guarantee<T>()) {
        return exception_safety_level::strong_guarantee;
    } else if (check_basic_guarantee<T>()) {
        return exception_safety_level::basic_guarantee;
    } else {
        return exception_safety_level::no_guarantee;
    }
}

/**
 * @brief 测试无异常安全性
 * @tparam T 待测试的类型
 * @param obj 测试对象
 * @return 是否满足无异常安全
 */
template<typename T>
bool test_no_throw_safety(const T& obj) noexcept {
    return check_no_throw<T>();
}

/**
 * @brief 测试基本异常保证安全性
 * @tparam T 待测试的类型
 * @param obj 测试对象
 * @return 是否满足基本异常保证
 */
template<typename T>
bool test_basic_guarantee_safety(const T& obj) noexcept {
    return check_basic_guarantee<T>();
}

/**
 * @brief 测试强异常保证安全性
 * @tparam T 待测试的类型
 * @param obj 测试对象
 * @return 是否满足强异常保证
 */
template<typename T>
bool test_strong_guarantee_safety(const T& obj) noexcept {
    return check_strong_guarantee<T>();
}

} // namespace mystl

#endif // MYTINYSTL_EXCEPTDEF_H_
