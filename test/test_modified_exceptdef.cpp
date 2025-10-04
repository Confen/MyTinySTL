#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <windows.h>

// 测试类
class TestObject {
private:
    std::vector<int> data;
    bool valid;
    
public:
    TestObject() : valid(true) {}
    
    bool is_valid() const { return valid; }
    
    std::vector<int> get_state() const { return data; }
    
    void operation_that_throws() {
        throw std::runtime_error("测试异常");
    }
    
    void operation_that_succeeds() {
        data.push_back(1);
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 测试修改后的 exceptdef.h ===" << std::endl;
    
    TestObject obj;
    
    std::cout << "\n1. 测试基本异常保证宏：" << std::endl;
    // 使用改进的宏，需要两个参数
    MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(obj.operation_that_throws(), obj);
    
    std::cout << "\n2. 测试强异常保证宏：" << std::endl;
    // 使用改进的宏，需要两个参数
    MYSTL_STRONG_GUARANTEE_ASSERT(obj.operation_that_throws(), obj);
    
    std::cout << "\n=== 问题分析 ===" << std::endl;
    std::cout << "发现的问题：" << std::endl;
    std::cout << "1. IMPROVED_BASIC_GUARANTEE_ASSERT 需要两个参数 (expr, obj)" << std::endl;
    std::cout << "2. MYSTL_STRONG_GUARANTEE_ASSERT 只接受一个参数，但内部使用了 obj" << std::endl;
    std::cout << "3. 宏名称不一致" << std::endl;
    std::cout << "4. 需要包含 <iostream> 头文件" << std::endl;
    
    return 0;
}
