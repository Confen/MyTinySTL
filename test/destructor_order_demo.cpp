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

void test_stack_destructor_order() {
    std::cout << "=== 栈对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 局部变量析构顺序 ---" << std::endl;
    TestClass obj1(1);  // 第一个构造
    TestClass obj2(2);  // 第二个构造
    TestClass obj3(3);  // 第三个构造
    
    std::cout << "作用域结束，开始析构..." << std::endl;
    // 析构顺序：obj3 -> obj2 -> obj1（后进先出）
}

void test_heap_destructor_order() {
    std::cout << "\n=== 堆对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 手动控制析构顺序 ---" << std::endl;
    TestClass* ptr1 = new TestClass(10);
    TestClass* ptr2 = new TestClass(20);
    TestClass* ptr3 = new TestClass(30);
    
    std::cout << "开始手动析构..." << std::endl;
    delete ptr3;  // 先析构
    delete ptr2;  // 再析构
    delete ptr1;  // 最后析构
}

void test_smart_pointer_destructor_order() {
    std::cout << "\n=== 智能指针析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 智能指针析构顺序 ---" << std::endl;
    unique_ptr<TestClass> ptr1(new TestClass(100));
    unique_ptr<TestClass> ptr2(new TestClass(200));
    unique_ptr<TestClass> ptr3(new TestClass(300));
    
    std::cout << "作用域结束，智能指针开始析构..." << std::endl;
    // 析构顺序：ptr3 -> ptr2 -> ptr1（后进先出）
}

void test_mixed_destructor_order() {
    std::cout << "\n=== 混合对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 栈对象 + 智能指针 ---" << std::endl;
    TestClass stack_obj(1000);
    unique_ptr<TestClass> smart_ptr(new TestClass(2000));
    
    std::cout << "作用域结束，开始析构..." << std::endl;
    // 析构顺序：smart_ptr -> stack_obj（后进先出）
}

class Container {
    TestClass member1;
    TestClass member2;
    
public:
    Container(int v1, int v2) : member1(v1), member2(v2) {
        std::cout << "构造 Container" << std::endl;
    }
    
    ~Container() {
        std::cout << "析构 Container" << std::endl;
    }
};

void test_composite_destructor_order() {
    std::cout << "\n=== 复合对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 复合对象析构顺序 ---" << std::endl;
    Container container(500, 600);
    
    std::cout << "作用域结束，开始析构..." << std::endl;
    // 析构顺序：Container析构函数 -> member2 -> member1
}

class Base {
public:
    Base() { std::cout << "构造 Base" << std::endl; }
    ~Base() { std::cout << "析构 Base" << std::endl; }
};

class Derived : public Base {
    TestClass member;
    
public:
    Derived() : member(700) {
        std::cout << "构造 Derived" << std::endl;
    }
    
    ~Derived() {
        std::cout << "析构 Derived" << std::endl;
    }
};

void test_inheritance_destructor_order() {
    std::cout << "\n=== 继承对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 继承对象析构顺序 ---" << std::endl;
    Derived derived;
    
    std::cout << "作用域结束，开始析构..." << std::endl;
    // 析构顺序：Derived析构函数 -> member -> Base析构函数
}

void test_array_destructor_order() {
    std::cout << "\n=== 数组对象析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 数组对象析构顺序 ---" << std::endl;
    TestClass arr[3] = {TestClass(800), TestClass(900), TestClass(1000)};
    
    std::cout << "作用域结束，开始析构..." << std::endl;
    // 析构顺序：arr[2] -> arr[1] -> arr[0]（后进先出）
}

void test_nested_scope_destructor_order() {
    std::cout << "\n=== 嵌套作用域析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 外层作用域 ---" << std::endl;
    TestClass outer1(1100);
    TestClass outer2(1200);
    
    {
        std::cout << "\n--- 内层作用域 ---" << std::endl;
        TestClass inner1(1300);
        TestClass inner2(1400);
        
        std::cout << "内层作用域结束，开始析构..." << std::endl;
        // 析构顺序：inner2 -> inner1
    }
    
    std::cout << "外层作用域结束，开始析构..." << std::endl;
    // 析构顺序：outer2 -> outer1
}

void test_exception_destructor_order() {
    std::cout << "\n=== 异常情况析构顺序 ===" << std::endl;
    
    std::cout << "\n--- 异常情况析构顺序 ---" << std::endl;
    try {
        TestClass obj1(1500);
        TestClass obj2(1600);
        
        std::cout << "抛出异常..." << std::endl;
        throw std::runtime_error("测试异常");
        
    } catch (const std::exception& e) {
        std::cout << "异常被捕获: " << e.what() << std::endl;
        // 析构顺序：obj2 -> obj1（异常安全）
    }
}

void test_destructor_order_summary() {
    std::cout << "\n=== 析构顺序总结 ===" << std::endl;
    
    std::cout << "\n--- 析构顺序规则 ---" << std::endl;
    std::cout << "1. 栈对象：后进先出（LIFO）" << std::endl;
    std::cout << "2. 堆对象：手动控制" << std::endl;
    std::cout << "3. 智能指针：后进先出（LIFO）" << std::endl;
    std::cout << "4. 复合对象：成员变量逆序析构" << std::endl;
    std::cout << "5. 继承对象：派生类 -> 基类" << std::endl;
    std::cout << "6. 数组对象：逆序析构" << std::endl;
    std::cout << "7. 异常情况：栈展开时逆序析构" << std::endl;
}

int main() {
    std::cout << "析构顺序详解" << std::endl;
    std::cout << "============" << std::endl;
    
    try {
        test_stack_destructor_order();
        test_heap_destructor_order();
        test_smart_pointer_destructor_order();
        test_mixed_destructor_order();
        test_composite_destructor_order();
        test_inheritance_destructor_order();
        test_array_destructor_order();
        test_nested_scope_destructor_order();
        test_exception_destructor_order();
        test_destructor_order_summary();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}