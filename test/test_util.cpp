/**
 * @file test_util.cpp
 * @brief 测试 MyTinySTL 工具函数系统
 * @author MyTinySTL
 * @date 2024
 */

#include "../util.h"
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

// 测试用的类
class TestClass {
    int value;
public:
    TestClass(int v = 0) : value(v) {}
    
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
    
    void swap(TestClass& other) noexcept {
        using std::swap;
        swap(value, other.value);
    }
    
    bool operator<(const TestClass& other) const {
        return value < other.value;
    }
    
    bool operator==(const TestClass& other) const {
        return value == other.value;
    }
};

void test_pair() {
    std::cout << "=== 测试 pair 模板类 ===" << std::endl;
    
    // 测试基本构造
    mystl::pair<int, std::string> p1(42, "hello");
    std::cout << "p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    
    // 测试 make_pair
    auto p2 = mystl::make_pair(3.14, std::string("world"));
    std::cout << "p2: (" << p2.first << ", " << p2.second << ")" << std::endl;
    
    // 测试移动语义
    auto p3 = mystl::make_pair(std::move(p1.first), std::move(p1.second));
    std::cout << "p3 (移动后): (" << p3.first << ", " << p3.second << ")" << std::endl;
    
    // 测试比较操作符
    mystl::pair<int, int> p4(1, 2);
    mystl::pair<int, int> p5(1, 2);
    mystl::pair<int, int> p6(2, 1);
    
    std::cout << "p4 == p5: " << (p4 == p5) << std::endl;
    std::cout << "p4 != p6: " << (p4 != p6) << std::endl;
    std::cout << "p4 < p6: " << (p4 < p6) << std::endl;
    std::cout << "p4 <= p5: " << (p4 <= p5) << std::endl;
    std::cout << "p4 > p6: " << (p4 > p6) << std::endl;
    std::cout << "p4 >= p5: " << (p4 >= p5) << std::endl;
    
    // 测试 swap
    p4.swap(p6);
    std::cout << "swap后 p4: (" << p4.first << ", " << p4.second << ")" << std::endl;
    std::cout << "swap后 p6: (" << p6.first << ", " << p6.second << ")" << std::endl;
}

void test_move_forward() {
    std::cout << "\n=== 测试移动语义函数 ===" << std::endl;
    
    // 测试 move
    std::string str1 = "hello";
    std::string str2 = mystl::move(str1);
    std::cout << "move后 str1: '" << str1 << "'" << std::endl;
    std::cout << "move后 str2: '" << str2 << "'" << std::endl;
    
    // 测试 forward
    auto forward_test = [](std::string&& s) {
        return mystl::forward<std::string>(s);
    };
    
    std::string str3 = "world";
    std::string str4 = forward_test(mystl::move(str3));
    std::cout << "forward后 str3: '" << str3 << "'" << std::endl;
    std::cout << "forward后 str4: '" << str4 << "'" << std::endl;
}

void test_swap() {
    std::cout << "\n=== 测试 swap 函数 ===" << std::endl;
    
    // 测试基本类型
    int a = 10, b = 20;
    std::cout << "swap前: a=" << a << ", b=" << b << std::endl;
    mystl::swap(a, b);
    std::cout << "swap后: a=" << a << ", b=" << b << std::endl;
    
    // 测试自定义类型
    TestClass tc1(100), tc2(200);
    std::cout << "swap前: tc1=" << tc1.getValue() << ", tc2=" << tc2.getValue() << std::endl;
    mystl::swap(tc1, tc2);
    std::cout << "swap后: tc1=" << tc1.getValue() << ", tc2=" << tc2.getValue() << std::endl;
    
    // 测试数组
    int arr1[3] = {1, 2, 3};
    int arr2[3] = {4, 5, 6};
    std::cout << "swap前 arr1: [" << arr1[0] << ", " << arr1[1] << ", " << arr1[2] << "]" << std::endl;
    std::cout << "swap前 arr2: [" << arr2[0] << ", " << arr2[1] << ", " << arr2[2] << "]" << std::endl;
    mystl::swap(arr1, arr2);
    std::cout << "swap后 arr1: [" << arr1[0] << ", " << arr1[1] << ", " << arr1[2] << "]" << std::endl;
    std::cout << "swap后 arr2: [" << arr2[0] << ", " << arr2[1] << ", " << arr2[2] << "]" << std::endl;
}

void test_min_max() {
    std::cout << "\n=== 测试 min/max 函数 ===" << std::endl;
    
    // 测试基本类型
    int x = 10, y = 20;
    std::cout << "min(10, 20) = " << mystl::min(x, y) << std::endl;
    std::cout << "max(10, 20) = " << mystl::max(x, y) << std::endl;
    
    // 测试自定义类型
    TestClass tc1(15), tc2(25);
    std::cout << "min(TestClass(15), TestClass(25)) = " << mystl::min(tc1, tc2).getValue() << std::endl;
    std::cout << "max(TestClass(15), TestClass(25)) = " << mystl::max(tc1, tc2).getValue() << std::endl;
    
    // 测试自定义比较器
    auto comp = [](const TestClass& a, const TestClass& b) {
        return a.getValue() > b.getValue();  // 反向比较
    };
    std::cout << "min(反向比较) = " << mystl::min(tc1, tc2, comp).getValue() << std::endl;
    std::cout << "max(反向比较) = " << mystl::max(tc1, tc2, comp).getValue() << std::endl;
    
    // 测试 minmax
    auto result = mystl::minmax(x, y);
    std::cout << "minmax(10, 20) = (" << result.first << ", " << result.second << ")" << std::endl;
    
    // 测试初始化列表
    std::cout << "min({5, 2, 8, 1, 9}) = " << mystl::min({5, 2, 8, 1, 9}) << std::endl;
    std::cout << "max({5, 2, 8, 1, 9}) = " << mystl::max({5, 2, 8, 1, 9}) << std::endl;
    
    auto minmax_result = mystl::minmax({5, 2, 8, 1, 9});
    std::cout << "minmax({5, 2, 8, 1, 9}) = (" << minmax_result.first << ", " << minmax_result.second << ")" << std::endl;
}

void test_performance() {
    std::cout << "\n=== 性能测试 ===" << std::endl;
    
    const int N = 1000000;
    std::vector<int> vec1(N), vec2(N);
    
    // 初始化
    for (int i = 0; i < N; ++i) {
        vec1[i] = i;
        vec2[i] = N - i;
    }
    
    // 测试 swap 性能
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        mystl::swap(vec1, vec2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "swap " << N << " 个元素 1000 次耗时: " << duration.count() << " 微秒" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL 工具函数系统测试 ===" << std::endl;
    
    try {
        test_pair();
        test_move_forward();
        test_swap();
        test_min_max();
        test_performance();
        
        std::cout << "\n=== 所有测试通过 ===" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}



















