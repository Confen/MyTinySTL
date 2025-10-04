#include <iostream>
#include "../memory.h"
#include <windows.h> 
using namespace mystl;

class TestClass {
public:
    int value;
    
    TestClass(int v) : value(v) {
        std::cout << "构造 TestClass(" << value << ")" << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass(" << value << ")" << std::endl;
    }
};

void test_release_without_delete() {
    std::cout << "=== 测试 release() 不手动删除 ===" << std::endl;
    
    {
        unique_ptr<TestClass> ptr(new TestClass(100));
        std::cout << "智能指针管理的对象值: " << ptr->value << std::endl;
        
        // release() 释放所有权，但不删除对象
        TestClass* raw_ptr = ptr.release();  
        std::cout << "release() 后智能指针是否为空: " << (ptr ? "否" : "是") << std::endl;
        std::cout << "raw_ptr 指向的对象值: " << raw_ptr->value << std::endl;
        
        // 注意：这里没有手动删除 raw_ptr！
        // 当作用域结束时，智能指针不会删除对象（因为所有权已经释放）
        // raw_ptr 指向的对象永远不会被删除 = 内存泄漏！
    }
    
    std::cout << "作用域结束，但没有析构函数被调用！" << std::endl;
    std::cout << "这就是内存泄漏！" << std::endl;
}

void test_release_with_delete() {
    std::cout << "\n=== 测试 release() 手动删除 ===" << std::endl;
    
    {
        unique_ptr<TestClass> ptr(new TestClass(200));
        std::cout << "智能指针管理的对象值: " << ptr->value << std::endl;
        
        // release() 释放所有权
        TestClass* raw_ptr = ptr.release();
        std::cout << "release() 后智能指针是否为空: " << (ptr ? "否" : "是") << std::endl;
        std::cout << "raw_ptr 指向的对象值: " << raw_ptr->value << std::endl;
        
        // 手动删除 raw_ptr
        delete raw_ptr;
        std::cout << "手动删除 raw_ptr 后" << std::endl;
    }
    
    std::cout << "作用域结束，对象已被正确删除" << std::endl;
}

void test_reset_vs_release_comparison() {
    std::cout << "\n=== reset() vs release() 对比 ===" << std::endl;
    
    // 测试 reset()
    std::cout << "--- 使用 reset() ---" << std::endl;
    {
        unique_ptr<TestClass> ptr1(new TestClass(300));
        std::cout << "reset() 前对象值: " << ptr1->value << std::endl;
        
        ptr1.reset(new TestClass(400));
        std::cout << "reset() 后对象值: " << ptr1->value << std::endl;
        // reset() 会自动删除旧对象，管理新对象
    }
    std::cout << "作用域结束，reset() 自动管理资源" << std::endl;
    
    // 测试 release()
    std::cout << "\n--- 使用 release() ---" << std::endl;
    {
        unique_ptr<TestClass> ptr2(new TestClass(500));
        std::cout << "release() 前对象值: " << ptr2->value << std::endl;
        
        TestClass* raw_ptr = ptr2.release();
        std::cout << "release() 后智能指针是否为空: " << (ptr2 ? "否" : "是") << std::endl;
        std::cout << "raw_ptr 指向的对象值: " << raw_ptr->value << std::endl;
        
        // 必须手动删除
        delete raw_ptr;
        std::cout << "手动删除 raw_ptr" << std::endl;
    }
    std::cout << "作用域结束，release() 需要手动管理资源" << std::endl;
}

void test_common_release_use_cases() {
    std::cout << "\n=== release() 常见使用场景 ===" << std::endl;
    
    // 场景1：传递给需要原始指针的函数
    std::cout << "--- 场景1：传递给需要原始指针的函数 ---" << std::endl;
    {
        unique_ptr<TestClass> ptr(new TestClass(600));
        
        // 假设有一个函数需要原始指针
        auto process_raw_pointer = [](TestClass* p) {
            if (p) {
                std::cout << "处理原始指针，对象值: " << p->value << std::endl;
                delete p;  // 函数负责删除
            }
        };
        
        // 使用 release() 转移所有权给函数
        process_raw_pointer(ptr.release());
        std::cout << "函数处理完成，智能指针已为空" << std::endl;
    }
    
    // 场景2：返回原始指针给调用者
    std::cout << "\n--- 场景2：返回原始指针给调用者 ---" << std::endl;
    {
        auto create_and_release = []() -> TestClass* {
            unique_ptr<TestClass> ptr(new TestClass(700));
            std::cout << "创建对象，值: " << ptr->value << std::endl;
            return ptr.release();  // 返回原始指针，调用者负责删除
        };
        
        TestClass* returned_ptr = create_and_release();
        std::cout << "获得返回的原始指针，对象值: " << returned_ptr->value << std::endl;
        
        // 调用者负责删除
        delete returned_ptr;
        std::cout << "调用者删除返回的指针" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "MyTinySTL release() 方法详解" << std::endl;
    std::cout << "============================" << std::endl;
    
    try {
        test_release_without_delete();
        test_release_with_delete();
        test_reset_vs_release_comparison();
        test_common_release_use_cases();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}






