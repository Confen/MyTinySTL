#include <iostream>
#include "../memory.h"

using namespace mystl;

class TestClass {
public:
    int value;
    
    TestClass(int v) : value(v) {
        std::cout << "构造 TestClass(" << value << ") 地址: " << this << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass(" << value << ") 地址: " << this << std::endl;
    }
};

void test_pointer_vs_object() {
    std::cout << "=== 指针 vs 对象 的区别 ===" << std::endl;
    
    {
        std::cout << "\n--- 创建智能指针 ---" << std::endl;
        unique_ptr<TestClass> ptr(new TestClass(100));
        std::cout << "智能指针 ptr 的地址: " << &ptr << std::endl;
        std::cout << "ptr 指向的对象地址: " << ptr.get() << std::endl;
        
        std::cout << "\n--- 使用 release() ---" << std::endl;
        TestClass* raw_ptr = ptr.release();
        std::cout << "原始指针 raw_ptr 的地址: " << &raw_ptr << std::endl;
        std::cout << "raw_ptr 指向的对象地址: " << raw_ptr << std::endl;
        std::cout << "智能指针 ptr 现在是否为空: " << (ptr ? "否" : "是") << std::endl;
        
        std::cout << "\n--- 作用域结束 ---" << std::endl;
        // 此时：
        // 1. raw_ptr 变量会被销毁（栈上的指针变量）
        // 2. 但 raw_ptr 指向的对象不会被销毁（堆上的对象）
        // 3. 没有任何析构函数被调用
    }
    
    std::cout << "作用域结束，raw_ptr 变量被销毁，但对象仍然存在！" << std::endl;
}

void test_automatic_destruction() {
    std::cout << "\n=== 自动销毁 vs 手动销毁 ===" << std::endl;
    
    {
        std::cout << "\n--- 栈对象（自动销毁）---" << std::endl;
        TestClass stack_obj(200);  // 栈对象
        std::cout << "栈对象地址: " << &stack_obj << std::endl;
        // 栈对象会在作用域结束时自动调用析构函数
    }
    std::cout << "栈对象自动销毁完成" << std::endl;
    
    {
        std::cout << "\n--- 堆对象（需要手动销毁）---" << std::endl;
        TestClass* heap_obj = new TestClass(300);  // 堆对象
        std::cout << "堆对象地址: " << heap_obj << std::endl;
        // 堆对象不会自动销毁，需要手动 delete
        delete heap_obj;  // 手动销毁
    }
    std::cout << "堆对象手动销毁完成" << std::endl;
}

void test_pointer_lifetime() {
    std::cout << "\n=== 指针变量的生命周期 ===" << std::endl;
    
    {
        std::cout << "\n--- 局部指针变量 ---" << std::endl;
        TestClass* local_ptr = new TestClass(400);
        std::cout << "局部指针 local_ptr 地址: " << &local_ptr << std::endl;
        std::cout << "local_ptr 指向的对象地址: " << local_ptr << std::endl;
        
        // local_ptr 是栈上的变量，会在作用域结束时销毁
        // 但 local_ptr 指向的堆对象不会自动销毁
    }
    std::cout << "局部指针变量被销毁，但堆对象仍然存在！" << std::endl;
}

void test_smart_pointer_mechanism() {
    std::cout << "\n=== 智能指针的自动销毁机制 ===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针自动管理 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(500));
        std::cout << "智能指针 smart_ptr 地址: " << &smart_ptr << std::endl;
        std::cout << "smart_ptr 指向的对象地址: " << smart_ptr.get() << std::endl;
        
        // 智能指针的析构函数会自动调用 delete
        // 这就是为什么智能指针能自动管理内存
    }
    std::cout << "智能指针自动销毁完成" << std::endl;
}

void test_manual_vs_automatic() {
    std::cout << "\n=== 手动管理 vs 自动管理对比 ===" << std::endl;
    
    std::cout << "\n--- 手动管理（原始指针）---" << std::endl;
    {
        TestClass* manual_ptr = new TestClass(600);
        std::cout << "手动创建对象: " << manual_ptr->value << std::endl;
        
        // 必须手动删除
        delete manual_ptr;
        std::cout << "手动删除完成" << std::endl;
    }
    
    std::cout << "\n--- 自动管理（智能指针）---" << std::endl;
    {
        unique_ptr<TestClass> auto_ptr(new TestClass(700));
        std::cout << "自动创建对象: " << auto_ptr->value << std::endl;
        
        // 自动删除，无需手动操作
    }
    std::cout << "自动删除完成" << std::endl;
}

void test_release_ownership_transfer() {
    std::cout << "\n=== release() 的所有权转移 ===" << std::endl;
    
    {
        std::cout << "\n--- 所有权转移过程 ---" << std::endl;
        unique_ptr<TestClass> owner(new TestClass(800));
        std::cout << "初始所有者: " << owner->value << std::endl;
        
        // release() 转移所有权
        TestClass* new_owner = owner.release();
        std::cout << "原所有者是否为空: " << (owner ? "否" : "是") << std::endl;
        std::cout << "新所有者: " << new_owner->value << std::endl;
        
        // 现在 new_owner 负责管理对象
        delete new_owner;
        std::cout << "新所有者手动删除完成" << std::endl;
    }
    std::cout << "所有权转移完成" << std::endl;
}

int main() {
    std::cout << "指针 vs 对象 详解" << std::endl;
    std::cout << "=================" << std::endl;
    
    try {
        test_pointer_vs_object();
        test_automatic_destruction();
        test_pointer_lifetime();
        test_smart_pointer_mechanism();
        test_manual_vs_automatic();
        test_release_ownership_transfer();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

