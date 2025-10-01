#include <iostream>
#include <windows.h>

namespace B {
    struct A {
    private:
        int data;
    public:
        A(int v) : data(v) {}
        
        // 成员函数 swap
        void swap(A& other) {
            std::cout << "调用成员函数 swap" << std::endl;
            int temp = data;
            data = other.data;
            other.data = temp;
        }
        
        // 友元函数 swap（正确的定义）
        friend void swap(A& a, A& b) {
            std::cout << "调用友元函数 swap" << std::endl;
            a.swap(b);
        }
        
        // 获取数据用于显示
        int getData() const { return data; }
    };
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    B::A a(1);
    B::A b(2);
    
    std::cout << "交换前: a=" << a.getData() << ", b=" << b.getData() << std::endl;
    
    // 方法1：直接调用友元函数
    swap(a, b);  // ADL 会找到 B::swap
    
    std::cout << "交换后: a=" << a.getData() << ", b=" << b.getData() << std::endl;
    
    std::cout << "\n--- 其他调用方式 ---" << std::endl;
    
    // 方法2：显式调用友元函数（友元函数不能通过命名空间调用）
    // B::swap(a, b);  // 错误！友元函数不能这样调用
    
    // 再次调用友元函数
    swap(a, b);
    std::cout << "再次交换后: a=" << a.getData() << ", b=" << b.getData() << std::endl;
    
    // 方法3：调用成员函数
    a.swap(b);
    
    std::cout << "成员函数交换后: a=" << a.getData() << ", b=" << b.getData() << std::endl;
    
    return 0;
}
