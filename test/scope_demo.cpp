#include <iostream>
#include <windows.h>

namespace B {
    struct A {
        int data;
        
        A(int v) : data(v) {}
        
        // 成员函数 - 属于类的作用域
        void member_swap(A& other) {
            std::cout << "调用成员函数: B::A::member_swap" << std::endl;
            int temp = data;
            data = other.data;
            other.data = temp;
        }
        
        // 友元函数 - 属于命名空间的作用域
        friend void friend_swap(A& a, A& b) {
            std::cout << "调用友元函数: B::friend_swap" << std::endl;
            a.member_swap(b);
        }
    };
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    B::A a(1);
    B::A b(2);
    
    std::cout << "=== 作用域规则演示 ===" << std::endl;
    
    // 1. 成员函数调用 - 不需要命名空间
    std::cout << "1. 成员函数调用:" << std::endl;
    a.member_swap(b);  // 直接调用，不需要 B::A::member_swap
    
    std::cout << "   交换后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 2. 友元函数调用 - 需要 ADL 或命名空间
    std::cout << "\n2. 友元函数调用:" << std::endl;
    
    // 方法1：通过 ADL（推荐）
    friend_swap(a, b);  // ADL 找到 B::friend_swap
    
    std::cout << "   交换后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 方法2：显式指定命名空间（不推荐，因为友元函数通常不在命名空间中）
    // B::friend_swap(a, b);  // 这可能会编译错误
    
    // 3. 对比：如果友元函数在全局作用域
    std::cout << "\n3. 全局友元函数调用:" << std::endl;
    global_swap(a, b);  // 直接调用全局函数
    
    return 0;
}

// 全局友元函数（在类外部定义）
void global_swap(B::A& a, B::A& b) {
    std::cout << "调用全局友元函数: ::global_swap" << std::endl;
    a.member_swap(b);
}











