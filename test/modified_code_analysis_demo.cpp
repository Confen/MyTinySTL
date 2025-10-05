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
    
    std::cout << "=== 分析修改后的代码 ===" << std::endl;
    
    // 修改后的代码
    std::cout << "\n修改后的代码执行：" << std::endl;
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
    
    std::cout << "\n=== 原因分析 ===" << std::endl;
    std::cout << "修改后的代码问题：" << std::endl;
    std::cout << "1. ++cou 在 try 块内部，在 construct() 调用之前" << std::endl;
    std::cout << "2. 即使 construct() 抛出异常，++cou 已经执行了" << std::endl;
    std::cout << "3. 异常被 catch(...) 捕获，使用 break 退出循环" << std::endl;
    std::cout << "4. 但是 ++cou 已经执行了，所以 cou 会递增" << std::endl;
    
    std::cout << "\n=== 详细执行流程 ===" << std::endl;
    std::cout << "修改后的代码执行流程：" << std::endl;
    std::cout << "第1次循环: ++cou → cou=1, construct() 成功" << std::endl;
    std::cout << "第2次循环: ++cou → cou=2, construct() 成功" << std::endl;
    std::cout << "第3次循环: ++cou → cou=3, construct() 抛出异常" << std::endl;
    std::cout << "异常被捕获: 输出'错误', break 退出循环" << std::endl;
    std::cout << "最终结果: cou=3" << std::endl;
    
    std::cout << "\n=== 关键问题 ===" << std::endl;
    std::cout << "问题1：++cou 位置错误" << std::endl;
    std::cout << "for (;i < 5;i++) {" << std::endl;
    std::cout << "    try {" << std::endl;
    std::cout << "        ++cou;              // 总是执行" << std::endl;
    std::cout << "        construct(obj);     // 可能抛出异常" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    catch (...) {" << std::endl;
    std::cout << "        break;              // 异常时退出循环" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n问题2：计数逻辑错误" << std::endl;
    std::cout << "- ++cou 在 construct() 之前执行" << std::endl;
    std::cout << "- 即使 construct() 失败，cou 也会递增" << std::endl;
    std::cout << "- 这不符合预期的计数逻辑" << std::endl;
    
    std::cout << "\n=== 正确的修复方案 ===" << std::endl;
    std::cout << "方案1：将 ++cou 移到成功时执行" << std::endl;
    std::cout << "for (;i < 5;i++) {" << std::endl;
    std::cout << "    try {" << std::endl;
    std::cout << "        construct(obj);     // 先执行可能抛出异常的代码" << std::endl;
    std::cout << "        ++cou;              // 只有成功时才递增" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    catch (...) {" << std::endl;
    std::cout << "        std::cout << \"错误\" << \" \";" << std::endl;
    std::cout << "        break;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n=== 测试正确的修复方案 ===" << std::endl;
    A::resetcount();  // 重置计数
    i = 0;
    cou = 0;
    
    for (;i < 5;i++) {
        try {
            construct(obj);
            ++cou;  // 只有成功时才递增
            std::cout << "循环 " << cou << ": construct() 成功，++cou 执行" << std::endl;
        }
        catch (...) {
            std::cout << "循环 " << (cou+1) << ": construct() 失败，输出'错误'" << std::endl;
            break;
        }
    }
    
    std::cout << "正确修复方案最终 cou 值: " << cou << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "修改后的代码问题：" << std::endl;
    std::cout << "1. ++cou 在 construct() 之前执行" << std::endl;
    std::cout << "2. 即使 construct() 失败，cou 也会递增" << std::endl;
    std::cout << "3. 这不符合预期的计数逻辑" << std::endl;
    
    std::cout << "\n正确的修复方法：" << std::endl;
    std::cout << "1. 将 ++cou 移到 construct() 成功之后" << std::endl;
    std::cout << "2. 确保只有成功时才计数" << std::endl;
    std::cout << "3. 异常时立即退出循环" << std::endl;
    
    return 0;
}













