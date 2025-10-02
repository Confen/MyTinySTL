#include <iostream>
#include <type_traits>
#include <stdexcept>

struct A
{
    A(){
        count++;
        if (count == 3)
        {
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
        new(ptr) T(args...);
    }
}

int main()
{
    char buffer[sizeof(A)];
    A* obj = reinterpret_cast<A*>(buffer);
    const size_t size = 5;
    int i = 0;
    int cou = 0;
    
    std::cout << "=== 分析为什么有 break 依旧为 5 ===" << std::endl;
    
    // 原始代码（有问题的版本）
    std::cout << "\n原始代码执行：" << std::endl;
    A::resetcount();  // 重置计数
    i = 0;
    cou = 0;
    
    try {
        for (;i < size;i++ )
        {
            cou++;
            std::cout << "循环 " << cou << ": 尝试构造对象" << std::endl;
            construct(obj);
            std::cout << "循环 " << cou << ": 构造成功" << std::endl;
        }
    }
    catch(...){
        std::cout << "错误" << " ";
    }
    
    std::cout << "原始代码最终 cou 值: " << cou << std::endl;
    
    // 修复后的代码
    std::cout << "\n修复后的代码执行：" << std::endl;
    A::resetcount();  // 重置计数
    i = 0;
    cou = 0;
    
    for (;i < size;i++ )
    {
        try {
            construct(obj);
            cou++;
            std::cout << "循环 " << cou << ": 构造成功" << std::endl;
        } catch(...) {
            std::cout << "循环 " << (cou+1) << ": 构造失败，输出错误" << std::endl;
            break;  // 异常时退出循环
        }
    }
    
    std::cout << "修复后代码最终 cou 值: " << cou << std::endl;
    
    std::cout << "\n=== 原因分析 ===" << std::endl;
    std::cout << "原始代码的问题：" << std::endl;
    std::cout << "1. cou++ 在 try 块内部，在 construct() 调用之前" << std::endl;
    std::cout << "2. 即使 construct() 抛出异常，cou++ 已经执行了" << std::endl;
    std::cout << "3. 异常被 catch(...) 捕获，但循环继续执行" << std::endl;
    std::cout << "4. 所以 cou 会一直递增到 5" << std::endl;
    
    std::cout << "\n修复后代码的正确性：" << std::endl;
    std::cout << "1. cou++ 在 construct() 成功后才执行" << std::endl;
    std::cout << "2. 如果 construct() 抛出异常，cou++ 不会执行" << std::endl;
    std::cout << "3. 异常被 catch(...) 捕获，使用 break 退出循环" << std::endl;
    std::cout << "4. 所以 cou 只会在成功时递增" << std::endl;
    
    std::cout << "\n=== 关键区别 ===" << std::endl;
    std::cout << "原始代码：" << std::endl;
    std::cout << "for (;i < size;i++ )" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    cou++;              // 总是执行" << std::endl;
    std::cout << "    construct(obj);     // 可能抛出异常" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n修复后代码：" << std::endl;
    std::cout << "for (;i < size;i++ )" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    try {" << std::endl;
    std::cout << "        construct(obj);" << std::endl;
    std::cout << "        cou++;          // 只有成功时才执行" << std::endl;
    std::cout << "    } catch(...) {" << std::endl;
    std::cout << "        break;          // 异常时退出循环" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    return 0;
}





