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

void test_stack_instantiation() {
    std::cout << "=== 栈实例化（自动管理）===" << std::endl;
    
    {
        std::cout << "\n--- 局部对象实例化 ---" << std::endl;
        TestClass stack_obj(100);  // 在栈上实例化
        std::cout << "栈对象地址: " << &stack_obj << std::endl;
        std::cout << "栈对象值: " << stack_obj.value << std::endl;
        
        // 栈对象会在作用域结束时自动销毁
    }
    std::cout << "栈对象自动销毁完成" << std::endl;
}

void test_heap_instantiation() {
    std::cout << "\n=== 堆实例化（手动管理）===" << std::endl;
    
    {
        std::cout << "\n--- 堆对象实例化 ---" << std::endl;
        TestClass* heap_obj = new TestClass(200);  // 在堆上实例化
        std::cout << "堆对象地址: " << heap_obj << std::endl;
        std::cout << "堆对象值: " << heap_obj->value << std::endl;
        
        // 堆对象需要手动删除
        delete heap_obj;
        std::cout << "堆对象手动删除完成" << std::endl;
    }
}

void test_smart_pointer_instantiation() {
    std::cout << "\n=== 智能指针实例化（自动管理堆对象）===" << std::endl;
    
    {
        std::cout << "\n--- 智能指针实例化 ---" << std::endl;
        unique_ptr<TestClass> smart_obj(new TestClass(300));  // 在堆上实例化，但自动管理
        std::cout << "智能指针地址: " << &smart_obj << std::endl;
        std::cout << "智能指针指向的对象地址: " << smart_obj.get() << std::endl;
        std::cout << "智能指针对象值: " << smart_obj->value << std::endl;
        
        // 智能指针会在作用域结束时自动删除堆对象
    }
    std::cout << "智能指针自动删除完成" << std::endl;
}

void test_memory_layout() {
    std::cout << "\n=== 内存布局分析 ===" << std::endl;
    
    {
        std::cout << "\n--- 栈对象内存布局 ---" << std::endl;
        TestClass stack_obj(400);
        std::cout << "栈对象地址: " << &stack_obj << std::endl;
        std::cout << "栈对象大小: " << sizeof(stack_obj) << " 字节" << std::endl;
        
        std::cout << "\n--- 堆对象内存布局 ---" << std::endl;
        TestClass* heap_obj = new TestClass(500);
        std::cout << "堆对象地址: " << heap_obj << std::endl;
        std::cout << "堆对象大小: " << sizeof(*heap_obj) << " 字节" << std::endl;
        
        std::cout << "\n--- 智能指针内存布局 ---" << std::endl;
        unique_ptr<TestClass> smart_obj(new TestClass(600));
        std::cout << "智能指针地址: " << &smart_obj << std::endl;
        std::cout << "智能指针大小: " << sizeof(smart_obj) << " 字节" << std::endl;
        std::cout << "智能指针指向的对象地址: " << smart_obj.get() << std::endl;
        std::cout << "智能指针指向的对象大小: " << sizeof(*smart_obj) << " 字节" << std::endl;
        
        delete heap_obj;
    }
}

void test_instantiation_timing() {
    std::cout << "\n=== 实例化时机分析 ===" << std::endl;
    
    std::cout << "\n--- 声明时实例化 ---" << std::endl;
    TestClass obj1(700);  // 声明时立即实例化
    std::cout << "obj1 已实例化，值: " << obj1.value << std::endl;
    
    std::cout << "\n--- 延迟实例化 ---" << std::endl;
    TestClass* obj2 = nullptr;  // 只声明指针，不实例化
    std::cout << "obj2 指针已声明，但对象未实例化" << std::endl;
    
    obj2 = new TestClass(800);  // 现在才实例化
    std::cout << "obj2 现在已实例化，值: " << obj2->value << std::endl;
    
    delete obj2;
}

void test_constructor_calls() {
    std::cout << "\n=== 构造函数调用分析 ===" << std::endl;
    
    std::cout << "\n--- 栈实例化构造函数调用 ---" << std::endl;
    TestClass stack_obj(900);  // 调用构造函数
    
    std::cout << "\n--- 堆实例化构造函数调用 ---" << std::endl;
    TestClass* heap_obj = new TestClass(1000);  // 调用构造函数
    
    std::cout << "\n--- 智能指针实例化构造函数调用 ---" << std::endl;
    unique_ptr<TestClass> smart_obj(new TestClass(1100));  // 调用构造函数
    
    delete heap_obj;
}

void test_destructor_calls() {
    std::cout << "\n=== 析构函数调用分析 ===" << std::endl;
    
    std::cout << "\n--- 栈对象析构函数调用 ---" << std::endl;
    {
        TestClass stack_obj(1200);
        std::cout << "栈对象创建完成" << std::endl;
        // 作用域结束时自动调用析构函数
    }
    std::cout << "栈对象析构函数已调用" << std::endl;
    
    std::cout << "\n--- 堆对象析构函数调用 ---" << std::endl;
    TestClass* heap_obj = new TestClass(1300);
    std::cout << "堆对象创建完成" << std::endl;
    delete heap_obj;  // 手动调用析构函数
    std::cout << "堆对象析构函数已调用" << std::endl;
    
    std::cout << "\n--- 智能指针析构函数调用 ---" << std::endl;
    {
        unique_ptr<TestClass> smart_obj(new TestClass(1400));
        std::cout << "智能指针对象创建完成" << std::endl;
        // 作用域结束时自动调用析构函数
    }
    std::cout << "智能指针对象析构函数已调用" << std::endl;
}

int main() {
    std::cout << "类的实例化位置详解" << std::endl;
    std::cout << "==================" << std::endl;
    
    try {
        test_stack_instantiation();
        test_heap_instantiation();
        test_smart_pointer_instantiation();
        test_memory_layout();
        test_instantiation_timing();
        test_constructor_calls();
        test_destructor_calls();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

