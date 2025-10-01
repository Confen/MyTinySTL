#include <iostream>
#include <stack>
#include <string>
#include <windows.h>

// 基类
class Base {
public:
    Base() {
        std::cout << "Base 构造函数 - 入栈" << std::endl;
    }
    
    virtual ~Base() {
        std::cout << "Base 析构函数 - 出栈" << std::endl;
    }
};

// 派生类
class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived 构造函数 - 入栈" << std::endl;
    }
    
    virtual ~Derived() {
        std::cout << "Derived 析构函数 - 出栈" << std::endl;
    }
};

// 最派生类
class GrandChild : public Derived {
public:
    GrandChild() {
        std::cout << "GrandChild 构造函数 - 入栈" << std::endl;
    }
    
    virtual ~GrandChild() {
        std::cout << "GrandChild 析构函数 - 出栈" << std::endl;
    }
};

void demonstrate_stack_analogy() {
    std::cout << "=== 析构函数链的栈类比 ===" << std::endl;
    
    std::cout << "1. 创建对象 - 构造函数入栈:" << std::endl;
    GrandChild* obj = new GrandChild();
    
    std::cout << "\n2. 删除对象 - 析构函数出栈:" << std::endl;
    delete obj;
}

void demonstrate_manual_stack() {
    std::cout << "\n=== 手动模拟栈操作 ===" << std::endl;
    
    std::stack<std::string> constructorStack;
    std::stack<std::string> destructorStack;
    
    std::cout << "1. 模拟构造函数入栈:" << std::endl;
    constructorStack.push("Base 构造函数");
    std::cout << "   栈内容: ";
    printStack(constructorStack);
    
    constructorStack.push("Derived 构造函数");
    std::cout << "   栈内容: ";
    printStack(constructorStack);
    
    constructorStack.push("GrandChild 构造函数");
    std::cout << "   栈内容: ";
    printStack(constructorStack);
    
    std::cout << "\n2. 模拟析构函数出栈:" << std::endl;
    while (!constructorStack.empty()) {
        std::string func = constructorStack.top();
        constructorStack.pop();
        destructorStack.push(func.replace(func.find("构造"), 4, "析构"));
        std::cout << "   出栈: " << func << std::endl;
        std::cout << "   剩余栈: ";
        printStack(constructorStack);
    }
}

void printStack(std::stack<std::string> s) {
    if (s.empty()) {
        std::cout << "空栈" << std::endl;
        return;
    }
    
    std::cout << "[";
    while (!s.empty()) {
        std::cout << s.top();
        s.pop();
        if (!s.empty()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void demonstrate_why_stack_order() {
    std::cout << "\n=== 为什么是栈的顺序 ===" << std::endl;
    
    std::cout << "1. 依赖关系:" << std::endl;
    std::cout << "   GrandChild 依赖于 Derived" << std::endl;
    std::cout << "   Derived 依赖于 Base" << std::endl;
    std::cout << "   必须先清理依赖者，再清理被依赖者" << std::endl;
    
    std::cout << "\n2. 资源管理:" << std::endl;
    std::cout << "   派生类可能依赖基类的资源" << std::endl;
    std::cout << "   必须先清理派生类，再清理基类" << std::endl;
    
    std::cout << "\n3. 异常安全:" << std::endl;
    std::cout << "   如果派生类析构函数抛出异常" << std::endl;
    std::cout << "   基类析构函数仍然会被调用" << std::endl;
}

void demonstrate_stack_visualization() {
    std::cout << "\n=== 栈的可视化演示 ===" << std::endl;
    
    std::cout << "1. 构造函数调用栈:" << std::endl;
    std::cout << "   ┌─────────────────────┐ ← 栈顶" << std::endl;
    std::cout << "   │ GrandChild 构造函数 │" << std::endl;
    std::cout << "   ├─────────────────────┤" << std::endl;
    std::cout << "   │ Derived 构造函数    │" << std::endl;
    std::cout << "   ├─────────────────────┤" << std::endl;
    std::cout << "   │ Base 构造函数       │ ← 栈底" << std::endl;
    std::cout << "   └─────────────────────┘" << std::endl;
    
    std::cout << "\n2. 析构函数调用栈:" << std::endl;
    std::cout << "   ┌─────────────────────┐ ← 栈顶" << std::endl;
    std::cout << "   │ GrandChild 析构函数 │ ← 先出栈" << std::endl;
    std::cout << "   ├─────────────────────┤" << std::endl;
    std::cout << "   │ Derived 析构函数    │ ← 后出栈" << std::endl;
    std::cout << "   ├─────────────────────┤" << std::endl;
    std::cout << "   │ Base 析构函数       │ ← 最后出栈" << std::endl;
    std::cout << "   └─────────────────────┘" << std::endl;
}

void demonstrate_exception_safety_stack() {
    std::cout << "\n=== 异常安全的栈行为 ===" << std::endl;
    
    std::cout << "1. 正常情况:" << std::endl;
    std::cout << "   所有析构函数都正常执行" << std::endl;
    
    std::cout << "\n2. 异常情况:" << std::endl;
    std::cout << "   如果某个析构函数抛出异常" << std::endl;
    std::cout << "   后续的析构函数仍然会被调用" << std::endl;
    std::cout << "   这保证了异常安全" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 析构函数链的栈类比演示 ===" << std::endl;
    
    demonstrate_stack_analogy();
    demonstrate_manual_stack();
    demonstrate_why_stack_order();
    demonstrate_stack_visualization();
    demonstrate_exception_safety_stack();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}










