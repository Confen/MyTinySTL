#include <iostream>
#include <typeinfo>
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

void test_smart_vs_raw_pointer() {
    std::cout << "=== 智能指针 vs 原始指针 ===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针阶段 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(100));
        std::cout << "smart_ptr 类型: " << typeid(smart_ptr).name() << std::endl;
        std::cout << "smart_ptr 大小: " << sizeof(smart_ptr) << " 字节" << std::endl;
        std::cout << "smart_ptr 是否为空: " << (smart_ptr ? "否" : "是") << std::endl;
        std::cout << "smart_ptr 对象值: " << smart_ptr->value << std::endl;
        
        std::cout << "\n--- 使用 release() 后 ---" << std::endl;
        TestClass* raw_ptr = smart_ptr.release();
        std::cout << "raw_ptr 类型: " << typeid(raw_ptr).name() << std::endl;
        std::cout << "raw_ptr 大小: " << sizeof(raw_ptr) << " 字节" << std::endl;
        std::cout << "smart_ptr 是否为空: " << (smart_ptr ? "否" : "是") << std::endl;
        std::cout << "raw_ptr 是否为空: " << (raw_ptr ? "否" : "是") << std::endl;
        std::cout << "raw_ptr 对象值: " << raw_ptr->value << std::endl;
        
        // 手动删除
        delete raw_ptr;
    }
}

void test_pointer_capabilities() {
    std::cout << "\n=== 指针功能对比 ===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针功能 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(200));
        
        // 智能指针的功能
        std::cout << "智能指针功能:" << std::endl;
        std::cout << "1. 自动内存管理: 是" << std::endl;
        std::cout << "2. 自动析构: 是" << std::endl;
        std::cout << "3. 异常安全: 是" << std::endl;
        std::cout << "4. 移动语义: 是" << std::endl;
        std::cout << "5. 空指针检查: 是" << std::endl;
        
        std::cout << "\n--- 原始指针功能 ---" << std::endl;
        TestClass* raw_ptr = smart_ptr.release();
        
        // 原始指针的功能
        std::cout << "原始指针功能:" << std::endl;
        std::cout << "1. 自动内存管理: 否" << std::endl;
        std::cout << "2. 自动析构: 否" << std::endl;
        std::cout << "3. 异常安全: 否" << std::endl;
        std::cout << "4. 移动语义: 否" << std::endl;
        std::cout << "5. 空指针检查: 否" << std::endl;
        
        delete raw_ptr;
    }
}

void test_memory_management() {
    std::cout << "\n=== 内存管理对比 ===" << std::endl;
    
    std::cout << "\n--- 智能指针内存管理 ---" << std::endl;
    {
        unique_ptr<TestClass> smart_ptr(new TestClass(300));
        std::cout << "智能指针创建对象" << std::endl;
        // 智能指针会自动管理内存
    }
    std::cout << "智能指针自动释放内存" << std::endl;
    
    std::cout << "\n--- 原始指针内存管理 ---" << std::endl;
    TestClass* raw_ptr = new TestClass(400);
    std::cout << "原始指针创建对象" << std::endl;
    delete raw_ptr;  // 必须手动释放
    std::cout << "原始指针手动释放内存" << std::endl;
}

void test_exception_safety() {
    std::cout << "\n=== 异常安全性对比 ===" << std::endl;
    
    std::cout << "\n--- 智能指针异常安全 ---" << std::endl;
    try {
        unique_ptr<TestClass> smart_ptr(new TestClass(500));
        std::cout << "智能指针创建成功" << std::endl;
        throw std::runtime_error("测试异常");
    } catch (const std::exception& e) {
        std::cout << "异常被捕获: " << e.what() << std::endl;
        std::cout << "智能指针自动释放内存" << std::endl;
    }
    
    std::cout << "\n--- 原始指针异常安全 ---" << std::endl;
    TestClass* raw_ptr = nullptr;
    try {
        raw_ptr = new TestClass(600);
        std::cout << "原始指针创建成功" << std::endl;
        throw std::runtime_error("测试异常");
    } catch (const std::exception& e) {
        std::cout << "异常被捕获: " << e.what() << std::endl;
        std::cout << "原始指针需要手动释放内存" << std::endl;
        if (raw_ptr) {
            delete raw_ptr;
            std::cout << "原始指针手动释放完成" << std::endl;
        }
    }
}

void test_pointer_operations() {
    std::cout << "\n=== 指针操作对比 ===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针操作 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(700));
        
        // 智能指针操作
        std::cout << "智能指针操作:" << std::endl;
        std::cout << "1. 解引用: " << (*smart_ptr).value << std::endl;
        std::cout << "2. 箭头操作: " << smart_ptr->value << std::endl;
        std::cout << "3. 获取原始指针: " << smart_ptr.get() << std::endl;
        std::cout << "4. 释放所有权: " << smart_ptr.release() << std::endl;
        std::cout << "5. 重置指针: smart_ptr.reset()" << std::endl;
        
        std::cout << "\n--- 原始指针操作 ---" << std::endl;
        TestClass* raw_ptr = new TestClass(800);
        
        // 原始指针操作
        std::cout << "原始指针操作:" << std::endl;
        std::cout << "1. 解引用: " << (*raw_ptr).value << std::endl;
        std::cout << "2. 箭头操作: " << raw_ptr->value << std::endl;
        std::cout << "3. 指针算术: " << (raw_ptr + 1) << std::endl;
        std::cout << "4. 指针比较: " << (raw_ptr == nullptr) << std::endl;
        std::cout << "5. 手动删除: delete raw_ptr" << std::endl;
        
        delete raw_ptr;
    }
}

void test_ownership_transfer() {
    std::cout << "\n=== 所有权转移 ===" << std::endl;
    
    {
        std::cout << "\n--- 所有权转移过程 ---" << std::endl;
        unique_ptr<TestClass> smart_ptr(new TestClass(900));
        std::cout << "初始所有者: 智能指针" << std::endl;
        std::cout << "智能指针对象值: " << smart_ptr->value << std::endl;
        
        // 转移所有权
        TestClass* raw_ptr = smart_ptr.release();
        std::cout << "所有权转移后:" << std::endl;
        std::cout << "智能指针是否为空: " << (smart_ptr ? "否" : "是") << std::endl;
        std::cout << "原始指针对象值: " << raw_ptr->value << std::endl;
        std::cout << "新所有者: 原始指针" << std::endl;
        
        // 原始指针负责释放
        delete raw_ptr;
        std::cout << "原始指针释放完成" << std::endl;
    }
}

int main() {
    std::cout << "智能指针 vs 原始指针详解" << std::endl;
    std::cout << "========================" << std::endl;
    
    try {
        test_smart_vs_raw_pointer();
        test_pointer_capabilities();
        test_memory_management();
        test_exception_safety();
        test_pointer_operations();
        test_ownership_transfer();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}










