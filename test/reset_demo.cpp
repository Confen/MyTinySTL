#include <iostream>
#include <memory>
#include "../memory.h"

using namespace mystl;

// 测试类，用于观察构造和析构
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

void test_reset_basic() {
    std::cout << "=== 测试 reset 基本用法 ===" << std::endl;
    
    // 创建 unique_ptr
    unique_ptr<TestClass> ptr(new TestClass(10));
    std::cout << "ptr 管理的对象值: " << ptr->value << std::endl;
    
    // 重置为空指针
    std::cout << "\n调用 reset():" << std::endl;
    ptr.reset();
    std::cout << "ptr 是否为空: " << (ptr ? "否" : "是") << std::endl;
    
    // 重置为新的指针
    std::cout << "\n调用 reset(new TestClass(20)):" << std::endl;
    ptr.reset(new TestClass(20));
    std::cout << "ptr 管理的对象值: " << ptr->value << std::endl;
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

void test_reset_with_move_assignment() {
    std::cout << "\n=== 测试移动赋值中的 reset ===" << std::endl;
    
    unique_ptr<TestClass> ptr1(new TestClass(100));
    unique_ptr<TestClass> ptr2(new TestClass(200));
    
    std::cout << "移动赋值前:" << std::endl;
    std::cout << "ptr1 管理的对象值: " << ptr1->value << std::endl;
    std::cout << "ptr2 管理的对象值: " << ptr2->value << std::endl;
    
    // 移动赋值：内部会调用 reset(other.release())
    std::cout << "\n执行 ptr1 = std::move(ptr2):" << std::endl;
    ptr1 = std::move(ptr2);
    
    std::cout << "移动赋值后:" << std::endl;
    std::cout << "ptr1 管理的对象值: " << ptr1->value << std::endl;
    std::cout << "ptr2 是否为空: " << (ptr2 ? "否" : "是") << std::endl;
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

void test_reset_chain() {
    std::cout << "\n=== 测试 reset 链式调用 ===" << std::endl;
    
    unique_ptr<TestClass> ptr(new TestClass(300));
    std::cout << "初始对象值: " << ptr->value << std::endl;
    
    // 连续重置
    std::cout << "\n第一次 reset(new TestClass(400)):" << std::endl;
    ptr.reset(new TestClass(400));
    std::cout << "当前对象值: " << ptr->value << std::endl;
    
    std::cout << "\n第二次 reset(new TestClass(500)):" << std::endl;
    ptr.reset(new TestClass(500));
    std::cout << "当前对象值: " << ptr->value << std::endl;
    
    std::cout << "\n第三次 reset():" << std::endl;
    ptr.reset();
    std::cout << "ptr 是否为空: " << (ptr ? "否" : "是") << std::endl;
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

void test_reset_with_exception_safety() {
    std::cout << "\n=== 测试 reset 异常安全性 ===" << std::endl;
    
    unique_ptr<TestClass> ptr(new TestClass(600));
    std::cout << "初始对象值: " << ptr->value << std::endl;
    
    try {
        // 即使 new 失败，reset 也能正确处理
        std::cout << "\n尝试 reset(nullptr):" << std::endl;
        ptr.reset(nullptr);
        std::cout << "reset(nullptr) 成功" << std::endl;
        
        std::cout << "\n尝试 reset(new TestClass(700)):" << std::endl;
        ptr.reset(new TestClass(700));
        std::cout << "reset(new TestClass(700)) 成功，对象值: " << ptr->value << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

void test_reset_vs_release() {
    std::cout << "\n=== 测试 reset vs release ===" << std::endl;
    
    // 测试 reset
    std::cout << "--- 测试 reset ---" << std::endl;
    unique_ptr<TestClass> ptr1(new TestClass(800));
    std::cout << "reset 前对象值: " << ptr1->value << std::endl;
    
    ptr1.reset(new TestClass(900));
    std::cout << "reset 后对象值: " << ptr1->value << std::endl;
    
    // 测试 release
    std::cout << "\n--- 测试 release ---" << std::endl;
    unique_ptr<TestClass> ptr2(new TestClass(1000));
    std::cout << "release 前对象值: " << ptr2->value << std::endl;
    
    TestClass* raw_ptr = ptr2.release();
    std::cout << "release 后 ptr2 是否为空: " << (ptr2 ? "否" : "是") << std::endl;
    std::cout << "raw_ptr 指向的对象值: " << raw_ptr->value << std::endl;
    
    // 手动删除 raw_ptr
    delete raw_ptr;
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

void test_reset_with_different_types() {
    std::cout << "\n=== 测试不同类型的 reset ===" << std::endl;
    
    // 测试基本类型
    unique_ptr<int> int_ptr(new int(42));
    std::cout << "int_ptr 初始值: " << *int_ptr << std::endl;
    
    int_ptr.reset(new int(84));
    std::cout << "int_ptr reset 后值: " << *int_ptr << std::endl;
    
    // 测试字符串类型
    unique_ptr<std::string> str_ptr(new std::string("Hello"));
    std::cout << "str_ptr 初始值: " << *str_ptr << std::endl;
    
    str_ptr.reset(new std::string("World"));
    std::cout << "str_ptr reset 后值: " << *str_ptr << std::endl;
    
    // 测试自定义类型
    unique_ptr<TestClass> custom_ptr(new TestClass(999));
    std::cout << "custom_ptr 初始值: " << custom_ptr->value << std::endl;
    
    custom_ptr.reset(new TestClass(1111));
    std::cout << "custom_ptr reset 后值: " << custom_ptr->value << std::endl;
    
    std::cout << "\n作用域结束，自动析构:" << std::endl;
}

int main() {
    std::cout << "MyTinySTL unique_ptr::reset 方法测试" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    try {
        test_reset_basic();
        test_reset_with_move_assignment();
        test_reset_chain();
        test_reset_with_exception_safety();
        test_reset_vs_release();
        test_reset_with_different_types();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
