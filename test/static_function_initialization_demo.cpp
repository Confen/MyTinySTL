#include <iostream>
#include <type_traits>
#include <stdexcept>

// 情况1：静态函数（不需要初始化）
struct A_StaticFunction
{
    // 静态函数：不需要初始化，可以直接调用
    static void staticFunction() {
        std::cout << "静态函数被调用" << std::endl;
    }
    
    // 静态函数：可以访问静态成员变量
    static void setCount(int value) {
        count = value;
        std::cout << "设置 count = " << count << std::endl;
    }
    
    // 静态函数：可以返回静态成员变量
    static int getCount() {
        return count;
    }
    
    // 静态函数：可以重置静态成员变量
    static void resetCount() {
        count = 0;
        std::cout << "重置 count = 0" << std::endl;
    }
    
private:
    static int count;  // 静态成员变量：需要初始化
};

// 静态成员变量定义（必须初始化）
int A_StaticFunction::count = 0;

// 情况2：非静态函数（需要对象实例）
struct A_NonStaticFunction
{
    // 非静态函数：需要对象实例才能调用
    void nonStaticFunction() {
        std::cout << "非静态函数被调用" << std::endl;
    }
    
    // 非静态函数：可以访问非静态成员变量
    void setValue(int value) {
        value_ = value;
        std::cout << "设置 value_ = " << value_ << std::endl;
    }
    
    // 非静态函数：可以返回非静态成员变量
    int getValue() {
        return value_;
    }
    
private:
    int value_ = 0;  // 非静态成员变量：不需要初始化
};

// 情况3：静态成员变量（必须初始化）
struct A_StaticMember
{
    static int count;           // 静态成员变量：必须初始化
    static double pi;           // 静态成员变量：必须初始化
    static std::string name;    // 静态成员变量：必须初始化
    static bool flag;           // 静态成员变量：必须初始化
};

// 静态成员变量定义（必须初始化）
int A_StaticMember::count = 0;
double A_StaticMember::pi = 3.14159;
std::string A_StaticMember::name = "MyTinySTL";
bool A_StaticMember::flag = true;

int main()
{
    std::cout << "=== 分析静态函数是否必须初始化 ===" << std::endl;
    
    std::cout << "\n1. 静态函数（不需要初始化）：" << std::endl;
    std::cout << "静态函数可以直接调用，不需要对象实例" << std::endl;
    
    // 直接调用静态函数，不需要对象实例
    A_StaticFunction::staticFunction();
    A_StaticFunction::setCount(42);
    std::cout << "当前 count = " << A_StaticFunction::getCount() << std::endl;
    A_StaticFunction::resetCount();
    std::cout << "重置后 count = " << A_StaticFunction::getCount() << std::endl;
    
    std::cout << "\n2. 非静态函数（需要对象实例）：" << std::endl;
    std::cout << "非静态函数需要对象实例才能调用" << std::endl;
    
    // 需要对象实例才能调用非静态函数
    A_NonStaticFunction obj;
    obj.nonStaticFunction();
    obj.setValue(100);
    std::cout << "当前 value_ = " << obj.getValue() << std::endl;
    
    std::cout << "\n3. 静态成员变量（必须初始化）：" << std::endl;
    std::cout << "静态成员变量必须初始化" << std::endl;
    
    // 访问静态成员变量
    std::cout << "A_StaticMember::count = " << A_StaticMember::count << std::endl;
    std::cout << "A_StaticMember::pi = " << A_StaticMember::pi << std::endl;
    std::cout << "A_StaticMember::name = " << A_StaticMember::name << std::endl;
    std::cout << "A_StaticMember::flag = " << A_StaticMember::flag << std::endl;
    
    std::cout << "\n=== 关键区别 ===" << std::endl;
    
    std::cout << "\n静态函数 vs 非静态函数：" << std::endl;
    std::cout << "1. 调用方式：" << std::endl;
    std::cout << "   - 静态函数：ClassName::functionName()" << std::endl;
    std::cout << "   - 非静态函数：object.functionName()" << std::endl;
    
    std::cout << "\n2. 初始化要求：" << std::endl;
    std::cout << "   - 静态函数：不需要初始化" << std::endl;
    std::cout << "   - 非静态函数：需要对象实例" << std::endl;
    
    std::cout << "\n3. 访问权限：" << std::endl;
    std::cout << "   - 静态函数：只能访问静态成员" << std::endl;
    std::cout << "   - 非静态函数：可以访问所有成员" << std::endl;
    
    std::cout << "\n4. 内存使用：" << std::endl;
    std::cout << "   - 静态函数：不占用对象内存" << std::endl;
    std::cout << "   - 非静态函数：占用对象内存" << std::endl;
    
    std::cout << "\n=== 实际应用示例 ===" << std::endl;
    
    std::cout << "\n1. 工具函数（静态函数）：" << std::endl;
    std::cout << "class MathUtils {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static int add(int a, int b) { return a + b; }" << std::endl;
    std::cout << "    static int multiply(int a, int b) { return a * b; }" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "// 使用：MathUtils::add(1, 2)" << std::endl;
    
    std::cout << "\n2. 单例模式（静态函数）：" << std::endl;
    std::cout << "class Singleton {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static Singleton& getInstance() {" << std::endl;
    std::cout << "        static Singleton instance;" << std::endl;
    std::cout << "        return instance;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "// 使用：Singleton::getInstance()" << std::endl;
    
    std::cout << "\n3. 工厂模式（静态函数）：" << std::endl;
    std::cout << "class Factory {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    static Object* createObject() {" << std::endl;
    std::cout << "        return new Object();" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "// 使用：Factory::createObject()" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "静态函数是否必须初始化：" << std::endl;
    std::cout << "1. 静态函数：不需要初始化" << std::endl;
    std::cout << "2. 静态函数：可以直接调用" << std::endl;
    std::cout << "3. 静态函数：不需要对象实例" << std::endl;
    std::cout << "4. 静态函数：只能访问静态成员" << std::endl;
    
    std::cout << "\n静态成员变量是否必须初始化：" << std::endl;
    std::cout << "1. 静态成员变量：必须初始化" << std::endl;
    std::cout << "2. 静态成员变量：必须在类外定义" << std::endl;
    std::cout << "3. 静态成员变量：所有对象共享" << std::endl;
    std::cout << "4. 静态成员变量：占用全局内存" << std::endl;
    
    std::cout << "\n建议：" << std::endl;
    std::cout << "1. 使用静态函数实现工具函数" << std::endl;
    std::cout << "2. 使用静态函数实现单例模式" << std::endl;
    std::cout << "3. 使用静态函数实现工厂模式" << std::endl;
    std::cout << "4. 确保静态成员变量正确初始化" << std::endl;
    
    return 0;
}



