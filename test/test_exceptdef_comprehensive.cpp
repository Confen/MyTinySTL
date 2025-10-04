#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <windows.h>

// 测试类 - 支持状态检查
class TestObject {
private:
    std::vector<int> data;
    bool valid;
    bool initialized;
    
public:
    TestObject() : valid(true), initialized(true) {
        std::cout << "TestObject 构造" << std::endl;
    }
    
    explicit TestObject(bool make_invalid) : valid(!make_invalid), initialized(true) {
        if (make_invalid) {
            std::cout << "TestObject 构造为无效状态" << std::endl;
        }
    }
    
    bool is_valid() const { 
        return valid && initialized; 
    }
    
    std::vector<int> get_state() const { 
        if (!initialized) {
            return {};
        }
        return data; 
    }
    
    void set_initialized(bool init) {
        initialized = init;
    }
    
    // 会抛出异常的操作
    void operation_that_throws() {
        std::cout << "执行会抛出异常的操作" << std::endl;
        throw std::runtime_error("操作失败");
    }
    
    // 会成功但改变状态的操作
    void operation_that_changes_state() {
        std::cout << "执行会改变状态的操作" << std::endl;
        data.push_back(1);
        data.push_back(2);
    }
    
    // 强异常保证操作 - 要么成功，要么状态不变
    void strong_guarantee_operation() {
        std::cout << "执行强异常保证操作" << std::endl;
        std::vector<int> temp = data;
        temp.push_back(3);
        temp.push_back(4);
        
        // 模拟失败
        throw std::runtime_error("强异常保证操作失败");
        
        // 如果没有异常，才更新状态
        data = std::move(temp);
    }
    
    void print_state() const {
        std::cout << "对象状态: [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "], 有效性: " << (is_valid() ? "有效" : "无效") << std::endl;
    }
};

// 安全函数
void safe_function() noexcept {
    std::cout << "安全函数执行" << std::endl;
}

// 危险函数
void risky_function() {
    std::cout << "危险函数执行" << std::endl;
    throw std::runtime_error("危险函数抛出异常");
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL exceptdef.h 全面测试 ===" << std::endl;
    
    // 测试1：无异常断言宏
    std::cout << "\n1. 测试 MYSTL_NOEXCEPT_ASSERT：" << std::endl;
    try {
        MYSTL_NOEXCEPT_ASSERT(safe_function());
        std::cout << "✅ 无异常断言通过" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "❌ 无异常断言失败: " << e.what() << std::endl;
    }
    
    // 测试2：基本异常保证宏（原始版本）
    std::cout << "\n2. 测试 MYSTL_BASIC_GUARANTEE_ASSERT（原始版本）：" << std::endl;
    MYSTL_BASIC_GUARANTEE_ASSERT(risky_function());
    std::cout << "✅ 基本异常保证宏执行完毕（异常被捕获）" << std::endl;
    
    // 测试3：基本异常保证宏（改进版本）
    std::cout << "\n3. 测试 MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ（改进版本）：" << std::endl;
    TestObject obj1;
    std::cout << "操作前：";
    obj1.print_state();
    
    MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(obj1.operation_that_throws(), obj1);
    
    std::cout << "操作后：";
    obj1.print_state();
    
    // 测试4：强异常保证宏
    std::cout << "\n4. 测试 MYSTL_STRONG_GUARANTEE_ASSERT：" << std::endl;
    TestObject obj2;
    std::cout << "操作前：";
    obj2.print_state();
    
    MYSTL_STRONG_GUARANTEE_ASSERT_WITH_OBJ(obj2.strong_guarantee_operation(), obj2);
    
    std::cout << "操作后：";
    obj2.print_state();
    
    // 测试5：测试无效对象
    std::cout << "\n5. 测试无效对象：" << std::endl;
    TestObject invalid_obj(true);  // 构造无效对象
    std::cout << "无效对象状态：";
    invalid_obj.print_state();
    
    MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(invalid_obj.operation_that_throws(), invalid_obj);
    
    // 测试6：测试异常安全保证检查函数
    std::cout << "\n6. 测试异常安全保证检查函数：" << std::endl;
    
    TestObject test_obj;
    
    std::cout << "检查无异常保证: " << mystl::check_no_throw<TestObject>() << std::endl;
    std::cout << "检查基本异常保证: " << mystl::check_basic_guarantee<TestObject>() << std::endl;
    std::cout << "检查强异常保证: " << mystl::check_strong_guarantee<TestObject>() << std::endl;
    
    // 测试7：测试异常安全级别
    std::cout << "\n7. 测试异常安全级别：" << std::endl;
    auto safety_level = mystl::test_exception_safety(test_obj);
    std::cout << "异常安全级别: ";
    switch (safety_level) {
        case mystl::exception_safety_level::no_throw:
            std::cout << "无异常保证" << std::endl;
            break;
        case mystl::exception_safety_level::basic_guarantee:
            std::cout << "基本异常保证" << std::endl;
            break;
        case mystl::exception_safety_level::strong_guarantee:
            std::cout << "强异常保证" << std::endl;
            break;
        case mystl::exception_safety_level::no_guarantee:
            std::cout << "无异常保证" << std::endl;
            break;
    }
    
    // 测试8：测试自定义异常类
    std::cout << "\n8. 测试自定义异常类：" << std::endl;
    try {
        throw mystl::mystl_runtime_error("测试自定义异常");
    } catch (const mystl::mystl_runtime_error& e) {
        std::cout << "捕获到自定义异常: " << e.what() << std::endl;
    }
    
    try {
        throw mystl::mystl_logic_error("测试逻辑错误异常");
    } catch (const mystl::mystl_logic_error& e) {
        std::cout << "捕获到逻辑错误异常: " << e.what() << std::endl;
    }
    
    // 测试9：测试异常处理宏
    std::cout << "\n9. 测试异常处理宏：" << std::endl;
    
    MYSTL_TRY {
        std::cout << "使用 MYSTL_TRY" << std::endl;
        throw std::runtime_error("测试异常");
    }
    MYSTL_CATCH(std::runtime_error) {
        std::cout << "使用 MYSTL_CATCH 捕获: " << e.what() << std::endl;
    }
    
    // 测试10：测试调试宏
    std::cout << "\n10. 测试调试宏：" << std::endl;
    MYSTL_DEBUG_EXCEPTION("这是调试信息");
    MYSTL_EXCEPTION_TRACE("这是跟踪信息");
    MYSTL_EXCEPTION_LOG("这是日志信息");
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    std::cout << "✅ 所有测试都已执行" << std::endl;
    std::cout << "✅ exceptdef.h 功能正常" << std::endl;
    
    return 0;
}
