#include "../exceptdef.h"
#include <iostream>

int main() {
    std::cout << "测试宏定义" << std::endl;
    
    // 测试基本异常保证宏
    MYSTL_BASIC_GUARANTEE_ASSERT(throw std::runtime_error("test"));
    
    return 0;
}







