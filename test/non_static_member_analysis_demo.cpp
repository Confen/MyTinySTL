#include <iostream>
#include <type_traits>
#include <stdexcept>

// 情况1：非静态成员变量（错误的设计）
struct A_NonStatic
{
    A_NonStatic(){
        count++;  // 每个对象都有自己的count，无法共享状态
        std::cout << "构造 A_NonStatic，count = " << count << std::endl;
        if (count == 3)
        {
            std::cout << "准备抛出异常" << std::endl;
            throw std::runtime_error("失败");
        }
    }
    static void resetcount(){ count = 0; }  // 错误：非静态成员不能有静态函数
private:
    int count = 0;  // 非静态成员变量
};

// 情况2：静态成员变量（正确的设计）
struct A_Static
{
    A_Static(){
        count++;
        std::cout << "构造 A_Static，count = " << count << std::endl;
        if (count == 3)
        {
            std::cout << "准备抛出异常" << std::endl;
            throw std::runtime_error("失败");
        }
    }
    static void resetcount(){ count = 0; }
private:
    static int count;  // 静态成员变量
};

// 静态成员变量定义
int A_Static::count = 0;

template <class T,class... Args>
void construct(T* ptr, Args ...args)
{
    if (ptr != nullptr)
    {
        std::cout << "调用 construct，准备构造对象" << std::endl;
        new(ptr) T(args...);
        std::cout << "construct 调用成功" << std::endl;
    }
}

int main()
{
    std::cout << "=== 分析如果不设其为静态成员的问题 ===" << std::endl;
    
    // 测试非静态成员变量
    std::cout << "\n测试非静态成员变量：" << std::endl;
    try {
        A_NonStatic a1;
        std::cout << "构造 a1 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a1 失败" << std::endl;
    }
    
    try {
        A_NonStatic a2;
        std::cout << "构造 a2 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a2 失败" << std::endl;
    }
    
    try {
        A_NonStatic a3;
        std::cout << "构造 a3 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a3 失败" << std::endl;
    }
    
    // 测试静态成员变量
    std::cout << "\n测试静态成员变量：" << std::endl;
    A_Static::resetcount();
    
    try {
        A_Static a1;
        std::cout << "构造 a1 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a1 失败" << std::endl;
    }
    
    try {
        A_Static a2;
        std::cout << "构造 a2 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a2 失败" << std::endl;
    }
    
    try {
        A_Static a3;
        std::cout << "构造 a3 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a3 失败" << std::endl;
    }
    
    std::cout << "\n=== 问题分析 ===" << std::endl;
    std::cout << "如果不设其为静态成员的问题：" << std::endl;
    
    std::cout << "\n1. 状态共享问题：" << std::endl;
    std::cout << "   - 非静态成员变量：每个对象都有自己的count" << std::endl;
    std::cout << "   - 静态成员变量：所有对象共享同一个count" << std::endl;
    std::cout << "   - 非静态成员无法实现全局计数功能" << std::endl;
    
    std::cout << "\n2. 内存使用问题：" << std::endl;
    std::cout << "   - 非静态成员变量：每个对象都占用内存" << std::endl;
    std::cout << "   - 静态成员变量：所有对象共享同一块内存" << std::endl;
    std::cout << "   - 非静态成员浪费内存" << std::endl;
    
    std::cout << "\n3. 功能实现问题：" << std::endl;
    std::cout << "   - 非静态成员变量：无法实现全局计数" << std::endl;
    std::cout << "   - 静态成员变量：可以实现全局计数" << std::endl;
    std::cout << "   - 非静态成员无法满足需求" << std::endl;
    
    std::cout << "\n4. 代码设计问题：" << std::endl;
    std::cout << "   - 非静态成员变量：设计不合理" << std::endl;
    std::cout << "   - 静态成员变量：设计合理" << std::endl;
    std::cout << "   - 非静态成员违反设计原则" << std::endl;
    
    std::cout << "\n=== 详细对比 ===" << std::endl;
    
    std::cout << "\n非静态成员变量的问题：" << std::endl;
    std::cout << "struct A_NonStatic {" << std::endl;
    std::cout << "    A_NonStatic(){" << std::endl;
    std::cout << "        count++;  // 每个对象都有自己的count" << std::endl;
    std::cout << "        if (count == 3) {" << std::endl;
    std::cout << "            throw std::runtime_error(\"失败\");" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    int count = 0;  // 非静态成员变量" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n问题：" << std::endl;
    std::cout << "1. 每个对象都有自己的count，无法共享状态" << std::endl;
    std::cout << "2. 无法实现全局计数功能" << std::endl;
    std::cout << "3. 浪费内存" << std::endl;
    std::cout << "4. 设计不合理" << std::endl;
    
    std::cout << "\n静态成员变量的优势：" << std::endl;
    std::cout << "struct A_Static {" << std::endl;
    std::cout << "    A_Static(){" << std::endl;
    std::cout << "        count++;  // 所有对象共享同一个count" << std::endl;
    std::cout << "        if (count == 3) {" << std::endl;
    std::cout << "            throw std::runtime_error(\"失败\");" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;  // 静态成员变量" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "1. 所有对象共享同一个count" << std::endl;
    std::cout << "2. 可以实现全局计数功能" << std::endl;
    std::cout << "3. 节省内存" << std::endl;
    std::cout << "4. 设计合理" << std::endl;
    
    std::cout << "\n=== 实际应用示例 ===" << std::endl;
    std::cout << "在STL中，静态成员变量常用于：" << std::endl;
    std::cout << "1. 全局计数器" << std::endl;
    std::cout << "2. 单例模式" << std::endl;
    std::cout << "3. 全局状态管理" << std::endl;
    std::cout << "4. 资源管理" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "如果不设其为静态成员的问题：" << std::endl;
    std::cout << "1. 状态共享问题：无法实现全局计数" << std::endl;
    std::cout << "2. 内存使用问题：浪费内存" << std::endl;
    std::cout << "3. 功能实现问题：无法满足需求" << std::endl;
    std::cout << "4. 代码设计问题：设计不合理" << std::endl;
    
    std::cout << "\n建议：" << std::endl;
    std::cout << "1. 使用静态成员变量实现全局计数" << std::endl;
    std::cout << "2. 确保静态成员变量正确初始化" << std::endl;
    std::cout << "3. 使用静态成员函数管理静态成员变量" << std::endl;
    std::cout << "4. 遵循设计原则" << std::endl;
    
    return 0;
}













