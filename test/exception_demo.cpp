#include "../exceptdef.h"
#include <iostream>
#include <windows.h>

void demonstrate_exception_creation() {
    std::cout << "=== 异常对象创建 vs 异常抛出 ===" << std::endl;
    
    // 1. 创建异常对象（不抛出）
    std::cout << "1. 创建异常对象（不抛出）:" << std::endl;
    mystl::mystl_exception ex1("这是一个异常对象");
    std::cout << "   异常对象创建成功，消息: " << ex1.what() << std::endl;
    std::cout << "   程序继续执行..." << std::endl;
    
    // 2. 抛出异常（这才是真正的异常）
    std::cout << "\n2. 抛出异常:" << std::endl;
    try {
        throw mystl::mystl_exception("这是真正的异常！");
        std::cout << "   这行不会执行" << std::endl;
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获到异常: " << e.what() << std::endl;
    }
    
    // 3. 使用不同的构造函数
    std::cout << "\n3. 使用不同的构造函数:" << std::endl;
    
    // 默认构造函数
    mystl::mystl_exception ex2;
    std::cout << "   默认构造: " << ex2.what() << std::endl;
    
    // 字符串构造函数
    mystl::mystl_exception ex3(std::string("字符串消息"));
    std::cout << "   字符串构造: " << ex3.what() << std::endl;
    
    // 4. 异常继承关系
    std::cout << "\n4. 异常继承关系:" << std::endl;
    try {
        throw mystl::mystl_exception("测试继承");
    } catch (const std::exception& e) {  // 可以捕获基类
        std::cout << "   通过 std::exception 捕获: " << e.what() << std::endl;
    }
}

void demonstrate_exception_usage() {
    std::cout << "\n=== 异常的实际使用场景 ===" << std::endl;
    
    // 场景1：参数验证
    auto divide = [](int a, int b) -> int {
        if (b == 0) {
            throw mystl::mystl_exception("除数不能为零");
        }
        return a / b;
    };
    
    std::cout << "1. 参数验证测试:" << std::endl;
    try {
        int result1 = divide(10, 2);
        std::cout << "   10 / 2 = " << result1 << std::endl;
        
        int result2 = divide(10, 0);  // 这会抛出异常
        std::cout << "   10 / 0 = " << result2 << std::endl;
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
    
    // 场景2：资源管理
    auto process_data = [](const std::string& data) {
        if (data.empty()) {
            throw mystl::mystl_exception("数据不能为空");
        }
        if (data.length() > 100) {
            throw mystl::mystl_exception("数据长度超过限制");
        }
        std::cout << "   处理数据: " << data << std::endl;
    };
    
    std::cout << "\n2. 资源管理测试:" << std::endl;
    try {
        process_data("正常数据");
        process_data("");  // 这会抛出异常
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
    
    try {
        process_data("这是一个非常长的字符串，超过了100个字符的限制，应该会抛出异常");
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
}

void demonstrate_exception_safety() {
    std::cout << "\n=== 异常安全性演示 ===" << std::endl;
    
    class SafeClass {
        int* data;
        size_t size;
    public:
        SafeClass(size_t n) : size(n) {
            data = new int[n];
            if (n > 1000) {
                delete[] data;  // 清理资源
                throw mystl::mystl_exception("大小超过限制");
            }
        }
        
        ~SafeClass() {
            delete[] data;
        }
        
        size_t getSize() const { return size; }
    };
    
    std::cout << "1. 异常安全测试:" << std::endl;
    try {
        SafeClass obj1(100);  // 正常情况
        std::cout << "   创建成功，大小: " << obj1.getSize() << std::endl;
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
    
    try {
        SafeClass obj2(2000);  // 会抛出异常
        std::cout << "   创建成功" << std::endl;
    } catch (const mystl::mystl_exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   异常安全：资源已正确清理" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 异常概念演示 ===" << std::endl;
    
    demonstrate_exception_creation();
    demonstrate_exception_usage();
    demonstrate_exception_safety();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}











