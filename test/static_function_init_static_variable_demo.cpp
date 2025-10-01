#include <iostream>
#include <type_traits>
#include <stdexcept>

// 情况1：不能用静态函数初始化静态变量（错误的方式）
struct A_Wrong
{
    static void initCount() {
        count = 0;  // 错误：不能在静态函数中初始化静态变量
    }
    
    static void resetCount() {
        count = 0;
    }
    
    static int getCount() {
        return count;
    }
    
private:
    static int count;  // 静态成员变量：未初始化
};

// 情况2：正确的静态变量初始化方式
struct A_Correct
{
    static void resetCount() {
        count = 0;  // 正确：可以修改已初始化的静态变量
    }
    
    static int getCount() {
        return count;
    }
    
private:
    static int count;  // 静态成员变量：已初始化
};

// 静态成员变量定义（必须初始化）
int A_Correct::count = 0;

// 情况3：使用静态函数返回静态变量
struct A_StaticFunction
{
    static int& getCount() {
        static int count = 0;  // 静态局部变量：自动初始化
        return count;
    }
    
    static void resetCount() {
        getCount() = 0;
    }
    
    static void setCount(int value) {
        getCount() = value;
    }
};

// 情况4：使用静态函数初始化静态变量（通过构造函数）
struct A_Constructor
{
    A_Constructor() {
        if (!initialized) {
            count = 0;
            initialized = true;
        }
    }
    
    static void resetCount() {
        count = 0;
    }
    
    static int getCount() {
        return count;
    }
    
private:
    static int count;
    static bool initialized;
};

// 静态成员变量定义
int A_Constructor::count = 0;
bool A_Constructor::initialized = false;

// 情况5：使用静态函数初始化静态变量（通过静态成员函数）
struct A_StaticInit
{
    static void init() {
        if (!initialized) {
            count = 0;
            initialized = true;
        }
    }
    
    static void resetCount() {
        count = 0;
    }
    
    static int getCount() {
        return count;
    }
    
private:
    static int count;
    static bool initialized;
};

// 静态成员变量定义
int A_StaticInit::count = 0;
bool A_StaticInit::initialized = false;

