#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <windows.h>

// 独立的函数对象实现，避免头文件冲突
namespace mystl {

// 基础函数对象类
template<typename Arg, typename Result>
struct unary_function {
    typedef Arg    argument_type;
    typedef Result result_type;
};

template<typename Arg1, typename Arg2, typename Result>
struct binary_function {
    typedef Arg1   first_argument_type;
    typedef Arg2   second_argument_type;
    typedef Result result_type;
};

// 算术函数对象
template<typename T>
struct plus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x + y;
    }
};

template<typename T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x - y;
    }
};

template<typename T>
struct multiplies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x * y;
    }
};

template<typename T>
struct divides : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x / y;
    }
};

template<typename T>
struct modulus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x % y;
    }
};

template<typename T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x) const {
        return -x;
    }
};

// 比较函数对象
template<typename T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x == y;
    }
};

template<typename T>
struct not_equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x != y;
    }
};

template<typename T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x > y;
    }
};

template<typename T>
struct less : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x < y;
    }
};

template<typename T>
struct greater_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x >= y;
    }
};

template<typename T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x <= y;
    }
};

// 逻辑函数对象
template<typename T>
struct logical_and : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x && y;
    }
};

template<typename T>
struct logical_or : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x || y;
    }
};

template<typename T>
struct logical_not : public unary_function<T, bool> {
    bool operator()(const T& x) const {
        return !x;
    }
};

// 位运算函数对象
template<typename T>
struct bit_and : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x & y;
    }
};

template<typename T>
struct bit_or : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x | y;
    }
};

template<typename T>
struct bit_xor : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x ^ y;
    }
};

// 谓词函数对象
template<typename T>
struct is_positive : public unary_function<T, bool> {
    bool operator()(const T& x) const {
        return x > T(0);
    }
};

template<typename T>
struct is_negative : public unary_function<T, bool> {
    bool operator()(const T& x) const {
        return x < T(0);
    }
};

template<typename T>
struct is_zero : public unary_function<T, bool> {
    bool operator()(const T& x) const {
        return x == T(0);
    }
};

// 工具函数对象
template<typename T>
struct identity : public unary_function<T, T> {
    const T& operator()(const T& x) const {
        return x;
    }
};

// 函数适配器
template<typename Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
private:
    Predicate pred_;

public:
    explicit unary_negate(const Predicate& pred) : pred_(pred) {}
    
    bool operator()(const typename Predicate::argument_type& x) const {
        return !pred_(x);
    }
};

template<typename Predicate>
class binary_negate : public binary_function<
    typename Predicate::first_argument_type,
    typename Predicate::second_argument_type,
    bool> {
private:
    Predicate pred_;

public:
    explicit binary_negate(const Predicate& pred) : pred_(pred) {}
    
    bool operator()(const typename Predicate::first_argument_type& x,
                   const typename Predicate::second_argument_type& y) const {
        return !pred_(x, y);
    }
};

// 取反适配器函数
template<typename Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {
    return unary_negate<Predicate>(pred);
}

template<typename Predicate>
inline binary_negate<Predicate> not2(const Predicate& pred) {
    return binary_negate<Predicate>(pred);
}

// 绑定器
template<typename Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
                                       typename Operation::result_type> {
private:
    Operation op_;
    typename Operation::first_argument_type value_;

public:
    binder1st(const Operation& op, const typename Operation::first_argument_type& value)
        : op_(op), value_(value) {}
    
    typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
        return op_(value_, x);
    }
};

template<typename Operation>
class binder2nd : public unary_function<typename Operation::first_argument_type,
                                       typename Operation::result_type> {
private:
    Operation op_;
    typename Operation::second_argument_type value_;

public:
    binder2nd(const Operation& op, const typename Operation::second_argument_type& value)
        : op_(op), value_(value) {}
    
    typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
        return op_(x, value_);
    }
};

