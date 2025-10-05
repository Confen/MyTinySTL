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
    static void resetcount(){ count = 0; }  // 修复：移除int声明
private:
    static int count;  // 修复：添加static声明
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
    
    std::cout << "=== 分析为什么 cou 为 5 ===" << std::endl;
    
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
        std::cout << "循环 " << cou << ": 捕获异常" << std::endl;
    }
    
    std::cout << "最终 cou 值: " << cou << std::endl;
    
    std::cout << "\n=== 原因分析 ===" << std::endl;
    std::cout << "1. cou++ 在 try 块内部，在 construct() 调用之前" << std::endl;
    std::cout << "2. 即使 construct() 抛出异常，cou++ 已经执行了" << std::endl;
    std::cout << "3. 循环继续执行，直到 i < size 为 false" << std::endl;
    std::cout << "4. 所以 cou 会一直递增到 5" << std::endl;
    
    std::cout << "\n=== 修复后的代码 ===" << std::endl;
    std::cout << "如果要让 cou 在异常时停止递增，应该这样写：" << std::endl;
    std::cout << "for (;i < size;i++ )" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    try {" << std::endl;
    std::cout << "        construct(obj);" << std::endl;
    std::cout << "        cou++;  // 只有成功时才递增" << std::endl;
    std::cout << "    } catch(...) {" << std::endl;
    std::cout << "        break;  // 异常时退出循环" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    return 0;
}














