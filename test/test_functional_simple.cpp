#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <windows.h>

// 简化的函数对象实现，避免头文件冲突
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

// 比较函数对象
template<typename T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x == y;
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

// 恒等函数
template<typename T>
struct identity : public unary_function<T, T> {
    const T& operator()(const T& x) const {
        return x;
    }
};

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
    
    mystl::plus<int> add;
    mystl::minus<int> sub;
    mystl::multiplies<int> mul;
    
    int a = 10, b = 3;
    std::cout << a << " + " << b << " = " << add(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << sub(a, b) << std::endl;
    std::cout << a << " * " << b << " = " << mul(a, b) << std::endl;
}

void test_comparison_functors() {
    std::cout << "\n=== 比较函数对象测试 ===" << std::endl;
    
    mystl::equal_to<int> eq;
    mystl::greater<int> gt;
    mystl::less<int> lt;
    
    int a = 5, b = 3;
    std::cout << a << " == " << b << " ? " << (eq(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " > " << b << " ? " << (gt(a, b) ? "true" : "false") << std::endl;
    std::cout << a << " < " << b << " ? " << (lt(a, b) ? "true" : "false") << std::endl;
}

void test_predicates() {
    std::cout << "\n=== 谓词函数对象测试 ===" << std::endl;
    
    mystl::is_positive<int> pos;
    mystl::is_negative<int> neg;
    
    int values[] = {5, -3, 0, 10, -7};
    for (int val : values) {
        std::cout << val << ": positive=" << (pos(val) ? "true" : "false")
                  << ", negative=" << (neg(val) ? "true" : "false") << std::endl;
    }
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

void test_function_object_concepts() {
    std::cout << "\n=== 函数对象概念演示 ===" << std::endl;
    
    // 1. 函数对象可以作为参数传递
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // 2. 使用 lambda 表达式（C++11 特性）
    auto square = [](int x) { return x * x; };
    std::cout << "lambda square(5) = " << square(5) << std::endl;
    
    // 3. 使用函数对象进行变换
    std::vector<int> squares;
    std::transform(numbers.begin(), numbers.end(), 
                   std::back_inserter(squares), square);
    
    std::cout << "平方数: ";
    for (int n : squares) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 4. 函数对象的组合使用
    mystl::plus<int> add;
    mystl::multiplies<int> mul;
    
    // 计算 (a + b) * c
    int a = 2, b = 3, c = 4;
    int result = mul(add(a, b), c);
    std::cout << "(" << a << " + " << b << ") * " << c << " = " << result << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL 函数对象系统测试（简化版）===" << std::endl;
    
    test_arithmetic_functors();
    test_comparison_functors();
    test_predicates();
    test_utility_functors();
    test_algorithm_usage();
    test_function_object_concepts();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}











