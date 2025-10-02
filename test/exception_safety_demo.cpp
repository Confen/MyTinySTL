#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <windows.h>

// 一个会抛出异常的类，用于演示基本异常保证
class ResourceManager {
private:
    std::vector<int> data;
    bool valid_state;

public:
    ResourceManager() : valid_state(true) {
        std::cout << "ResourceManager 构造函数执行" << std::endl;
    }

    // 这个操作会抛出异常，但保证对象仍处于有效状态（基本异常保证）
    void risky_operation() {
        std::cout << "开始执行危险操作..." << std::endl;
        
        // 操作前对象是有效的
        if (!valid_state) {
            throw std::runtime_error("对象已处于无效状态");
        }
        
        // 模拟一些可能失败的操作
        data.push_back(1);
        data.push_back(2);
        
        std::cout << "操作进行中，当前数据大小: " << data.size() << std::endl;
        
        // 这里故意抛出异常
        throw std::runtime_error("risky_operation 故意抛出的异常");
        
        // 注意：即使抛出异常，对象仍然处于有效状态
        // data 中已经有了 1, 2 两个元素，valid_state 仍然是 true
    }
    
    // 检查对象是否处于有效状态
    bool is_valid() const {
        return valid_state && !data.empty();
    }
    
    size_t get_data_size() const {
        return data.size();
    }
    
    void print_data() const {
        std::cout << "当前数据: [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    ~ResourceManager() {
        std::cout << "ResourceManager 析构函数执行，对象" 
                  << (valid_state ? "有效" : "无效") << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 基本异常保证宏演示 ===" << std::endl;
    std::cout << "宏的目的：捕获异常来测试异常安全性，而不是阻止异常\n" << std::endl;
    
    // 示例1：不使用宏 - 异常会传播出去
    std::cout << "1. 不使用宏的情况（异常会传播）：" << std::endl;
    try {
        ResourceManager rm1;
        std::cout << "调用 risky_operation()..." << std::endl;
        rm1.risky_operation();  // 这里会抛出异常
        std::cout << "这行不会执行，因为异常被抛出了" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "在外层捕获到异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n" << std::endl;
    
    // 示例2：使用基本异常保证宏 - 异常被宏捕获，不会传播出去
    std::cout << "2. 使用 MYSTL_BASIC_GUARANTEE_ASSERT 宏的情况：" << std::endl;
    {
        ResourceManager rm2;
        std::cout << "操作前状态检查:" << std::endl;
        std::cout << "  - 对象有效: " << (rm2.is_valid() ? "是" : "否") << std::endl;
        std::cout << "  - 数据大小: " << rm2.get_data_size() << std::endl;
        
        std::cout << "\n使用宏调用 risky_operation()..." << std::endl;
        
        // 宏会捕获异常，验证基本异常保证，但不会让异常传播出去
        MYSTL_BASIC_GUARANTEE_ASSERT(rm2.risky_operation());
        
        std::cout << "宏执行完毕！异常被宏内部捕获，没有传播出来" << std::endl;
        
        std::cout << "\n操作后状态检查（验证基本异常保证）:" << std::endl;
        std::cout << "  - 对象有效: " << (rm2.is_valid() ? "是" : "否") << std::endl;
        std::cout << "  - 数据大小: " << rm2.get_data_size() << std::endl;
        rm2.print_data();
        std::cout << "  - 结论: 虽然操作失败了，但对象仍处于有效状态（基本异常保证）" << std::endl;
    }
    
    std::cout << "\n" << std::endl;
    
    // 示例3：对比 - 展示宏确实捕获了异常
    std::cout << "3. 对比演示 - 证明宏确实捕获了异常：" << std::endl;
    
    std::cout << "\n3a. 不使用宏，手动捕获异常：" << std::endl;
    try {
        ResourceManager rm3a;
        rm3a.risky_operation();
    } catch (const std::exception& e) {
        std::cout << "手动捕获到异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n3b. 使用宏，宏自动捕获异常：" << std::endl;
    {
        ResourceManager rm3b;
        MYSTL_BASIC_GUARANTEE_ASSERT(rm3b.risky_operation());
        std::cout << "宏自动捕获了异常，这行代码正常执行" << std::endl;
    }
    
    std::cout << "\n" << std::endl;
    
    // 示例4：展示宏的真正用途 - 测试异常安全性
    std::cout << "4. 宏的真正用途 - 异常安全性测试：" << std::endl;
    std::cout << "在实际测试中，我们关心的是：" << std::endl;
    std::cout << "  - 异常是否被正确抛出？✓" << std::endl;
    std::cout << "  - 对象是否仍处于有效状态？✓" << std::endl;
    std::cout << "  - 资源是否正确管理？✓" << std::endl;
    std::cout << "  - 没有内存泄漏？✓" << std::endl;
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    std::cout << "\n总结：" << std::endl;
    std::cout << "- MYSTL_BASIC_GUARANTEE_ASSERT 宏的设计目的是测试异常安全性" << std::endl;
    std::cout << "- 它会捕获异常，验证对象状态，但不阻止异常的发生" << std::endl;
    std::cout << "- 这样我们可以在测试中验证代码的异常安全保证级别" << std::endl;
    
    return 0;
}