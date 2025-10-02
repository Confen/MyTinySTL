#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include "../functional.h"

using namespace mystl;

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
    negate<int> neg;
    std::cout << "negate(5) = " << neg(5) << std::endl;
    
    // 二元算术函数
    plus<int> add;
    minus<int> sub;
    multiplies<int> mul;
    divides<int> div;
    modulus<int> mod;
    
    int a = 10, b = 3;
    std::cout << a << " + " << b << " = " << add(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << sub(a, b) << std::endl;
    std::cout << a << " * " << b << " = " << mul(a, b) << std::endl;
    std::cout << a << " / " << b << " = " << div(a, b) << std::endl;
    std::cout << a << " % " << b << " = " << mod(a, b) << std::endl;
}

void test_comparison_functors() {
    std::cout << "\n=== 比较函数对象测试 ===" << std::endl;
    
    equal_to<int> eq;
    not_equal_to<int> ne;
    greater<int> gt;
    less<int> lt;
    greater_equal<int> ge;
    less_equal<int> le;
    
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
    
    logical_and<bool> and_op;
    logical_or<bool> or_op;
    logical_not<bool> not_op;
    
    bool x = true, y = false;
    std::cout << x << " && " << y << " = " << (and_op(x, y) ? "true" : "false") << std::endl;
    std::cout << x << " || " << y << " = " << (or_op(x, y) ? "true" : "false") << std::endl;
    std::cout << "!" << x << " = " << (not_op(x) ? "true" : "false") << std::endl;
}

void test_bitwise_functors() {
    std::cout << "\n=== 位运算函数对象测试 ===" << std::endl;
    
    bit_and<int> and_op;
    bit_or<int> or_op;
    bit_xor<int> xor_op;
    
    int a = 0b1010, b = 0b1100;
    std::cout << "a = " << a << " (0b1010)" << std::endl;
    std::cout << "b = " << b << " (0b1100)" << std::endl;
    std::cout << "a & b = " << and_op(a, b) << " (0b1000)" << std::endl;
    std::cout << "a | b = " << or_op(a, b) << " (0b1110)" << std::endl;
    std::cout << "a ^ b = " << xor_op(a, b) << " (0b0110)" << std::endl;
}

void test_predicates() {
    std::cout << "\n=== 谓词函数对象测试 ===" << std::endl;
    
    is_positive<int> pos;
    is_negative<int> neg;
    is_zero<int> zero;
    
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
    less<int> lt;
    auto not_lt = not2(lt);
    
    int a = 5, b = 3;
    std::cout << a << " < " << b << " ? " << (lt(a, b) ? "true" : "false") << std::endl;
    std::cout << "not(" << a << " < " << b << ") ? " << (not_lt(a, b) ? "true" : "false") << std::endl;
    
    // 绑定器
    plus<int> add;
    auto add5 = bind2nd(add, 5);
    
    std::cout << "add5(10) = " << add5(10) << std::endl;
    
    auto add_to_10 = bind1st(add, 10);
    std::cout << "add_to_10(3) = " << add_to_10(3) << std::endl;
}

void test_function_pointers() {
    std::cout << "\n=== 函数指针适配器测试 ===" << std::endl;
    
    // 普通函数
    int square(int x) { return x * x; }
    int add(int x, int y) { return x + y; }
    
    // 函数指针适配器
    auto square_fun = ptr_fun(square);
    auto add_fun = ptr_fun(add);
    
    std::cout << "square(5) = " << square_fun(5) << std::endl;
    std::cout << "add(3, 4) = " << add_fun(3, 4) << std::endl;
}

void test_member_function_adapters() {
    std::cout << "\n=== 成员函数适配器测试 ===" << std::endl;
    
    std::vector<TestClass> objects;
    objects.emplace_back(5, "Alice");
    objects.emplace_back(-3, "Bob");
    objects.emplace_back(0, "Charlie");
    objects.emplace_back(10, "David");
    
    // 成员函数适配器
    auto getValue = mem_fun_ref(&TestClass::getValue);
    auto isPositive = mem_fun_ref(&TestClass::isPositive);
    
    std::cout << "对象值: ";
    for (const auto& obj : objects) {
        std::cout << getValue(obj) << " ";
    }
    std::cout << std::endl;
    
    std::cout << "正值检查: ";
    for (const auto& obj : objects) {
        std::cout << (isPositive(obj) ? "true" : "false") << " ";
    }
    std::cout << std::endl;
}

void test_utility_functors() {
    std::cout << "\n=== 工具函数对象测试 ===" << std::endl;
    
    // 恒等函数
    identity<int> id;
    std::cout << "identity(42) = " << id(42) << std::endl;
    
    // 选择函数
    std::pair<int, std::string> p(10, "hello");
    select1st<std::pair<int, std::string>> sel1;
    select2nd<std::pair<int, std::string>> sel2;
    
    std::cout << "select1st(" << p.first << ", \"" << p.second << "\") = " << sel1(p) << std::endl;
    std::cout << "select2nd(" << p.first << ", \"" << p.second << "\") = " << sel2(p) << std::endl;
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
    std::sort(numbers.begin(), numbers.end(), less<int>());
    std::cout << "升序排序: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    std::sort(numbers.begin(), numbers.end(), greater<int>());
    std::cout << "降序排序: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 使用谓词进行过滤
    std::vector<int> positive_numbers;
    std::copy_if(numbers.begin(), numbers.end(), 
                 std::back_inserter(positive_numbers), 
                 is_positive<int>());
    
    std::cout << "正数: ";
    for (int n : positive_numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void test_placeholder_usage() {
    std::cout << "\n=== 占位符使用示例 ===" << std::endl;
    
    // 使用占位符进行绑定
    plus<int> add;
    auto add_placeholder = bind(add, _1, _2);
    
    std::cout << "add_placeholder(5, 3) = " << add_placeholder(5, 3) << std::endl;
    
    // 使用占位符进行部分绑定
    auto add_5 = bind(add, _1, 5);
    std::cout << "add_5(10) = " << add_5(10) << std::endl;
}

void test_function_object_composition() {
    std::cout << "\n=== 函数对象组合测试 ===" << std::endl;
    
    // 组合多个函数对象
    plus<int> add;
    multiplies<int> mul;
    
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
    plus<int> add;
    
    try {
        int result = add(5, 3);
        std::cout << "add(5, 3) = " << result << " (成功)" << std::endl;
    } catch (...) {
        std::cout << "add(5, 3) 抛出异常" << std::endl;
    }
    
    // 测试绑定器的异常安全性
    auto add_5 = bind2nd(plus<int>(), 5);
    
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
    
    std::cout << "=== MyTinySTL 函数对象系统完整测试 ===" << std::endl;
    
    test_arithmetic_functors();
    test_comparison_functors();
    test_logical_functors();
    test_bitwise_functors();
    test_predicates();
    test_function_adapters();
    test_function_pointers();
    test_member_function_adapters();
    test_utility_functors();
    test_algorithm_usage();
    test_placeholder_usage();
    test_function_object_composition();
    test_exception_safety();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}











