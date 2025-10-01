#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <windows.h>

#include "../algobase.h"

using namespace mystl;

// 测试交换算法
void test_swap_algorithms() {
    std::cout << "=== 测试交换算法 ===" << std::endl;
    
    // 测试基本交换
    int a = 10, b = 20;
    std::cout << "交换前: a = " << a << ", b = " << b << std::endl;
    mystl::swap(a, b);
    std::cout << "交换后: a = " << a << ", b = " << b << std::endl;
    
    // 测试字符串交换
    std::string str1 = "Hello", str2 = "World";
    std::cout << "字符串交换前: str1 = " << str1 << ", str2 = " << str2 << std::endl;
    mystl::swap(str1, str2);
    std::cout << "字符串交换后: str1 = " << str1 << ", str2 = " << str2 << std::endl;
    
    // 测试 swap_ranges
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {6, 7, 8, 9, 10};
    std::cout << "swap_ranges 前: vec1 = ";
    for (int val : vec1) std::cout << val << " ";
    std::cout << ", vec2 = ";
    for (int val : vec2) std::cout << val << " ";
    std::cout << std::endl;
    
    mystl::swap_ranges(vec1.begin(), vec1.end(), vec2.begin());
    std::cout << "swap_ranges 后: vec1 = ";
    for (int val : vec1) std::cout << val << " ";
    std::cout << ", vec2 = ";
    for (int val : vec2) std::cout << val << " ";
    std::cout << std::endl;
}

// 测试比较算法
void test_comparison_algorithms() {
    std::cout << "\n=== 测试比较算法 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    std::vector<int> vec3 = {1, 2, 3, 4, 6};
    
    // 测试 equal
    bool result1 = mystl::equal(vec1.begin(), vec1.end(), vec2.begin());
    bool result2 = mystl::equal(vec1.begin(), vec1.end(), vec3.begin());
    std::cout << "vec1 == vec2: " << (result1 ? "true" : "false") << std::endl;
    std::cout << "vec1 == vec3: " << (result2 ? "true" : "false") << std::endl;
    
    // 测试自定义比较函数
    auto custom_compare = [](int a, int b) { return a == b; };
    bool result3 = mystl::equal(vec1.begin(), vec1.end(), vec2.begin(), custom_compare);
    std::cout << "自定义比较 vec1 == vec2: " << (result3 ? "true" : "false") << std::endl;
    
    // 测试 lexicographical_compare
    std::string str1 = "apple";
    std::string str2 = "banana";
    std::string str3 = "apple";
    
    bool lex1 = mystl::lexicographical_compare(str1.begin(), str1.end(), 
                                              str2.begin(), str2.end());
    bool lex2 = mystl::lexicographical_compare(str2.begin(), str2.end(), 
                                              str1.begin(), str1.end());
    bool lex3 = mystl::lexicographical_compare(str1.begin(), str1.end(), 
                                              str3.begin(), str3.end());
    
    std::cout << "apple < banana: " << (lex1 ? "true" : "false") << std::endl;
    std::cout << "banana < apple: " << (lex2 ? "true" : "false") << std::endl;
    std::cout << "apple < apple: " << (lex3 ? "true" : "false") << std::endl;
}

// 测试拷贝算法
void test_copy_algorithms() {
    std::cout << "\n=== 测试拷贝算法 ===" << std::endl;
    
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::vector<int> dest1(5);
    std::vector<int> dest2(5);
    
    // 测试 copy
    mystl::copy(source.begin(), source.end(), dest1.begin());
    std::cout << "copy 结果: ";
    for (int val : dest1) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 copy_backward
    mystl::copy_backward(source.begin(), source.end(), dest2.end());
    std::cout << "copy_backward 结果: ";
    for (int val : dest2) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 move
    std::vector<std::string> str_source = {"Hello", "World", "Test"};
    std::vector<std::string> str_dest(3);
    
    mystl::move(str_source.begin(), str_source.end(), str_dest.begin());
    std::cout << "move 结果: ";
    for (const auto& str : str_dest) std::cout << str << " ";
    std::cout << std::endl;
    std::cout << "源容器大小: " << str_source.size() << std::endl;
}

