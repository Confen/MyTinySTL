#include <iostream>
#include <vector>
#include <windows.h>

class TestObject {
private:
    std::vector<int> data;
    bool valid;
    bool initialized;
    
public:
    // 默认构造 - 有效状态
    TestObject() : valid(true), initialized(true) {
        std::cout << "默认构造函数执行" << std::endl;
        std::cout << "  data.size() = " << data.size() << std::endl;
        std::cout << "  data.capacity() = " << data.capacity() << std::endl;
        std::cout << "  valid = " << valid << std::endl;
        std::cout << "  initialized = " << initialized << std::endl;
    }
    
    // 未初始化状态
    static TestObject create_uninitialized() {
        std::cout << "\n=== create_uninitialized() 执行过程 ===" << std::endl;
        
        std::cout << "1. 调用默认构造函数创建 obj" << std::endl;
        TestObject obj;  // 这里会调用默认构造函数
        
        std::cout << "\n2. 修改 obj 的状态" << std::endl;
        std::cout << "  修改前: initialized=" << obj.initialized << ", valid=" << obj.valid << std::endl;
        std::cout << "  修改前: data.size()=" << obj.data.size() << std::endl;
        
        obj.initialized = false;
        obj.valid = false;
        
        std::cout << "  修改后: initialized=" << obj.initialized << ", valid=" << obj.valid << std::endl;
        std::cout << "  修改后: data.size()=" << obj.data.size() << std::endl;
        std::cout << "  注意：data 本身没有被修改，只是标志位改变了！" << std::endl;
        
        std::cout << "\n3. 返回修改后的对象" << std::endl;
        return obj;
    }
    
    bool is_valid() const { 
        return valid && initialized; 
    }
    
    std::vector<int> get_state() const { 
        if (!initialized) {
            std::cout << "get_state(): 对象未初始化，返回空 vector" << std::endl;
            return {}; // 返回空的 vector
        }
        std::cout << "get_state(): 对象已初始化，返回实际 data" << std::endl;
        return data; 
    }
    
    void print_detailed_state() const {
        std::cout << "=== 详细状态信息 ===" << std::endl;
        std::cout << "initialized: " << initialized << std::endl;
        std::cout << "valid: " << valid << std::endl;
        std::cout << "is_valid(): " << is_valid() << std::endl;
        
        std::cout << "实际的 data 成员变量:" << std::endl;
        std::cout << "  data.size(): " << data.size() << std::endl;
        std::cout << "  data.capacity(): " << data.capacity() << std::endl;
        std::cout << "  data 内容: [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        std::cout << "get_state() 返回的内容:" << std::endl;
        auto state = get_state();
        std::cout << "  返回的 vector.size(): " << state.size() << std::endl;
        std::cout << "  返回的 vector 内容: [";
        for (size_t i = 0; i < state.size(); ++i) {
            std::cout << state[i];
            if (i < state.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        std::cout << "print_state() 显示的内容:" << std::endl;
        if (!initialized) {
            std::cout << "  显示: [未初始化]（因为 initialized=false）" << std::endl;
        } else {
            std::cout << "  显示: 实际 data 内容" << std::endl;
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
    
    // 添加一些数据用于测试
    void add_test_data() {
        data.push_back(100);
        data.push_back(200);
        std::cout << "添加测试数据: [100, 200]" << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 调查 broken_obj 的 data 到底是什么 ===" << std::endl;
    
    // 创建一个 "broken" 对象
    std::cout << "\n创建 broken_obj:" << std::endl;
    TestObject broken_obj = TestObject::create_uninitialized();
    
    std::cout << "\n调用 broken_obj.print_state():" << std::endl;
    broken_obj.print_state();
    
    std::cout << "\n详细分析 broken_obj:" << std::endl;
    broken_obj.print_detailed_state();
    
    std::cout << "\n=== 对比：先添加数据再设为未初始化 ===" << std::endl;
    TestObject test_obj;
    test_obj.add_test_data();
    std::cout << "\n添加数据后的状态:" << std::endl;
    test_obj.print_detailed_state();
    
    // 手动模拟 create_uninitialized 的过程
    std::cout << "\n手动设置为未初始化:" << std::endl;
    // 注意：这里我们不能直接访问私有成员，但可以通过分析理解过程
    
    std::cout << "\n=== 关键理解 ===" << std::endl;
    std::cout << "1. broken_obj 的实际 data 成员变量是一个空的 std::vector<int>" << std::endl;
    std::cout << "2. 因为它是通过默认构造函数创建的，data 被初始化为空 vector" << std::endl;
    std::cout << "3. create_uninitialized() 只是改变了 initialized 和 valid 标志" << std::endl;
    std::cout << "4. print_state() 显示 [未初始化] 是因为 initialized=false" << std::endl;
    std::cout << "5. get_state() 返回空 vector 是因为检查到 initialized=false" << std::endl;
    std::cout << "6. 但实际的 data 成员变量本身就是空的，没有任何元素" << std::endl;
    
    return 0;
}








