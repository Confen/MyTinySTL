#include <iostream>
#include <windows.h>

// 基类
class Base {
public:
    Base() {
        std::cout << "Base 构造函数" << std::endl;
    }
    
    virtual ~Base() {
        std::cout << "Base 析构函数" << std::endl;
    }
};

// 派生类
class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived 构造函数" << std::endl;
    }
    
    virtual ~Derived() {
        std::cout << "Derived 析构函数" << std::endl;
    }
};

// 多层继承
class GrandChild : public Derived {
public:
    GrandChild() {
        std::cout << "GrandChild 构造函数" << std::endl;
    }
    
    virtual ~GrandChild() {
        std::cout << "GrandChild 析构函数" << std::endl;
    }
};

void demonstrate_constructor_destructor_order() {
    std::cout << "=== 构造函数和析构函数调用顺序 ===" << std::endl;
    
    std::cout << "1. 创建 Derived 对象:" << std::endl;
    Derived* derived = new Derived();
    
    std::cout << "\n2. 删除 Derived 对象:" << std::endl;
    delete derived;
    
    std::cout << "\n3. 创建 GrandChild 对象:" << std::endl;
    GrandChild* grandChild = new GrandChild();
    
    std::cout << "\n4. 删除 GrandChild 对象:" << std::endl;
    delete grandChild;
}

void demonstrate_why_base_destructor_called() {
    std::cout << "\n=== 为什么基类析构函数会被调用 ===" << std::endl;
    
    std::cout << "1. 继承关系:" << std::endl;
    std::cout << "   Derived 对象包含 Base 部分" << std::endl;
    std::cout << "   删除 Derived 对象时，需要清理 Base 部分" << std::endl;
    
    std::cout << "\n2. 内存布局:" << std::endl;
    std::cout << "   Derived 对象内存布局:" << std::endl;
    std::cout << "   ┌─────────────────┐" << std::endl;
    std::cout << "   │ Base 部分       │ ← 需要调用 Base 析构函数" << std::endl;
    std::cout << "   ├─────────────────┤" << std::endl;
    std::cout << "   │ Derived 部分    │ ← 需要调用 Derived 析构函数" << std::endl;
    std::cout << "   └─────────────────┘" << std::endl;
    
    std::cout << "\n3. 析构函数调用顺序:" << std::endl;
    std::cout << "   1. 调用 Derived 析构函数" << std::endl;
    std::cout << "   2. 自动调用 Base 析构函数" << std::endl;
}

void demonstrate_virtual_destructor_importance() {
    std::cout << "\n=== 虚析构函数的重要性 ===" << std::endl;
    
    std::cout << "1. 通过基类指针删除派生类对象:" << std::endl;
    Base* base = new Derived();
    std::cout << "   基类指针指向派生类对象" << std::endl;
    
    std::cout << "\n2. 删除对象:" << std::endl;
    delete base;  // 如果 Base 析构函数不是虚函数，只会调用 Base 析构函数
    std::cout << "   虚析构函数确保调用正确的析构函数" << std::endl;
}

void demonstrate_manual_destructor_call() {
    std::cout << "\n=== 手动调用析构函数演示 ===" << std::endl;
    
    std::cout << "1. 创建对象:" << std::endl;
    Derived* derived = new Derived();
    
    std::cout << "\n2. 手动调用析构函数:" << std::endl;
    derived->~Derived();  // 手动调用析构函数
    
    std::cout << "\n3. 释放内存:" << std::endl;
    delete derived;  // 释放内存，但不会再次调用析构函数
}

void demonstrate_destructor_chain() {
    std::cout << "\n=== 析构函数链演示 ===" << std::endl;
    
    std::cout << "1. 多层继承的析构函数链:" << std::endl;
    GrandChild* grandChild = new GrandChild();
    
    std::cout << "\n2. 删除对象时的析构函数链:" << std::endl;
    delete grandChild;
    
    std::cout << "\n3. 析构函数调用顺序:" << std::endl;
    std::cout << "   1. GrandChild 析构函数" << std::endl;
    std::cout << "   2. Derived 析构函数" << std::endl;
    std::cout << "   3. Base 析构函数" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 析构函数调用顺序详解 ===" << std::endl;
    
    demonstrate_constructor_destructor_order();
    demonstrate_why_base_destructor_called();
    demonstrate_virtual_destructor_importance();
    demonstrate_manual_destructor_call();
    demonstrate_destructor_chain();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}