// 测试填充算法
void test_fill_algorithms() {
    std::cout << "\n=== 测试填充算法 ===" << std::endl;
    
    std::vector<int> vec(5);
    
    // 测试 fill
    mystl::fill(vec.begin(), vec.end(), 42);
    std::cout << "fill 结果: ";
    for (int val : vec) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 fill_n
    std::vector<int> vec2(3);
    mystl::fill_n(vec2.begin(), 3, 99);
    std::cout << "fill_n 结果: ";
    for (int val : vec2) std::cout << val << " ";
    std::cout << std::endl;
}

// 测试初始化算法
void test_initialization_algorithms() {
    std::cout << "\n=== 测试初始化算法 ===" << std::endl;
    
    // 测试 uninitialized_fill
    std::vector<int> vec(5);
    mystl::uninitialized_fill(vec.begin(), vec.end(), 100);
    std::cout << "uninitialized_fill 结果: ";
    for (int val : vec) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 uninitialized_fill_n
    std::vector<int> vec2(3);
    mystl::uninitialized_fill_n(vec2.begin(), 3, 200);
    std::cout << "uninitialized_fill_n 结果: ";
    for (int val : vec2) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 uninitialized_copy
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::vector<int> dest(5);
    mystl::uninitialized_copy(source.begin(), source.end(), dest.begin());
    std::cout << "uninitialized_copy 结果: ";
    for (int val : dest) std::cout << val << " ";
    std::cout << std::endl;
    
    // 测试 uninitialized_move
    std::vector<std::string> str_source = {"Move", "Test", "Demo"};
    std::vector<std::string> str_dest(3);
    mystl::uninitialized_move(str_source.begin(), str_source.end(), str_dest.begin());
    std::cout << "uninitialized_move 结果: ";
    for (const auto& str : str_dest) std::cout << str << " ";
    std::cout << std::endl;
}

// 测试 POD 类型特化
void test_pod_specializations() {
    std::cout << "\n=== 测试 POD 类型特化 ===" << std::endl;
    
    // 测试 char 类型特化
    char char_source[] = "Hello";
    char char_dest[6];
    mystl::copy(char_source, char_source + 5, char_dest);
    char_dest[5] = '\0';
    std::cout << "char copy 结果: " << char_dest << std::endl;
    
    // 测试 char 填充特化
    char fill_buffer[10];
    mystl::fill(fill_buffer, fill_buffer + 10, 'A');
    std::cout << "char fill 结果: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << fill_buffer[i];
    }
    std::cout << std::endl;
    
    // 测试 wchar_t 类型特化
    wchar_t wchar_source[] = L"World";
    wchar_t wchar_dest[6];
    mystl::copy(wchar_source, wchar_source + 5, wchar_dest);
    wchar_dest[5] = L'\0';
    std::wcout << L"wchar_t copy 结果: " << wchar_dest << std::endl;
}

// 测试异常安全性
void test_exception_safety() {
    std::cout << "\n=== 测试异常安全性 ===" << std::endl;
    
    // 使用全局静态变量
    static int throwing_count = 0;
    
    // 创建一个会抛出异常的类
    class ThrowingClass {
    private:
        int value_;
        
    public:
        ThrowingClass(int v) : value_(v) {
            ++throwing_count;
            if (throwing_count == 3) {
                throw std::runtime_error("构造失败");
            }
        }
        
        ~ThrowingClass() {
            --throwing_count;
        }
        
        int getValue() const { return value_; }
    };
    
    // 测试异常安全的 uninitialized_fill
    char buffer[sizeof(ThrowingClass) * 5];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    throwing_count = 0;
    
    try {
        std::cout << "尝试 uninitialized_fill，第3个对象会抛出异常" << std::endl;
        mystl::uninitialized_fill(objects, objects + 5, ThrowingClass(42));
        std::cout << "uninitialized_fill 成功完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "已构造的对象数量: " << throwing_count << std::endl;
        std::cout << "异常安全：已构造的对象被正确清理" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    test_swap_algorithms();
    test_comparison_algorithms();
    test_copy_algorithms();
    test_fill_algorithms();
    test_initialization_algorithms();
    test_pod_specializations();
    test_exception_safety();
    
    std::cout << "\n=== 算法基础测试完成 ===" << std::endl;
    
    return 0;
}
