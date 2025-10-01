/**
 * @file test_navigation.cpp
 * @brief 测试 Cursor 的"定位到声明"功能
 * @author MyTinySTL
 * @date 2024
 */

#include "util.h"
#include "type_traits.h"
#include "exceptdef.h"
#include "functional.h"
#include <iostream>
#include <vector>

using namespace mystl;

// 测试类，用于验证符号导航
class TestClass {
public:
    TestClass() = default;
    ~TestClass() = default;
    
    void testMethod() {
        std::cout << "Test method called" << std::endl;
    }
    
    int testValue = 42;
};

// 测试函数，用于验证函数导航
void testFunction() {
    std::cout << "Test function called" << std::endl;
}

// 测试模板函数
template<typename T>
void testTemplateFunction(T value) {
    std::cout << "Template function called with: " << value << std::endl;
}

int main() {
    std::cout << "=== MyTinySTL 符号导航测试 ===" << std::endl;
    
    // 测试 pair 类导航
    pair<int, std::string> testPair = make_pair(42, "hello");
    std::cout << "Pair: (" << testPair.first << ", " << testPair.second << ")" << std::endl;
    
    // 测试 move 函数导航
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = move(vec1);
    std::cout << "Move test completed" << std::endl;
    
    // 测试 swap 函数导航
    int a = 10, b = 20;
    swap(a, b);
    std::cout << "After swap: a=" << a << ", b=" << b << std::endl;
    
    // 测试 max/min 函数导航
    int maxVal = max(a, b);
    int minVal = min(a, b);
    std::cout << "Max: " << maxVal << ", Min: " << minVal << std::endl;
    
    // 测试自定义类导航
    TestClass testObj;
    testObj.testMethod();
    std::cout << "Test value: " << testObj.testValue << std::endl;
    
    // 测试自定义函数导航
    testFunction();
    
    // 测试模板函数导航
    testTemplateFunction(42);
    testTemplateFunction(std::string("template test"));
    
    // 测试类型萃取导航
    static_assert(is_same<int, int>::value, "Type traits test");
    std::cout << "Type traits test passed" << std::endl;
    
    // 测试异常处理导航
    try {
        throw mystl::mystl_logic_error("Test exception");
    } catch (const mystl::mystl_logic_error& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    // 测试函数对象导航
    mystl::plus<int> adder;
    int sum = adder(10, 20);
    std::cout << "Function object test: 10 + 20 = " << sum << std::endl;
    
    std::cout << "=== 所有测试完成 ===" << std::endl;
    
    return 0;
}