// 绑定器函数
template<typename Operation, typename T>
inline binder1st<Operation> bind1st(const Operation& op, const T& value) {
    return binder1st<Operation>(op, value);
}

template<typename Operation, typename T>
inline binder2nd<Operation> bind2nd(const Operation& op, const T& value) {
    return binder2nd<Operation>(op, value);
}

} // namespace mystl

// 测试用的类
class TestClass {
private:
    int value_;
    std::string name_;

public:
    TestClass(int v, const std::string& n) : value_(v), name_(n) {}
    
    int getValue() const { return value_; }
    void setValue(int v) { value_ = v; }
    
    std::string getName() const { return name_; }
    void setName(const std::string& n) { name_ = n; }
    
    bool isPositive() const { return value_ > 0; }
    bool isEven() const { return value_ % 2 == 0; }
    
    void print() const {
        std::cout << "TestClass(" << value_ << ", \"" << name_ << "\")";
    }
};

// 测试函数
void test_arithmetic_functors() {
    std::cout << "=== 算术函数对象测试 ===" << std::endl;
    
    // 一元算术函数
    mystl::negate<int> neg;
    std::cout << "negate(5) = " << neg(5) << std::endl;
    
    // 二元算术函数
    mystl::plus<int> add;
    mystl::minus<int> sub;
    mystl::multiplies<int> mul;
    mystl::divides<int> div;
    mystl::modulus<int> mod;
    
    int a = 10, b = 3;
    std::cout << a << " + " << b << " = " << add(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << sub(a, b) << std::endl;
    std::cout << a << " * " << b << " = " << mul(a, b) << std::endl;
    std::cout << a << " / " << b << " = " << div(a, b) << std::endl;
    std::cout << a << " % " << b << " = " << mod(a, b) << std::endl;
}

void test_comparison_functors() {
    std::cout << "\n=== 比较函数对象测试 ===" << std::endl;
    
    mystl::equal_to<int> eq;
    mystl::not_equal_to<int> ne;
    mystl::greater<int> gt;
    mystl::less<int> lt;
    mystl::greater_equal<int> ge;
    mystl::less_equal<int> le;
    
    int a = 5, b = 3;
    std::cout << a << " == " << b << " ? " << (eq(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " != " << b << " ? " << (ne(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " > " << b << " ? " << (gt(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " < " << b << " ? " << (lt(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " >= " << b << " ? " << (ge(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " <= " << b << " ? " << (le(a, b) ? "true" : "false") << std::endl;
}

void test_logical_functors() {
    std::cout << "\n=== 逻辑函数对象测试 ===" << std::endl;
    
    mystl::logical_and<bool> and_op;
    mystl::logical_or<bool> or_op;
    mystl::logical_not<bool> not_op;
    
    bool x = true, y = false;
    std::cout << x << " && " << y << " = " << (and_op(x, y) ? "true" : "false") << std::endl;
    std::cout << x << " || " << y << " = " << (or_op(x, y) ? "true" : "false") << std::endl;
    std::cout << "!" << x << " = " << (not_op(x) ? "true" : "false") << std::endl;
}

void test_bitwise_functors() {
    std::cout << "\n=== 位运算函数对象测试 ===" << std::endl;
    
    mystl::bit_and<int> and_op;
    mystl::bit_or<int> or_op;
    mystl::bit_xor<int> xor_op;
    
    int a = 0b1010, b = 0b1100;
    std::cout << "a = " << a << " (0b1010)" << std::endl;
    std::cout << "b = " << b << " (0b1100)" << std::endl;
    std::cout << "a & b = " << and_op(a, b) << " (0b1000)" << std::endl;
    std::cout << "a | b = " << or_op(a, b) << " (0b1110)" << std::endl;
    std::cout << "a ^ b = " << xor_op(a, b) << " (0b0110)" << std::endl;
}

void test_predicates() {
    std::cout << "\n=== 谓词函数对象测试 ===" << std::endl;
    
    mystl::is_positive<int> pos;
    mystl::is_negative<int> neg;
    mystl::is_zero<int> zero;
    
    int values[] = {5, -3, 0, 10, -7};
    for (int val : values) {
        std::cout << val << ": positive=" << (pos(val) ? "true" : "false")
                  << ", negative=" << (neg(val) ? "true" : "false")
                  << ", zero=" << (zero(val) ? "true" : "false") << std::endl;
    }
}

void test_function_adapters() {
    std::cout << "\n=== 函数适配器测试 ===" << std::endl;
    
    // 取反适配器
    mystl::less<int> lt;
    auto not_lt = mystl::not2(lt);
    
    int a = 5, b = 3;
    std::cout << a << " < " << b << " ? " << (lt(a, b) ? "true" : "false") << std::endl;
    std::cout << "not(" << a << " < " << b << ") ? " << (not_lt(a, b) ? "true" : "false") << std::endl;
    
    // 绑定器
    mystl::plus<int> add;
    auto add5 = mystl::bind2nd(add, 5);
    
    std::cout << "add5(10) = " << add5(10) << std::endl;
    
    auto add_to_10 = mystl::bind1st(add, 10);
    std::cout << "add_to_10(3) = " << add_to_10(3) << std::endl;
}

void test_utility_functors() {
    std::cout << "\n=== 工具函数对象测试 ===" << std::endl;
    
    mystl::identity<int> id;
    std::cout << "identity(42) = " << id(42) << std::endl;
}

void test_algorithm_usage() {
    std::cout << "\n=== 算法使用示例 ===" << std::endl;
    
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    std::cout << "原始数组: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 使用函数对象进行排序
    std::sort(numbers.begin(), numbers.end(), mystl::less<int>());
    std::cout << "升序排序: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    std::sort(numbers.begin(), numbers.end(), mystl::greater<int>());
    std::cout << "降序排序: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 使用谓词进行过滤
    std::vector<int> positive_numbers;
    std::copy_if(numbers.begin(), numbers.end(), 
                 std::back_inserter(positive_numbers), 
                 mystl::is_positive<int>());
    
    std::cout << "正数: ";
    for (int n : positive_numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void test_function_object_composition() {
    std::cout << "\n=== 函数对象组合测试 ===" << std::endl;
    
    // 组合多个函数对象
    mystl::plus<int> add;
    mystl::multiplies<int> mul;
    
    // 计算 (a + b) * c
    int a = 2, b = 3, c = 4;
    int result = mul(add(a, b), c);
    std::cout << "(" << a << " + " << b << ") * " << c << " = " << result << std::endl;
    
    // 使用函数对象进行复杂计算
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // 计算所有数的平方和
    int sum = 0;
    for (int n : numbers) {
        sum = add(sum, mul(n, n));
    }
    std::cout << "平方和: " << sum << std::endl;
}

void test_exception_safety() {
    std::cout << "\n=== 异常安全测试 ===" << std::endl;
    
    // 测试函数对象的异常安全性
    mystl::plus<int> add;
    
    try {
        int result = add(5, 3);
        std::cout << "add(5, 3) = " << result << " (成功)" << std::endl;
    } catch (...) {
        std::cout << "add(5, 3) 抛出异常" << std::endl;
    }
    
    // 测试绑定器的异常安全性
    auto add_5 = mystl::bind2nd(mystl::plus<int>(), 5);
    
    try {
        int result = add_5(10);
        std::cout << "add_5(10) = " << result << " (成功)" << std::endl;
    } catch (...) {
        std::cout << "add_5(10) 抛出异常" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL 函数对象系统完整测试（独立版）===" << std::endl;
    
    test_arithmetic_functors();
    test_comparison_functors();
    test_logical_functors();
    test_bitwise_functors();
    test_predicates();
    test_function_adapters();
    test_utility_functors();
    test_algorithm_usage();
    test_function_object_composition();
    test_exception_safety();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}











