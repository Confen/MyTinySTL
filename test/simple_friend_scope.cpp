#include <iostream>
#include <windows.h>

namespace B {
    struct A {
        int data;
        
        A(int v) : data(v) {}
        
        // 方式1：友元函数内联定义（在类内部）
        friend void inline_friend_swap(A& a, A& b) {
            std::cout << "内联友元函数: 在类内部定义" << std::endl;
            int temp = a.data;
            a.data = b.data;
            b.data = temp;
        }
        
        // 方式2：友元函数声明（在命名空间内定义）
        friend void namespace_friend_swap(A& a, A& b);
    };
    
    // 方式2：在命名空间内定义友元函数
    void namespace_friend_swap(A& a, A& b) {
        std::cout << "命名空间内友元函数: B::namespace_friend_swap" << std::endl;
        int temp = a.data;
        a.data = b.data;
        b.data = temp;
    }
}

// 方式3：全局友元函数（不在任何命名空间中）
void global_friend_swap(B::A& a, B::A& b) {
    std::cout << "全局友元函数: ::global_friend_swap" << std::endl;
    int temp = a.data;
    a.data = b.data;
    b.data = temp;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    B::A a(1);
    B::A b(2);
    
    std::cout << "=== 友元函数作用域演示 ===" << std::endl;
    std::cout << "初始值: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 1. 内联友元函数（在类内部定义）
    std::cout << "\n1. 内联友元函数:" << std::endl;
    inline_friend_swap(a, b);  // ADL 找到 B::inline_friend_swap
    std::cout << "   交换后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 2. 命名空间内友元函数
    std::cout << "\n2. 命名空间内友元函数:" << std::endl;
    namespace_friend_swap(a, b);  // ADL 找到 B::namespace_friend_swap
    std::cout << "   交换后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 3. 全局友元函数
    std::cout << "\n3. 全局友元函数:" << std::endl;
    global_friend_swap(a, b);  // ADL 找到 ::global_friend_swap
    std::cout << "   交换后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 4. 显式调用不同作用域的友元函数
    std::cout << "\n4. 显式调用:" << std::endl;
    
    // 调用命名空间内的友元函数
    B::namespace_friend_swap(a, b);
    std::cout << "   调用 B::namespace_friend_swap 后: a=" << a.data << ", b=" << b.data << std::endl;
    
    // 调用全局友元函数
    ::global_friend_swap(a, b);
    std::cout << "   调用 ::global_friend_swap 后: a=" << a.data << ", b=" << b.data << std::endl;
    
    return 0;
}











