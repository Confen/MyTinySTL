#include <iostream>
#include <stdexcept>
#include <vector>
#include <windows.h>

// 测试类 - 可能处于无效状态
class TestObject {
private:
    std::vector<int> data;
    bool valid;
    bool initialized;
    
public:
    // 默认构造 - 有效状态
    TestObject() : valid(true), initialized(true) {
        std::cout << "构造有效对象" << std::endl;
    }
    
    // 特殊构造 - 无效状态
    explicit TestObject(bool make_invalid) : valid(!make_invalid), initialized(true) {
        if (make_invalid) {
            std::cout << "构造无效对象" << std::endl;
        }
    }
    
    // 未初始化状态
    static TestObject create_uninitialized() {
        TestObject obj;
        obj.initialized = false;
        obj.valid = false;
        std::cout << "创建未初始化对象" << std::endl;
        return obj;
    }
    
    bool is_valid() const { 
        return valid && initialized; 
    }
    
    std::vector<int> get_state() const { 
        if (!initialized) {
            return {}; // 未初始化对象返回空状态
        }
        return data; 
    }
    
    // 在无效对象上的操作
    void operation_on_invalid() {
        if (!is_valid()) {
            std::cout << "在无效对象上执行操作" << std::endl;
            throw std::runtime_error("无法在无效对象上执行操作");
        }
        data.push_back(1);
    }
    
    // 尝试修复无效对象的操作
    void try_to_fix() {
        std::cout << "尝试修复对象..." << std::endl;
        if (!initialized) {
            initialized = true;
            valid = true;
            data.clear();
            std::cout << "对象已修复" << std::endl;
        } else {
            throw std::runtime_error("修复失败");
        }
    }
    
    void print_state() const {
        std::cout << "对象状态: ";
        if (!initialized) {
            std::cout << "[未初始化]";
        } else {
            std::cout << "[";
            for (size_t i = 0; i < data.size(); ++i) {
                std::cout << data[i];
                if (i < data.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        }
        std::cout << ", 有效性: " << (is_valid() ? "有效" : "无效") << std::endl;
    }
};

// 改进的异常安全宏 - 处理无效对象的情况
#define SMART_BASIC_GUARANTEE_ASSERT(expr, obj) \
    do { \
        bool was_valid_before = (obj).is_valid(); \
        std::cout << "操作前对象" << (was_valid_before ? "有效" : "无效") << std::endl; \
        try { \
            expr; \
        } catch (...) { \
            bool is_valid_after = (obj).is_valid(); \
            std::cout << "操作后对象" << (is_valid_after ? "有效" : "无效") << std::endl; \
            \
            if (!was_valid_before) { \
                std::cout << "⚠️  对象最初就无效，基本异常保证的含义：" << std::endl; \
                std::cout << "   - 如果操作前无效，操作后可以仍然无效" << std::endl; \
                std::cout << "   - 或者操作可能尝试修复对象使其变为有效" << std::endl; \
                std::cout << "   - 关键是不能让情况变得更糟" << std::endl; \
            } else { \
                if (is_valid_after) { \
                    std::cout << "✅ 基本异常保证成功：有效对象仍然有效" << std::endl; \
                } else { \
                    std::cout << "❌ 基本异常保证失败：有效对象变为无效" << std::endl; \
                } \
            } \
        } \
    } while(0)

#define SMART_STRONG_GUARANTEE_ASSERT(expr, obj) \
    do { \
        bool was_valid_before = (obj).is_valid(); \
        auto state_before = (obj).get_state(); \
        std::cout << "操作前对象" << (was_valid_before ? "有效" : "无效") << std::endl; \
        try { \
            expr; \
        } catch (...) { \
            bool is_valid_after = (obj).is_valid(); \
            auto state_after = (obj).get_state(); \
            std::cout << "操作后对象" << (is_valid_after ? "有效" : "无效") << std::endl; \
            \
            if (!was_valid_before) { \
                std::cout << "⚠️  对象最初就无效，强异常保证的含义：" << std::endl; \
                std::cout << "   - 无效对象的状态应该保持不变" << std::endl; \
                std::cout << "   - 不应该让无效对象变得'更加无效'" << std::endl; \
                if (state_before == state_after && was_valid_before == is_valid_after) { \
                    std::cout << "✅ 强异常保证成功：无效对象状态未改变" << std::endl; \
                } else { \
                    std::cout << "❌ 强异常保证失败：无效对象状态发生变化" << std::endl; \
                } \
            } else { \
                if (state_before == state_after && is_valid_after) { \
                    std::cout << "✅ 强异常保证成功：有效对象状态完全不变" << std::endl; \
                } else { \
                    std::cout << "❌ 强异常保证失败：有效对象状态发生变化" << std::endl; \
                } \
            } \
        } \
    } while(0)

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 处理最初就无效的对象 ===" << std::endl;
    
    // 测试1：有效对象上的操作
    std::cout << "\n1. 测试有效对象：" << std::endl;
    TestObject valid_obj;
    valid_obj.print_state();
    SMART_BASIC_GUARANTEE_ASSERT(valid_obj.operation_on_invalid(), valid_obj);
    valid_obj.print_state();
    
    // 测试2：无效对象上的操作
    std::cout << "\n2. 测试无效对象：" << std::endl;
    TestObject invalid_obj(true);  // 构造无效对象
    invalid_obj.print_state();
    SMART_BASIC_GUARANTEE_ASSERT(invalid_obj.operation_on_invalid(), invalid_obj);
    invalid_obj.print_state();
    
    // 测试3：未初始化对象
    std::cout << "\n3. 测试未初始化对象：" << std::endl;
    TestObject uninit_obj = TestObject::create_uninitialized();
    uninit_obj.print_state();
    SMART_STRONG_GUARANTEE_ASSERT(uninit_obj.operation_on_invalid(), uninit_obj);
    uninit_obj.print_state();
    
    // 测试4：尝试修复无效对象
    std::cout << "\n4. 测试修复操作：" << std::endl;
    TestObject broken_obj = TestObject::create_uninitialized();
    broken_obj.print_state();
    SMART_BASIC_GUARANTEE_ASSERT(broken_obj.try_to_fix(), broken_obj);
    broken_obj.print_state();
    
    std::cout << "\n=== 关键理解 ===" << std::endl;
    std::cout << "当对象最初就无效时：" << std::endl;
    
    std::cout << "\n基本异常保证的含义：" << std::endl;
    std::cout << "✓ 不会让情况变得更糟" << std::endl;
    std::cout << "✓ 无效对象可以保持无效，或者被修复为有效" << std::endl;
    std::cout << "✓ 不会造成资源泄漏或崩溃" << std::endl;
    std::cout << "✗ 不能让有效对象变为无效" << std::endl;
    
    std::cout << "\n强异常保证的含义：" << std::endl;
    std::cout << "✓ 对象状态完全不变（包括无效状态）" << std::endl;
    std::cout << "✓ 无效对象应该保持原样的无效" << std::endl;
    std::cout << "✓ 操作要么完全成功，要么完全失败" << std::endl;
    std::cout << "✗ 不允许任何状态改变" << std::endl;
    
    std::cout << "\n实际应用中的考虑：" << std::endl;
    std::cout << "1. 需要明确定义什么是'有效状态'" << std::endl;
    std::cout << "2. 对于无效对象，操作可能应该立即失败" << std::endl;
    std::cout << "3. 某些操作可能专门用于修复无效对象" << std::endl;
    std::cout << "4. 异常安全保证的验证需要考虑初始状态" << std::endl;
    
    return 0;
}








