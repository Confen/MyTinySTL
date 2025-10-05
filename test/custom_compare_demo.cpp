#include "../util.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

// 自定义类
class Person {
public:
    std::string name;
    int age;
    
    Person(const std::string& n, int a) : name(n), age(a) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name << "(" << p.age << ")";
        return os;
    }
};

void demonstrate_basic_compare() {
    std::cout << "=== 基本比较函数演示 ===" << std::endl;
    
    // 1. 默认比较（使用 < 操作符）
    int a = 10, b = 20;
    std::cout << "1. 默认比较:" << std::endl;
    std::cout << "   max(10, 20) = " << mystl::max(a, b) << std::endl;
    
    // 2. 自定义比较函数 - 数值比较
    auto greater = [](int x, int y) { return x > y; };
    std::cout << "\n2. 自定义比较函数 - 数值比较:" << std::endl;
    std::cout << "   max(10, 20, greater) = " << mystl::max(a, b, greater) << std::endl;
    
    // 3. 自定义比较函数 - 反向比较
    auto less = [](int x, int y) { return x < y; };
    std::cout << "\n3. 自定义比较函数 - 反向比较:" << std::endl;
    std::cout << "   max(10, 20, less) = " << mystl::max(a, b, less) << std::endl;
}

void demonstrate_string_compare() {
    std::cout << "\n=== 字符串比较演示 ===" << std::endl;
    
    std::string str1 = "hello";
    std::string str2 = "world";
    
    // 1. 按字典序比较
    auto lexicographical = [](const std::string& a, const std::string& b) {
        return a < b;
    };
    std::cout << "1. 按字典序比较:" << std::endl;
    std::cout << "   max(\"hello\", \"world\", lexicographical) = " 
              << mystl::max(str1, str2, lexicographical) << std::endl;
    
    // 2. 按长度比较
    auto byLength = [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    };
    std::cout << "\n2. 按长度比较:" << std::endl;
    std::cout << "   max(\"hello\", \"world\", byLength) = " 
              << mystl::max(str1, str2, byLength) << std::endl;
    
    // 3. 按长度反向比较
    auto byLengthReverse = [](const std::string& a, const std::string& b) {
        return a.length() > b.length();
    };
    std::cout << "\n3. 按长度反向比较:" << std::endl;
    std::cout << "   max(\"hello\", \"world\", byLengthReverse) = " 
              << mystl::max(str1, str2, byLengthReverse) << std::endl;
}

void demonstrate_object_compare() {
    std::cout << "\n=== 对象比较演示 ===" << std::endl;
    
    Person p1("Alice", 25);
    Person p2("Bob", 30);
    
    // 1. 按年龄比较
    auto byAge = [](const Person& a, const Person& b) {
        return a.age < b.age;
    };
    std::cout << "1. 按年龄比较:" << std::endl;
    std::cout << "   max(Alice(25), Bob(30), byAge) = " 
              << mystl::max(p1, p2, byAge) << std::endl;
    
    // 2. 按姓名比较
    auto byName = [](const Person& a, const Person& b) {
        return a.name < b.name;
    };
    std::cout << "\n2. 按姓名比较:" << std::endl;
    std::cout << "   max(Alice(25), Bob(30), byName) = " 
              << mystl::max(p1, p2, byName) << std::endl;
    
    // 3. 按年龄反向比较
    auto byAgeReverse = [](const Person& a, const Person& b) {
        return a.age > b.age;
    };
    std::cout << "\n3. 按年龄反向比较:" << std::endl;
    std::cout << "   max(Alice(25), Bob(30), byAgeReverse) = " 
              << mystl::max(p1, p2, byAgeReverse) << std::endl;
}

void demonstrate_function_objects() {
    std::cout << "\n=== 函数对象演示 ===" << std::endl;
    
    // 1. 使用标准库函数对象
    std::cout << "1. 使用标准库函数对象:" << std::endl;
    int a = 10, b = 20;
    std::cout << "   max(10, 20, std::greater<int>()) = " 
              << mystl::max(a, b, std::greater<int>()) << std::endl;
    
    // 2. 自定义函数对象
    class CustomCompare {
    public:
        bool operator()(int x, int y) const {
            return x % 10 < y % 10;  // 按个位数比较
        }
    };
    
    std::cout << "\n2. 自定义函数对象:" << std::endl;
    std::cout << "   max(15, 23, CustomCompare()) = " 
              << mystl::max(15, 23, CustomCompare()) << std::endl;
}

void demonstrate_complex_compare() {
    std::cout << "\n=== 复杂比较演示 ===" << std::endl;
    
    std::vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 1. 按绝对值比较
    auto byAbs = [](int x, int y) {
        return std::abs(x) < std::abs(y);
    };
    
    int maxAbs = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        maxAbs = mystl::max(maxAbs, nums[i], byAbs);
    }
    
    std::cout << "1. 按绝对值比较:" << std::endl;
    std::cout << "   数组: [3, 1, 4, 1, 5, 9, 2, 6]" << std::endl;
    std::cout << "   最大绝对值: " << maxAbs << std::endl;
    
    // 2. 按数字各位数之和比较
    auto byDigitSum = [](int x, int y) {
        auto digitSum = [](int n) {
            int sum = 0;
            while (n > 0) {
                sum += n % 10;
                n /= 10;
            }
            return sum;
        };
        return digitSum(x) < digitSum(y);
    };
    
    int maxDigitSum = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        maxDigitSum = mystl::max(maxDigitSum, nums[i], byDigitSum);
    }
    
    std::cout << "\n2. 按数字各位数之和比较:" << std::endl;
    std::cout << "   最大各位数之和: " << maxDigitSum << std::endl;
}

void demonstrate_compare_benefits() {
    std::cout << "\n=== 自定义比较函数的优势 ===" << std::endl;
    
    std::cout << "1. 灵活性:" << std::endl;
    std::cout << "   可以定义任何比较规则" << std::endl;
    std::cout << "   不局限于默认的 < 操作符" << std::endl;
    
    std::cout << "\n2. 可读性:" << std::endl;
    std::cout << "   比较逻辑清晰明确" << std::endl;
    std::cout << "   代码意图更容易理解" << std::endl;
    
    std::cout << "\n3. 复用性:" << std::endl;
    std::cout << "   同一个比较函数可以在多处使用" << std::endl;
    std::cout << "   避免重复代码" << std::endl;
    
    std::cout << "\n4. 性能:" << std::endl;
    std::cout << "   编译器可以内联比较函数" << std::endl;
    std::cout << "   运行时性能优秀" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 自定义比较函数演示 ===" << std::endl;
    
    demonstrate_basic_compare();
    demonstrate_string_compare();
    demonstrate_object_compare();
    demonstrate_function_objects();
    demonstrate_complex_compare();
    demonstrate_compare_benefits();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}




