int main()
{
    std::cout << "=== 分析是否能用静态函数初始化静态变量 ===" << std::endl;
    
    std::cout << "\n1. 不能用静态函数初始化静态变量（错误的方式）：" << std::endl;
    std::cout << "struct A_Wrong {" << std::endl;
    std::cout << "    static void initCount() {" << std::endl;
    std::cout << "        count = 0;  // 错误：不能在静态函数中初始化静态变量" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;  // 未初始化" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n问题：" << std::endl;
    std::cout << "- 静态函数不能初始化静态变量" << std::endl;
    std::cout << "- 静态变量必须在类外定义" << std::endl;
    std::cout << "- 静态函数只能修改已初始化的静态变量" << std::endl;
    
    std::cout << "\n2. 正确的静态变量初始化方式：" << std::endl;
    std::cout << "struct A_Correct {" << std::endl;
    std::cout << "    static void resetCount() {" << std::endl;
    std::cout << "        count = 0;  // 正确：可以修改已初始化的静态变量" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    static int count;  // 已初始化" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "int A_Correct::count = 0;  // 必须初始化" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 静态变量在类外初始化" << std::endl;
    std::cout << "- 静态函数可以修改已初始化的静态变量" << std::endl;
    std::cout << "- 调用时不会出错" << std::endl;
    
    // 测试正确的静态变量初始化
    std::cout << "\n测试 A_Correct：" << std::endl;
    A_Correct::resetCount();
    std::cout << "当前 count = " << A_Correct::getCount() << std::endl;
    
    std::cout << "\n3. 使用静态函数返回静态变量：" << std::endl;
    std::cout << "struct A_StaticFunction {" << std::endl;
    std::cout << "    static int& getCount() {" << std::endl;
    std::cout << "        static int count = 0;  // 静态局部变量：自动初始化" << std::endl;
    std::cout << "        return count;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 静态局部变量自动初始化" << std::endl;
    std::cout << "- 不需要在类外定义" << std::endl;
    std::cout << "- 延迟初始化" << std::endl;
    
    // 测试静态函数返回静态变量
    std::cout << "\n测试 A_StaticFunction：" << std::endl;
    A_StaticFunction::resetCount();
    std::cout << "当前 count = " << A_StaticFunction::getCount() << std::endl;
    A_StaticFunction::setCount(42);
    std::cout << "设置后 count = " << A_StaticFunction::getCount() << std::endl;
    
    std::cout << "\n4. 使用静态函数初始化静态变量（通过构造函数）：" << std::endl;
    std::cout << "struct A_Constructor {" << std::endl;
    std::cout << "    A_Constructor() {" << std::endl;
    std::cout << "        if (!initialized) {" << std::endl;
    std::cout << "            count = 0;" << std::endl;
    std::cout << "            initialized = true;" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 通过构造函数初始化" << std::endl;
    std::cout << "- 延迟初始化" << std::endl;
    std::cout << "- 只初始化一次" << std::endl;
    
    // 测试构造函数初始化
    std::cout << "\n测试 A_Constructor：" << std::endl;
    A_Constructor a1;
    std::cout << "构造 a1 后 count = " << A_Constructor::getCount() << std::endl;
    A_Constructor a2;
    std::cout << "构造 a2 后 count = " << A_Constructor::getCount() << std::endl;
    
    std::cout << "\n5. 使用静态函数初始化静态变量（通过静态成员函数）：" << std::endl;
    std::cout << "struct A_StaticInit {" << std::endl;
    std::cout << "    static void init() {" << std::endl;
    std::cout << "        if (!initialized) {" << std::endl;
    std::cout << "            count = 0;" << std::endl;
    std::cout << "            initialized = true;" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "- 通过静态成员函数初始化" << std::endl;
    std::cout << "- 延迟初始化" << std::endl;
    std::cout << "- 只初始化一次" << std::endl;
    
    // 测试静态成员函数初始化
    std::cout << "\n测试 A_StaticInit：" << std::endl;
    A_StaticInit::init();
    std::cout << "初始化后 count = " << A_StaticInit::getCount() << std::endl;
    A_StaticInit::resetCount();
    std::cout << "重置后 count = " << A_StaticInit::getCount() << std::endl;
    
    std::cout << "\n=== 关键区别 ===" << std::endl;
    
    std::cout << "\n1. 静态变量初始化方式：" << std::endl;
    std::cout << "   - 类外定义：int A::count = 0;" << std::endl;
    std::cout << "   - 静态局部变量：static int count = 0;" << std::endl;
    std::cout << "   - 构造函数初始化：A() { count = 0; }" << std::endl;
    std::cout << "   - 静态成员函数初始化：static void init() { count = 0; }" << std::endl;
    
    std::cout << "\n2. 初始化时机：" << std::endl;
    std::cout << "   - 类外定义：程序启动时初始化" << std::endl;
    std::cout << "   - 静态局部变量：第一次调用时初始化" << std::endl;
    std::cout << "   - 构造函数初始化：创建对象时初始化" << std::endl;
    std::cout << "   - 静态成员函数初始化：调用函数时初始化" << std::endl;
    
    std::cout << "\n3. 初始化次数：" << std::endl;
    std::cout << "   - 类外定义：只初始化一次" << std::endl;
    std::cout << "   - 静态局部变量：只初始化一次" << std::endl;
    std::cout << "   - 构造函数初始化：每次创建对象时初始化" << std::endl;
    std::cout << "   - 静态成员函数初始化：只初始化一次" << std::endl;
    
    std::cout << "\n=== 实际应用示例 ===" << std::endl;
    
    std::cout << "\n1. 单例模式（静态局部变量）：" << std::endl;
    std::cout << "class Singleton {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static Singleton& getInstance() {" << std::endl;
    std::cout << "        static Singleton instance;  // 自动初始化" << std::endl;
    std::cout << "        return instance;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n2. 计数器（静态局部变量）：" << std::endl;
    std::cout << "class Counter {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static int& getCount() {" << std::endl;
    std::cout << "        static int count = 0;  // 自动初始化" << std::endl;
    std::cout << "        return count;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "是否能用静态函数初始化静态变量：" << std::endl;
    std::cout << "1. 不能直接初始化静态成员变量" << std::endl;
    std::cout << "2. 可以修改已初始化的静态成员变量" << std::endl;
    std::cout << "3. 可以使用静态局部变量" << std::endl;
    std::cout << "4. 可以通过构造函数初始化" << std::endl;
    std::cout << "5. 可以通过静态成员函数初始化" << std::endl;
    
    std::cout << "\n建议：" << std::endl;
    std::cout << "1. 使用类外定义初始化静态成员变量" << std::endl;
    std::cout << "2. 使用静态局部变量实现延迟初始化" << std::endl;
    std::cout << "3. 使用构造函数初始化静态成员变量" << std::endl;
    std::cout << "4. 使用静态成员函数初始化静态成员变量" << std::endl;
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. 静态成员变量必须在类外定义" << std::endl;
    std::cout << "2. 静态函数不能直接初始化静态成员变量" << std::endl;
    std::cout << "3. 静态局部变量自动初始化" << std::endl;
    std::cout << "4. 选择合适的初始化方式" << std::endl;
    
    return 0;
}



