#include "../exceptdef.h"
#include <iostream>
#include <windows.h>

void function_that_throws() {
    std::cout << "函数执行中..." << std::endl;
    throw std::runtime_error("这是一个测试异常");
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 宏行为对比演示 ===" << std::endl;
    std::cout << "证明：宏不会阻止异常，只是捕获异常用于测试\n" << std::endl;
    
    // 情况1：不使用宏 - 异常传播到外层
    std::cout << "1. 不使用宏的情况：" << std::endl;
    try {
        std::cout << "调用 function_that_throws()..." << std::endl;
        function_that_throws();  // 异常会传播出去
        std::cout << "这行不会执行" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "外层捕获到异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n" << std::endl;
    
    // 情况2：使用宏 - 异常被宏捕获，不传播
    std::cout << "2. 使用 MYSTL_BASIC_GUARANTEE_ASSERT 宏：" << std::endl;
    std::cout << "调用宏包装的 function_that_throws()..." << std::endl;
    
    MYSTL_BASIC_GUARANTEE_ASSERT(function_that_throws());
    
    std::cout << "这行会执行！因为异常被宏捕获了" << std::endl;
    std::cout << "宏的作用：捕获异常，验证基本异常保证，但不让异常传播" << std::endl;
    
    std::cout << "\n" << std::endl;
    
    // 情况3：展示宏的实际等价代码
    std::cout << "3. 宏的等价手写代码：" << std::endl;
    std::cout << "调用手写的异常捕获代码..." << std::endl;
    
    // 这就是宏展开后的实际代码
    do {
        try {
            function_that_throws();
        } catch (...) {
            /* 基本异常保证：对象处于有效状态 */
            std::cout << "手写代码捕获到异常（模拟宏的行为）" << std::endl;
        }
    } while(0);
    
    std::cout << "手写代码执行完毕，异常被捕获" << std::endl;
    
    std::cout << "\n=== 关键理解 ===" << std::endl;
    std::cout << "宏的设计意图：" << std::endl;
    std::cout << "✓ 异常确实会被抛出（函数内部的异常机制正常工作）" << std::endl;
    std::cout << "✓ 宏捕获异常，用于测试异常安全性" << std::endl;
    std::cout << "✓ 宏不让异常传播到外层，保证测试代码继续执行" << std::endl;
    std::cout << "✓ 这样可以在测试中验证：'即使发生异常，对象仍处于有效状态'" << std::endl;
    
    return 0;
}








