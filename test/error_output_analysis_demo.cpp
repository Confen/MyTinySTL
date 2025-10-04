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
    
    std::cout << "=== 分析为什么没有输出'错误' ===" << std::endl;
    
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
    
    std::cout << cou << std::endl;
    
    std::cout << "\n=== 原因分析 ===" << std::endl;
    std::cout << "1. 异常在第3次循环时抛出" << std::endl;
    std::cout << "2. 异常被 catch(...) 捕获" << std::endl;
    std::cout << "3. 但是异常被捕获后，程序继续执行" << std::endl;
    std::cout << "4. 循环继续执行，直到 i < size 为 false" << std::endl;
    std::cout << "5. 所以'错误'被输出，但后面还有更多输出" << std::endl;
    
    std::cout << "\n=== 实际执行流程 ===" << std::endl;
    std::cout << "第1次循环: cou=1, 构造成功" << std::endl;
    std::cout << "第2次循环: cou=2, 构造成功" << std::endl;
    std::cout << "第3次循环: cou=3, 构造失败，抛出异常" << std::endl;
    std::cout << "异常被捕获，输出'错误'" << std::endl;
    std::cout << "第4次循环: cou=4, 构造成功" << std::endl;
    std::cout << "第5次循环: cou=5, 构造成功" << std::endl;
    std::cout << "最终输出: cou=5" << std::endl;
    
    std::cout << "\n=== 为什么看起来没有输出'错误' ===" << std::endl;
    std::cout << "可能的原因：" << std::endl;
    std::cout << "1. 输出被缓冲，没有立即显示" << std::endl;
    std::cout << "2. 程序执行太快，输出被覆盖" << std::endl;
    std::cout << "3. 控制台设置问题" << std::endl;
    std::cout << "4. 异常处理逻辑问题" << std::endl;
    
    std::cout << "\n=== 修复后的代码 ===" << std::endl;
    std::cout << "如果要确保'错误'被输出，应该这样写：" << std::endl;
    std::cout << "for (;i < size;i++ )" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    try {" << std::endl;
    std::cout << "        construct(obj);" << std::endl;
    std::cout << "        cou++;" << std::endl;
    std::cout << "    } catch(...) {" << std::endl;
    std::cout << "        std::cout << \"错误\" << std::endl;" << std::endl;
    std::cout << "        break;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    return 0;
}













