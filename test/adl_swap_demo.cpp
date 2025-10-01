#include <iostream>
#include <string>
#include <windows.h>

// 自定义命名空间
namespace MyNamespace {
    class MyClass {
        int value;
    public:
        MyClass(int v) : value(v) {}
        
        int getValue() const { return value; }
        
        // 自定义 swap 函数
        void swap(MyClass& other) noexcept {
            std::cout << "  调用 MyClass::swap" << std::endl;
            std::swap(value, other.value);
        }
        
        friend void swap(MyClass& a, MyClass& b) noexcept {
            std::cout << "  调用 MyNamespace::swap(MyClass&, MyClass&)" << std::endl;
            a.swap(b);
        }
    };
}

// 另一个命名空间
namespace AnotherNamespace {
    class AnotherClass {
        std::string data;
    public:
        AnotherClass(const std::string& s) : data(s) {}
        
        const std::string& getData() const { return data; }
        
        // 只有成员 swap 函数
        void swap(AnotherClass& other) noexcept {
            std::cout << "  调用 AnotherClass::swap" << std::endl;
            data.swap(other.data);
        }
    };
}

// 没有自定义 swap 的类
class SimpleClass {
    int value;
public:
    SimpleClass(int v) : value(v) {}
    int getValue() const { return value; }
};

void demonstrate_adl_basic() {
    std::cout << "=== ADL 基本概念演示 ===" << std::endl;
    
    std::cout << "1. 什么是 ADL？" << std::endl;
    std::cout << "   ADL (Argument-Dependent Lookup) 允许编译器" << std::endl;
    std::cout << "   在参数类型所属的命名空间中查找函数" << std::endl;
    
    std::cout << "\n2. 传统函数查找:" << std::endl;
    std::cout << "   只在当前作用域和全局作用域查找" << std::endl;
    
    std::cout << "\n3. ADL 函数查找:" << std::endl;
    std::cout << "   在当前作用域 + 参数类型命名空间 + 全局作用域查找" << std::endl;
}

void demonstrate_adl_swap() {
    std::cout << "\n=== ADL 在 swap 中的应用 ===" << std::endl;
    
    // 1. 有自定义 swap 函数的类
    std::cout << "1. 有自定义 swap 函数的类:" << std::endl;
    MyNamespace::MyClass obj1(10);
    MyNamespace::MyClass obj2(20);
    
    std::cout << "   交换前: obj1=" << obj1.getValue() << ", obj2=" << obj2.getValue() << std::endl;
    
    // 使用 ADL 查找 swap 函数
    using std::swap;  // 引入 std::swap
    swap(obj1, obj2); // ADL 会找到 MyNamespace::swap
    
    std::cout << "   交换后: obj1=" << obj1.getValue() << ", obj2=" << obj2.getValue() << std::endl;
    
    // 2. 只有成员 swap 函数的类
    std::cout << "\n2. 只有成员 swap 函数的类:" << std::endl;
    AnotherNamespace::AnotherClass obj3("hello");
    AnotherNamespace::AnotherClass obj4("world");
    
    std::cout << "   交换前: obj3=" << obj3.getData() << ", obj4=" << obj4.getData() << std::endl;
    
    // 使用 ADL 查找 swap 函数
    using std::swap;  // 引入 std::swap
    swap(obj3, obj4); // ADL 找不到，使用 std::swap
    
    std::cout << "   交换后: obj3=" << obj3.getData() << ", obj4=" << obj4.getData() << std::endl;
    
    // 3. 没有自定义 swap 的类
    std::cout << "\n3. 没有自定义 swap 的类:" << std::endl;
    SimpleClass obj5(100);
    SimpleClass obj6(200);
    
    std::cout << "   交换前: obj5=" << obj5.getValue() << ", obj6=" << obj6.getValue() << std::endl;
    
    // 使用 ADL 查找 swap 函数
    using std::swap;  // 引入 std::swap
    swap(obj5, obj6); // ADL 找不到，使用 std::swap
    
    std::cout << "   交换后: obj5=" << obj5.getValue() << ", obj6=" << obj6.getValue() << std::endl;
}

void demonstrate_swap_priority() {
    std::cout << "\n=== swap 函数优先级演示 ===" << std::endl;
    
    std::cout << "1. swap 函数查找优先级:" << std::endl;
    std::cout << "   1. 成员函数 swap" << std::endl;
    std::cout << "   2. 同命名空间的自由函数 swap" << std::endl;
    std::cout << "   3. std::swap" << std::endl;
    std::cout << "   4. 通用实现（移动语义）" << std::endl;
    
    std::cout << "\n2. 为什么需要 ADL？" << std::endl;
    std::cout << "   - 允许类作者提供优化的 swap 实现" << std::endl;
    std::cout << "   - 保持与标准库的兼容性" << std::endl;
    std::cout << "   - 支持模板代码的通用性" << std::endl;
}

// 简化的 swap 实现
template<typename T>
void my_swap(T& a, T& b) {
    using std::swap;
    swap(a, b);
}

void demonstrate_manual_swap_impl() {
    std::cout << "\n=== 手动实现 swap_impl 演示 ===" << std::endl;
    
    // 测试不同类型的 swap
    std::cout << "1. 测试有成员 swap 的类:" << std::endl;
    MyNamespace::MyClass obj1(10);
    MyNamespace::MyClass obj2(20);
    my_swap(obj1, obj2);
    
    std::cout << "\n2. 测试有自由函数 swap 的类:" << std::endl;
    MyNamespace::MyClass obj3(30);
    MyNamespace::MyClass obj4(40);
    my_swap(obj3, obj4);
    
    std::cout << "\n3. 测试没有自定义 swap 的类:" << std::endl;
    SimpleClass obj5(100);
    SimpleClass obj6(200);
    my_swap(obj5, obj6);
}

void demonstrate_adl_benefits() {
    std::cout << "\n=== ADL 的优势 ===" << std::endl;
    
    std::cout << "1. 性能优化:" << std::endl;
    std::cout << "   类作者可以提供高效的 swap 实现" << std::endl;
    std::cout << "   避免不必要的拷贝操作" << std::endl;
    
    std::cout << "\n2. 代码复用:" << std::endl;
    std::cout << "   模板代码可以自动使用最佳的实现" << std::endl;
    std::cout << "   无需为每个类型特化" << std::endl;
    
    std::cout << "\n3. 标准兼容:" << std::endl;
    std::cout << "   与标准库的 swap 机制完全兼容" << std::endl;
    std::cout << "   遵循 C++ 最佳实践" << std::endl;
    
    std::cout << "\n4. 扩展性:" << std::endl;
    std::cout << "   可以轻松为现有类型添加 swap 支持" << std::endl;
    std::cout << "   不影响现有代码" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== ADL 在 swap 中的应用详解 ===" << std::endl;
    
    demonstrate_adl_basic();
    demonstrate_adl_swap();
    demonstrate_swap_priority();
    demonstrate_manual_swap_impl();
    demonstrate_adl_benefits();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}
