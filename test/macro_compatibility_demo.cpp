#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <windows.h>

// 测试类
class SimpleObject {
private:
    std::vector<int> data;
    
public:
    SimpleObject() = default;
    
    void operation_that_throws() {
        throw std::runtime_error("操作失败");
    }
    
    void operation_that_succeeds() {
        data.push_back(1);
    }
    
    size_t size() const { return data.size(); }
};

// 支持状态检查的复杂对象
class AdvancedObject {
private:
    std::vector<int> data;
    bool valid;
    
public:
    AdvancedObject() : valid(true) {}
    
    bool is_valid() const { return valid; }
    
    std::vector<int> get_state() const { return data; }
    
    void operation_that_throws() {
        throw std::runtime_error("操作失败");
    }
    
    void operation_that_changes_state() {
        data.push_back(1);
        data.push_back(2);
    }
    
    void print_state() const {
        std::cout << "状态: [";
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
    
    std::cout << "=== 宏兼容性设计演示 ===" << std::endl;
    
    std::cout << "\n📋 设计原则：" << std::endl;
    std::cout << "1. 原始版本：简单易用，向后兼容" << std::endl;
    std::cout << "2. 改进版本：功能强大，需要更多参数" << std::endl;
    std::cout << "3. 用户可以根据需求选择合适的版本" << std::endl;
    
    // 场景1：简单对象 + 原始宏
    std::cout << "\n🔹 场景1：简单对象使用原始宏" << std::endl;
    SimpleObject simple_obj;
    std::cout << "操作前大小: " << simple_obj.size() << std::endl;
    
    // 使用原始宏 - 简单易用
    MYSTL_BASIC_GUARANTEE_ASSERT(simple_obj.operation_that_throws());
    std::cout << "操作后大小: " << simple_obj.size() << std::endl;
    std::cout << "✅ 原始宏工作正常，异常被捕获" << std::endl;
    
    // 场景2：复杂对象 + 改进宏
    std::cout << "\n🔹 场景2：复杂对象使用改进宏" << std::endl;
    AdvancedObject advanced_obj;
    std::cout << "操作前：";
    advanced_obj.print_state();
    
    // 使用改进宏 - 提供详细验证
    MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(advanced_obj.operation_that_throws(), advanced_obj);
    
    std::cout << "操作后：";
    advanced_obj.print_state();
    
    // 场景3：强异常保证对比
    std::cout << "\n🔹 场景3：强异常保证对比" << std::endl;
    
    std::cout << "原始版本（简单）：" << std::endl;
    MYSTL_STRONG_GUARANTEE_ASSERT(simple_obj.operation_that_throws());
    std::cout << "✅ 异常被捕获，无额外信息" << std::endl;
    
    std::cout << "\n改进版本（详细）：" << std::endl;
    MYSTL_STRONG_GUARANTEE_ASSERT_WITH_OBJ(advanced_obj.operation_that_throws(), advanced_obj);
    std::cout << "✅ 异常被捕获，提供详细状态验证" << std::endl;
    
    // 场景4：迁移路径演示
    std::cout << "\n🔹 场景4：代码迁移路径" << std::endl;
    
    std::cout << "阶段1 - 使用原始宏：" << std::endl;
    std::cout << "  MYSTL_BASIC_GUARANTEE_ASSERT(expr);" << std::endl;
    std::cout << "  // 简单，兼容性好" << std::endl;
    
    std::cout << "\n阶段2 - 升级到改进宏：" << std::endl;
    std::cout << "  MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(expr, obj);" << std::endl;
    std::cout << "  // 功能更强，需要对象支持状态检查" << std::endl;
    
    std::cout << "\n阶段3 - 完全迁移：" << std::endl;
    std::cout << "  // 可以逐步替换，不需要一次性修改所有代码" << std::endl;
    
    // 场景5：不同使用场景
    std::cout << "\n🔹 场景5：不同使用场景" << std::endl;
    
    std::cout << "快速原型开发：" << std::endl;
    std::cout << "  ✅ 使用原始宏 - 简单快速" << std::endl;
    
    std::cout << "生产环境测试：" << std::endl;
    std::cout << "  ✅ 使用改进宏 - 详细验证" << std::endl;
    
    std::cout << "遗留代码维护：" << std::endl;
    std::cout << "  ✅ 继续使用原始宏 - 不破坏现有功能" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 双版本设计的优势：" << std::endl;
    std::cout << "1. ✅ 向后兼容 - 现有代码继续工作" << std::endl;
    std::cout << "2. ✅ 渐进升级 - 可以逐步迁移" << std::endl;
    std::cout << "3. ✅ 灵活选择 - 根据需求选择合适的版本" << std::endl;
    std::cout << "4. ✅ 降低风险 - 不会因为升级而破坏现有功能" << std::endl;
    std::cout << "5. ✅ 学习曲线 - 用户可以逐步学习新功能" << std::endl;
    
    std::cout << "\n📚 这是 C++ 标准库的常见模式：" << std::endl;
    std::cout << "- std::vector 有 push_back() 和 emplace_back()" << std::endl;
    std::cout << "- std::string 有 find() 和 find_first_of()" << std::endl;
    std::cout << "- std::algorithm 有 sort() 和 stable_sort()" << std::endl;
    std::cout << "- 都是为了提供不同层次的功能" << std::endl;
    
    return 0;
}







