#include <iostream>
#include <type_traits>
#include <stdexcept>

struct A
{
    A(){
        count++;
        std::cout << "构造 A，count = " << count << std::endl;
        if (count == 3)
        {
            std::cout << "准备抛出异常" << std::endl;
            throw std::runtime_error("失败");
        }
    }
    static void resetcount(){ count = 0; }
private:
    static int count;
};

// 静态成员变量定义
int A::count = 0;

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
    char buffer[sizeof(A)];
    A* obj = reinterpret_cast<A*>(buffer);
    const size_t size = 5;
    int i = 0;
    int cou = 0;
    
    std::cout << "=== 分析为什么调试后发现直接未报异常 ===" << std::endl;
    
    // 你的修改后代码
    std::cout << "\n你的修改后代码执行：" << std::endl;
    A::resetcount();  // 重置计数
    i = 0;
    cou = 0;
    
    for (;i < 5;i++) {
        try {
            ++cou;
            std::cout << "循环 " << cou << ": ++cou 已执行，值为 " << cou << std::endl;
            construct(obj);
            std::cout << "循环 " << cou << ": construct() 成功" << std::endl;
        }
        catch (...) {
            std::cout << "循环 " << cou << ": 捕获异常，输出'错误'" << std::endl;
            break;
        }
    }
    
    std::cout << "修改后代码最终 cou 值: " << cou << std::endl;
    
    std::cout << "\n=== 可能的原因分析 ===" << std::endl;
    std::cout << "为什么调试后发现直接未报异常：" << std::endl;
    
    std::cout << "\n1. 静态成员变量问题：" << std::endl;
    std::cout << "   - 静态成员变量 count 可能没有正确初始化" << std::endl;
    std::cout << "   - 或者 count 的值不是预期的值" << std::endl;
    
    std::cout << "\n2. 对象构造问题：" << std::endl;
    std::cout << "   - 对象可能没有正确构造" << std::endl;
    std::cout << "   - 或者构造过程中没有抛出异常" << std::endl;
    
    std::cout << "\n3. 异常处理问题：" << std::endl;
    std::cout << "   - 异常可能被其他地方捕获" << std::endl;
    std::cout << "   - 或者异常没有被正确抛出" << std::endl;
    
    std::cout << "\n4. 调试环境问题：" << std::endl;
    std::cout << "   - 调试器可能隐藏了异常" << std::endl;
    std::cout << "   - 或者异常处理被优化掉了" << std::endl;
    
    std::cout << "\n=== 调试建议 ===" << std::endl;
    std::cout << "1. 检查静态成员变量：" << std::endl;
    std::cout << "   - 确保 count 被正确初始化" << std::endl;
    std::cout << "   - 检查 count 的值是否正确" << std::endl;
    
    std::cout << "\n2. 检查对象构造：" << std::endl;
    std::cout << "   - 确保对象被正确构造" << std::endl;
    std::cout << "   - 检查构造过程中是否抛出异常" << std::endl;
    
    std::cout << "\n3. 检查异常处理：" << std::endl;
    std::cout << "   - 确保异常被正确抛出" << std::endl;
    std::cout << "   - 检查异常是否被正确捕获" << std::endl;
    
    std::cout << "\n4. 检查调试环境：" << std::endl;
    std::cout << "   - 确保调试器没有隐藏异常" << std::endl;
    std::cout << "   - 检查异常处理是否被优化" << std::endl;
    
    std::cout << "\n=== 测试代码 ===" << std::endl;
    std::cout << "测试静态成员变量和对象构造：" << std::endl;
    
    A::resetcount();
    std::cout << "重置后 count = " << A::count << std::endl;
    
    try {
        A a1;
        std::cout << "构造 a1 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a1 失败" << std::endl;
    }
    
    try {
        A a2;
        std::cout << "构造 a2 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a2 失败" << std::endl;
    }
    
    try {
        A a3;
        std::cout << "构造 a3 成功" << std::endl;
    } catch (...) {
        std::cout << "构造 a3 失败" << std::endl;
    }
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "调试后发现直接未报异常的可能原因：" << std::endl;
    std::cout << "1. 静态成员变量问题" << std::endl;
    std::cout << "2. 对象构造问题" << std::endl;
    std::cout << "3. 异常处理问题" << std::endl;
    std::cout << "4. 调试环境问题" << std::endl;
    
    std::cout << "\n建议：" << std::endl;
    std::cout << "1. 检查静态成员变量的初始化" << std::endl;
    std::cout << "2. 检查对象构造过程" << std::endl;
    std::cout << "3. 检查异常处理逻辑" << std::endl;
    std::cout << "4. 检查调试环境设置" << std::endl;
    
    return 0;
}













