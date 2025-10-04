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
    
    std::cout << "=== 分析为什么异常被捕获后，循环继续执行 ===" << std::endl;
    
    // 情况1：异常处理在循环外部（原始代码）
    std::cout << "\n情况1：异常处理在循环外部（原始代码）" << std::endl;
    A::resetcount();  // 重置计数
    i = 0;
    cou = 0;
    
    try {
        for (;i < 5;i++) {
            ++cou;
            std::cout << "循环 " << cou << ": ++cou 已执行，值为 " << cou << std::endl;
            construct(obj);
            std::cout << "循环 " << cou << ": construct() 成功" << std::endl;
        }
    }
    catch (...) {
        std::cout << "循环 " << cou << ": 捕获异常，输出'错误'" << std::endl;
    }
    
    std::cout << "情况1最终 cou 值: " << cou << std::endl;
    
    // 情况2：异常处理在循环内部（修复后代码）
    std::cout << "\n情况2：异常处理在循环内部（修复后代码）" << std::endl;
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
            break;  // 异常时退出循环
        }
    }
    
    std::cout << "情况2最终 cou 值: " << cou << std::endl;
    
    std::cout << "\n=== 原因分析 ===" << std::endl;
    std::cout << "为什么异常被捕获后，循环继续执行：" << std::endl;
    
    std::cout << "\n1. 异常处理位置问题：" << std::endl;
    std::cout << "   情况1（异常处理在循环外部）：" << std::endl;
    std::cout << "   try {" << std::endl;
    std::cout << "       for (;i < 5;i++) {" << std::endl;
    std::cout << "           ++cou;" << std::endl;
    std::cout << "           construct(obj);" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "   }" << std::endl;
    std::cout << "   catch (...) {" << std::endl;
    std::cout << "       // 异常被捕获，但循环已经继续执行" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n   情况2（异常处理在循环内部）：" << std::endl;
    std::cout << "   for (;i < 5;i++) {" << std::endl;
    std::cout << "       try {" << std::endl;
    std::cout << "           construct(obj);" << std::endl;
    std::cout << "           ++cou;" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "       catch (...) {" << std::endl;
    std::cout << "           break;  // 异常时退出循环" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n2. 执行顺序问题：" << std::endl;
    std::cout << "   情况1：" << std::endl;
    std::cout << "   - ++cou 在 construct() 之前执行" << std::endl;
    std::cout << "   - 即使 construct() 抛出异常，++cou 已经执行了" << std::endl;
    std::cout << "   - 异常被 catch(...) 捕获，但循环已经继续执行" << std::endl;
    std::cout << "   - 所以 cou 会一直递增到 5" << std::endl;
    
    std::cout << "\n   情况2：" << std::endl;
    std::cout << "   - construct() 先执行" << std::endl;
    std::cout << "   - 如果成功，++cou 执行" << std::endl;
    std::cout << "   - 如果失败，异常被 catch(...) 捕获，break 退出循环" << std::endl;
    std::cout << "   - 所以 cou 只会在成功时递增" << std::endl;
    
    std::cout << "\n3. 循环控制问题：" << std::endl;
    std::cout << "   情况1：" << std::endl;
    std::cout << "   - 异常被捕获后，循环继续执行" << std::endl;
    std::cout << "   - 没有 break 语句" << std::endl;
    std::cout << "   - 所以循环会一直执行到结束" << std::endl;
    
    std::cout << "\n   情况2：" << std::endl;
    std::cout << "   - 异常被捕获后，使用 break 退出循环" << std::endl;
    std::cout << "   - 所以循环会立即停止" << std::endl;
    
    std::cout << "\n=== 详细执行流程对比 ===" << std::endl;
    
    std::cout << "\n情况1（异常处理在循环外部）执行流程：" << std::endl;
    std::cout << "第1次循环: ++cou → cou=1, construct() 成功" << std::endl;
    std::cout << "第2次循环: ++cou → cou=2, construct() 成功" << std::endl;
    std::cout << "第3次循环: ++cou → cou=3, construct() 抛出异常" << std::endl;
    std::cout << "异常被捕获: 输出'错误'" << std::endl;
    std::cout << "第4次循环: ++cou → cou=4, construct() 成功" << std::endl;
    std::cout << "第5次循环: ++cou → cou=5, construct() 成功" << std::endl;
    std::cout << "最终结果: cou=5" << std::endl;
    
    std::cout << "\n情况2（异常处理在循环内部）执行流程：" << std::endl;
    std::cout << "第1次循环: construct() 成功, ++cou → cou=1" << std::endl;
    std::cout << "第2次循环: construct() 成功, ++cou → cou=2" << std::endl;
    std::cout << "第3次循环: construct() 抛出异常" << std::endl;
    std::cout << "异常被捕获: 输出'错误', break 退出循环" << std::endl;
    std::cout << "最终结果: cou=2" << std::endl;
    
    std::cout << "\n=== 关键区别 ===" << std::endl;
    std::cout << "1. 异常处理位置：" << std::endl;
    std::cout << "   - 情况1：异常处理在循环外部" << std::endl;
    std::cout << "   - 情况2：异常处理在循环内部" << std::endl;
    
    std::cout << "\n2. 执行顺序：" << std::endl;
    std::cout << "   - 情况1：++cou 在 construct() 之前执行" << std::endl;
    std::cout << "   - 情况2：++cou 在 construct() 成功之后执行" << std::endl;
    
    std::cout << "\n3. 循环控制：" << std::endl;
    std::cout << "   - 情况1：异常时循环继续执行" << std::endl;
    std::cout << "   - 情况2：异常时立即退出循环" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "异常被捕获后，循环继续执行的原因：" << std::endl;
    std::cout << "1. 异常处理在循环外部" << std::endl;
    std::cout << "2. ++cou 在 construct() 之前执行" << std::endl;
    std::cout << "3. 没有 break 语句" << std::endl;
    std::cout << "4. 循环会一直执行到结束" << std::endl;
    
    std::cout << "\n正确的修复方法：" << std::endl;
    std::cout << "1. 将异常处理移到循环内部" << std::endl;
    std::cout << "2. 将 ++cou 移到 construct() 成功之后" << std::endl;
    std::cout << "3. 使用 break 在异常时退出循环" << std::endl;
    
    return 0;
}












