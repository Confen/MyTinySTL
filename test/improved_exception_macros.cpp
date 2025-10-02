#include <iostream>
#include <stdexcept>
#include <vector>
#include <windows.h>

// 改进的异常安全宏设计

// 基本异常保证宏 - 只检查对象是否仍然有效
#define IMPROVED_BASIC_GUARANTEE_ASSERT(expr, obj) \
    do { \
        bool was_valid_before = (obj).is_valid(); \
        try { \
            expr; \
        } catch (...) { \
            /* 基本异常保证：检查对象是否仍处于有效状态 */ \
            if (!(obj).is_valid()) { \
                std::cout << "❌ 基本异常保证失败：对象处于无效状态" << std::endl; \
            } else { \
                std::cout << "✅ 基本异常保证成功：对象仍处于有效状态" << std::endl; \
            } \
        } \
    } while(0)

// 强异常保证宏 - 检查对象状态是否完全不变
#define IMPROVED_STRONG_GUARANTEE_ASSERT(expr, obj) \
    do { \
        auto state_before = (obj).get_state(); \
        try { \
            expr; \
        } catch (...) { \
            /* 强异常保证：检查对象状态是否完全不变 */ \
            auto state_after = (obj).get_state(); \
            if (state_before != state_after) { \
                std::cout << "❌ 强异常保证失败：对象状态发生了变化" << std::endl; \
            } else { \
                std::cout << "✅ 强异常保证成功：对象状态完全不变" << std::endl; \
            } \
        } \
    } while(0)

// 测试类 - 支持状态检查
class TestObject {
private:
    std::vector<int> data;
    bool valid;
    
public:
    TestObject() : valid(true) {}
    
    bool is_valid() const { return valid && data.size() >= 0; }
    
    std::vector<int> get_state() const { return data; }
    
    // 基本异常保证操作 - 可能改变状态但保持有效
    void basic_guarantee_operation() {
        data.push_back(1);  // 状态改变
        data.push_back(2);  // 状态改变
        throw std::runtime_error("操作失败，但对象仍有效");
    }
    
    // 强异常保证操作 - 要么成功，要么状态不变
    void strong_guarantee_operation() {
        std::vector<int> temp = data;  // 备份状态
        temp.push_back(3);
        temp.push_back(4);
        
        // 模拟失败条件
        throw std::runtime_error("操作失败，状态回滚");
        
        // 如果没有异常，才更新状态
        data = std::move(temp);
    }
    
    void print_state() const {
        std::cout << "对象状态: [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "], 有效性: " << (valid ? "有效" : "无效") << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 改进的异常安全宏对比 ===" << std::endl;
    
    // 测试基本异常保证
    std::cout << "\n1. 测试基本异常保证：" << std::endl;
    TestObject obj1;
    std::cout << "操作前：";
    obj1.print_state();
    
    IMPROVED_BASIC_GUARANTEE_ASSERT(obj1.basic_guarantee_operation(), obj1);
    
    std::cout << "操作后：";
    obj1.print_state();
    std::cout << "分析：对象状态改变了，但仍然有效（符合基本异常保证）" << std::endl;
    
    // 测试强异常保证
    std::cout << "\n2. 测试强异常保证：" << std::endl;
    TestObject obj2;
    std::cout << "操作前：";
    obj2.print_state();
    
    IMPROVED_STRONG_GUARANTEE_ASSERT(obj2.strong_guarantee_operation(), obj2);
    
    std::cout << "操作后：";
    obj2.print_state();
    std::cout << "分析：对象状态完全没有改变（符合强异常保证）" << std::endl;
    
    std::cout << "\n=== 关键区别总结 ===" << std::endl;
    std::cout << "基本异常保证宏：" << std::endl;
    std::cout << "- 只检查对象是否仍然有效" << std::endl;
    std::cout << "- 允许状态发生变化" << std::endl;
    std::cout << "- 确保没有资源泄漏" << std::endl;
    
    std::cout << "\n强异常保证宏：" << std::endl;
    std::cout << "- 检查对象状态是否完全不变" << std::endl;
    std::cout << "- 要求操作具有事务性（要么成功，要么回滚）" << std::endl;
    std::cout << "- 更严格的异常安全要求" << std::endl;
    
    std::cout << "\n当前 MyTinySTL 的实现问题：" << std::endl;
    std::cout << "❌ 两个宏完全相同，没有体现不同的异常安全级别" << std::endl;
    std::cout << "❌ 只是简单地捕获异常，没有进行实际的安全性验证" << std::endl;
    std::cout << "✅ 应该根据不同的异常安全级别进行相应的状态检查" << std::endl;
    
    return 0;
}
