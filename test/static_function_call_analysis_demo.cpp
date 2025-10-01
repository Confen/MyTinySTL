#include <iostream>
#include <type_traits>
#include <stdexcept>

// 情况1：静态函数可以调用，但静态成员变量未初始化
struct A_Uninitialized
{
    static void resetcount(){ 
        count = 0;  // 错误：count 未初始化
        std::cout << "resetcount 被调用，count = " << count << std::endl;
    }
    
    static void setCount(int value) {
        count = value;  // 错误：count 未初始化
        std::cout << "setCount 被调用，count = " << count << std::endl;
    }
    
    static int getCount() {
        return count;  // 错误：count 未初始化
    }
    
private:
    static int count;  // 静态成员变量：未初始化
};

// 情况2：静态函数可以调用，静态成员变量已初始化
struct A_Initialized
{
    static void resetcount(){ 
        count = 0;
        std::cout << "resetcount 被调用，count = " << count << std::endl;
    }
    
    static void setCount(int value) {
        count = value;
        std::cout << "setCount 被调用，count = " << count << std::endl;
    }
    
    static int getCount() {
        return count;
    }
    
private:
    static int count;  // 静态成员变量：已初始化
};

// 静态成员变量定义（必须初始化）
int A_Initialized::count = 0;

// 情况3：只有静态函数，没有静态成员变量
struct A_OnlyFunction
{
    static void resetcount(){ 
        std::cout << "resetcount 被调用（没有静态成员变量）" << std::endl;
    }
    
    static void setCount(int value) {
        std::cout << "setCount 被调用，value = " << value << std::endl;
    }
    
    static int getCount() {
        return 42;  // 返回固定值
    }
};

int main()
{
    std::cout << "=== 分析为什么直接调用静态函数不行 ===" << std::endl;
    
    std::cout << "\n1. 静态函数可以调用，但静态成员变量未初始化：" << std::endl;
    std::cout << "struct A_Uninitialized {" << std::endl;
    std::cout << "    static void resetcount(){ count = 0; }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;  // 未初始化" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n问题：" << std::endl;
    std::cout << "- 静态函数可以调用" << std::endl;
    std::cout << "- 但静态成员变量 count 未初始化" << std::endl;
    std::cout << "- 调用时会出错" << std::endl;
    
    // 尝试调用未初始化的静态函数
    try {
        std::cout << "\n尝试调用 A_Uninitialized::resetcount()：" << std::endl;
        A_Uninitialized::resetcount();  // 这里会出错
    } catch (...) {
        std::cout << "调用失败：静态成员变量未初始化" << std::endl;
    }
    
    std::cout << "\n2. 静态函数可以调用，静态成员变量已初始化：" << std::endl;
    std::cout << "struct A_Initialized {" << std::endl;
    std::cout << "    static void resetcount(){ count = 0; }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;  // 已初始化" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "int A_Initialized::count = 0;  // 必须初始化" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 静态函数可以调用" << std::endl;
    std::cout << "- 静态成员变量 count 已初始化" << std::endl;
    std::cout << "- 调用时不会出错" << std::endl;
    
    // 调用已初始化的静态函数
    std::cout << "\n调用 A_Initialized::resetcount()：" << std::endl;
    A_Initialized::resetcount();
    
    std::cout << "\n调用 A_Initialized::setCount(42)：" << std::endl;
    A_Initialized::setCount(42);
    
    std::cout << "\n调用 A_Initialized::getCount()：" << std::endl;
    std::cout << "当前 count = " << A_Initialized::getCount() << std::endl;
    
    std::cout << "\n3. 只有静态函数，没有静态成员变量：" << std::endl;
    std::cout << "struct A_OnlyFunction {" << std::endl;
    std::cout << "    static void resetcount(){ ... }" << std::endl;
    std::cout << "    static int getCount() { return 42; }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 静态函数可以调用" << std::endl;
    std::cout << "- 没有静态成员变量" << std::endl;
    std::cout << "- 调用时不会出错" << std::endl;
    
    // 调用只有静态函数的类
    std::cout << "\n调用 A_OnlyFunction::resetcount()：" << std::endl;
    A_OnlyFunction::resetcount();
    
    std::cout << "\n调用 A_OnlyFunction::setCount(100)：" << std::endl;
    A_OnlyFunction::setCount(100);
    
    std::cout << "\n调用 A_OnlyFunction::getCount()：" << std::endl;
    std::cout << "当前 count = " << A_OnlyFunction::getCount() << std::endl;
    
    std::cout << "\n=== 关键区别 ===" << std::endl;
    
    std::cout << "\n1. 静态函数 vs 静态成员变量：" << std::endl;
    std::cout << "   - 静态函数：不需要初始化" << std::endl;
    std::cout << "   - 静态成员变量：必须初始化" << std::endl;
    std::cout << "   - 静态函数可以调用" << std::endl;
    std::cout << "   - 静态成员变量必须定义" << std::endl;
    
    std::cout << "\n2. 调用方式：" << std::endl;
    std::cout << "   - 静态函数：ClassName::functionName()" << std::endl;
    std::cout << "   - 静态成员变量：ClassName::variableName" << std::endl;
    
    std::cout << "\n3. 初始化要求：" << std::endl;
    std::cout << "   - 静态函数：不需要初始化" << std::endl;
    std::cout << "   - 静态成员变量：必须在类外定义" << std::endl;
    
    std::cout << "\n=== 实际应用示例 ===" << std::endl;
    
    std::cout << "\n1. 正确的静态函数使用：" << std::endl;
    std::cout << "class Counter {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static void reset() { count = 0; }" << std::endl;
    std::cout << "    static void increment() { count++; }" << std::endl;
    std::cout << "    static int get() { return count; }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "int Counter::count = 0;  // 必须初始化" << std::endl;
    
    std::cout << "\n2. 使用方式：" << std::endl;
    std::cout << "Counter::reset();" << std::endl;
    std::cout << "Counter::increment();" << std::endl;
    std::cout << "int value = Counter::get();" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么直接调用静态函数不行：" << std::endl;
    std::cout << "1. 静态函数可以调用" << std::endl;
    std::cout << "2. 但静态成员变量未初始化" << std::endl;
    std::cout << "3. 调用时会出错" << std::endl;
    std::cout << "4. 需要先初始化静态成员变量" << std::endl;
    
    std::cout << "\n解决方案：" << std::endl;
    std::cout << "1. 在类外定义静态成员变量" << std::endl;
    std::cout << "2. 确保静态成员变量被正确初始化" << std::endl;
    std::cout << "3. 然后就可以调用静态函数了" << std::endl;
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. 静态函数不需要初始化" << std::endl;
    std::cout << "2. 静态成员变量必须初始化" << std::endl;
    std::cout << "3. 静态函数可以调用" << std::endl;
    std::cout << "4. 静态成员变量必须定义" << std::endl;
    
    return 0;
}



