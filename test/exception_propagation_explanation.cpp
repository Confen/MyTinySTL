#include <iostream>
#include <stdexcept>
#include <windows.h>

void function_that_throws() {
    std::cout << "函数内部：准备抛出异常" << std::endl;
    throw std::runtime_error("测试异常");
    std::cout << "函数内部：这行不会执行" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 异常传播机制详解 ===" << std::endl;
    
    // 情况1：异常被捕获但重新抛出 - 会传播到外层
    std::cout << "\n1. 异常被捕获但重新抛出（会传播到外层）：" << std::endl;
    try {
        std::cout << "外层：开始执行" << std::endl;
        
        try {
            std::cout << "内层：调用会抛异常的函数" << std::endl;
            function_that_throws();
        } catch (...) {
            std::cout << "内层：捕获到异常，但重新抛出" << std::endl;
            throw;  // 重新抛出异常，异常会继续传播
        }
        
        std::cout << "外层：这行不会执行" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "外层：捕获到重新抛出的异常: " << e.what() << std::endl;
    }
    
    // 情况2：异常被捕获但不重新抛出 - 不会传播到外层
    std::cout << "\n2. 异常被捕获但不重新抛出（不会传播到外层）：" << std::endl;
    try {
        std::cout << "外层：开始执行" << std::endl;
        
        try {
            std::cout << "内层：调用会抛异常的函数" << std::endl;
            function_that_throws();
        } catch (...) {
            std::cout << "内层：捕获到异常，但不重新抛出" << std::endl;
            // 注意：这里没有 throw; 语句
            // 异常被"吞掉"了，不会继续传播
        }
        
        std::cout << "外层：这行会执行！因为异常被内层处理了" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "外层：这行不会执行，因为没有异常传播到这里" << std::endl;
    }
    
    // 情况3：模拟宏的行为
    std::cout << "\n3. 模拟 MYSTL_BASIC_GUARANTEE_ASSERT 宏的行为：" << std::endl;
    
    std::cout << "宏外层：开始执行" << std::endl;
    
    // 这就是宏展开后的代码
    do {
        try {
            std::cout << "宏内层：调用会抛异常的函数" << std::endl;
            function_that_throws();
        } catch (...) {
            std::cout << "宏内层：捕获到异常，进行异常安全性验证" << std::endl;
            std::cout << "宏内层：验证对象是否仍处于有效状态..." << std::endl;
            // 关键：这里没有 throw; 语句
            // 所以异常不会传播出去
        }
    } while(0);
    
    std::cout << "宏外层：这行会执行！异常被宏内部处理了" << std::endl;
    
    // 情况4：对比 - 如果宏重新抛出异常会怎样
    std::cout << "\n4. 如果宏重新抛出异常会怎样：" << std::endl;
    try {
        std::cout << "外层：开始执行" << std::endl;
        
        // 假设这是一个会重新抛出异常的宏
        do {
            try {
                std::cout << "假想宏内层：调用会抛异常的函数" << std::endl;
                function_that_throws();
            } catch (...) {
                std::cout << "假想宏内层：捕获到异常，但重新抛出" << std::endl;
                throw;  // 如果宏这样做，异常就会传播出去
            }
        } while(0);
        
        std::cout << "外层：这行不会执行" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "外层：捕获到从宏传播出来的异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== 关键理解 ===" << std::endl;
    std::cout << "异常传播的规则：" << std::endl;
    std::cout << "1. 异常被 try-catch 捕获后，默认不会继续传播" << std::endl;
    std::cout << "2. 只有在 catch 块中使用 'throw;' 才会重新抛出异常" << std::endl;
    std::cout << "3. MYSTL_BASIC_GUARANTEE_ASSERT 宏故意不重新抛出异常" << std::endl;
    std::cout << "4. 这样设计是为了让测试代码能够继续执行，验证异常安全性" << std::endl;
    
    return 0;
}








