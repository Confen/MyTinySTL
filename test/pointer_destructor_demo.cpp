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

void test_pointer_vs_object_destructor() {
    std::cout << "=== 指针 vs 对象析构函数 ===" << std::endl;
    
    {
        std::cout << "\n--- 栈对象（自动析构）---" << std::endl;
        TestClass obj(100);  // 栈对象
        std::cout << "obj 类型: " << typeid(obj).name() << std::endl;
        std::cout << "obj 地址: " << &obj << std::endl;
        // 栈对象会在作用域结束时自动调用析构函数
    }
    std::cout << "栈对象自动析构完成" << std::endl;
    
    {
        std::cout << "\n--- 堆对象（手动析构）---" << std::endl;
        TestClass* ptr = new TestClass(200);  // 堆对象
        std::cout << "ptr 类型: " << typeid(ptr).name() << std::endl;
        std::cout << "ptr 地址: " << &ptr << std::endl;
        std::cout << "ptr 指向的对象地址: " << ptr << std::endl;
        
        // 堆对象需要手动调用析构函数
        delete ptr;  // 这会调用析构函数
        std::cout << "堆对象手动析构完成" << std::endl;
    }
}

void test_pointer_lifetime() {
    std::cout << "\n=== 指针生命周期 ===" << std::endl;
    
    {
        std::cout << "\n--- 指针变量生命周期 ---" << std::endl;
        TestClass* ptr = new TestClass(300);
        std::cout << "指针变量 ptr 已创建" << std::endl;
        std::cout << "ptr 指向的对象地址: " << ptr << std::endl;
        
        // 指针变量会在作用域结束时自动销毁
        // 但指针指向的对象不会自动销毁
    }
    std::cout << "指针变量已销毁，但堆对象仍然存在！" << std::endl;
    std::cout << "这就是内存泄漏！" << std::endl;
}

void test_manual_destructor_call() {
    std::cout << "\n=== 手动析构函数调用 ===" << std::endl;
    
    {
        std::cout << "\n--- 手动调用析构函数 ---" << std::endl;
        TestClass* ptr = new TestClass(400);
        std::cout << "对象已创建，值: " << ptr->value << std::endl;
        
        // 手动调用析构函数
        ptr->~TestClass();
        std::cout << "析构函数已手动调用" << std::endl;
        
        // 注意：手动调用析构函数后，对象仍然存在，但处于未定义状态
        // 必须调用 delete 来释放内存
        delete ptr;
        std::cout << "内存已释放" << std::endl;
    }
}

void test_destructor_vs_delete() {
    std::cout << "\n=== 析构函数 vs delete ===" << std::endl;
    
    {
        std::cout << "\n--- 析构函数 vs delete ---" << std::endl;
        TestClass* ptr = new TestClass(500);
        std::cout << "对象已创建，值: " << ptr->value << std::endl;
        
        // delete 会做两件事：
        // 1. 调用析构函数
        // 2. 释放内存
        delete ptr;
        std::cout << "delete 完成（析构函数 + 内存释放）" << std::endl;
    }
}

void test_smart_pointer_destructor() {
    std::cout << "\n=== 智能指针析构函数 ===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针自动析构 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(600));
        std::cout << "智能指针已创建，对象值: " << smart_ptr->value << std::endl;
        
        // 智能指针会在作用域结束时自动调用 delete
        // delete 会调用析构函数并释放内存
    }
    std::cout << "智能指针自动析构完成" << std::endl;
}

void test_destructor_call_timing() {
    std::cout << "\n=== 析构函数调用时机 ===" << std::endl;
    
    std::cout << "\n--- 栈对象析构时机 ---" << std::endl;
    {
        TestClass stack_obj(700);
        std::cout << "栈对象创建完成" << std::endl;
        // 作用域结束时自动调用析构函数
    }
    std::cout << "栈对象析构函数已调用" << std::endl;
    
    std::cout << "\n--- 堆对象析构时机 ---" << std::endl;
    TestClass* heap_ptr = new TestClass(800);
    std::cout << "堆对象创建完成" << std::endl;
    delete heap_ptr;  // 手动调用析构函数
    std::cout << "堆对象析构函数已调用" << std::endl;
    
    std::cout << "\n--- 智能指针析构时机 ---" << std::endl;
    {
        unique_ptr<TestClass> smart_ptr(new TestClass(900));
        std::cout << "智能指针对象创建完成" << std::endl;
        // 作用域结束时自动调用析构函数
    }
    std::cout << "智能指针对象析构函数已调用" << std::endl;
}

void test_destructor_necessity() {
    std::cout << "\n=== 析构函数的必要性 ===" << std::endl;
    
    std::cout << "\n--- 为什么需要析构函数？ ---" << std::endl;
    std::cout << "1. 清理资源（文件、网络连接等）" << std::endl;
    std::cout << "2. 释放动态分配的内存" << std::endl;
    std::cout << "3. 执行必要的清理操作" << std::endl;
    
    std::cout << "\n--- 不调用析构函数的后果 ---" << std::endl;
    std::cout << "1. 内存泄漏" << std::endl;
    std::cout << "2. 资源泄漏" << std::endl;
    std::cout << "3. 程序崩溃" << std::endl;
}

int main() {
    std::cout << "指针析构函数详解" << std::endl;
    std::cout << "================" << std::endl;
    
    try {
        test_pointer_vs_object_destructor();
        test_pointer_lifetime();
        test_manual_destructor_call();
        test_destructor_vs_delete();
        test_smart_pointer_destructor();
        test_destructor_call_timing();
        test_destructor_necessity();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

