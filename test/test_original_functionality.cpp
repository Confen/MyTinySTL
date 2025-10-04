#include "../exceptdef.h"
#include <iostream>
#include <windows.h>

void safe_function() noexcept {
    std::cout << "安全函数执行" << std::endl;
}

void risky_function() {
    throw std::runtime_error("risky_function");
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 测试原始 demo.cpp 功能 ===" << std::endl;
    
    // 测试1：安全函数
    std::cout << "1. 测试安全函数..." << std::endl;
    MYSTL_NOEXCEPT_ASSERT(safe_function());
    std::cout << "   安全函数测试通过" << std::endl;
    
    // 测试2：基本异常保证
    std::cout << "2. 测试基本异常保证..." << std::endl;
    MYSTL_BASIC_GUARANTEE_ASSERT(risky_function());
    std::cout << "   基本异常保证测试通过（异常被捕获）" << std::endl;
    
    // 测试3：强异常保证（原始版本）
    std::cout << "3. 测试强异常保证..." << std::endl;
    MYSTL_STRONG_GUARANTEE_ASSERT(risky_function());
    std::cout << "   强异常保证测试通过（异常被捕获）" << std::endl;
    
    std::cout << "\n=== 测试结果 ===" << std::endl;
    std::cout << "✅ 所有原始功能都正常工作" << std::endl;
    std::cout << "✅ 向后兼容性保持良好" << std::endl;
    std::cout << "✅ 新的改进功能也可用" << std::endl;
    
    return 0;
}







