#include "../exceptdef.h"
#include <iostream>
#include <windows.h>

// 演示类：没有自赋值检查
class BadAssignment {
    std::string* data;
public:
    BadAssignment(const std::string& s) : data(new std::string(s)) {}
    
    ~BadAssignment() { delete data; }
    
    // 危险的赋值操作符（没有自赋值检查）
    BadAssignment& operator=(const BadAssignment& other) {
        std::cout << "  执行赋值操作..." << std::endl;
        delete data;                    // 删除自己的数据
        data = new std::string(*other.data);  // 复制其他对象的数据
        return *this;
    }
    
    const std::string& getData() const { return *data; }
};

// 演示类：有自赋值检查
class GoodAssignment {
    std::string* data;
public:
    GoodAssignment(const std::string& s) : data(new std::string(s)) {}
    
    ~GoodAssignment() { delete data; }
    
    // 安全的赋值操作符（有自赋值检查）
    GoodAssignment& operator=(const GoodAssignment& other) {
        std::cout << "  执行赋值操作..." << std::endl;
        if (this != &other) {          // 自赋值检查
            std::cout << "    不是自赋值，执行复制..." << std::endl;
            delete data;
            data = new std::string(*other.data);
        } else {
            std::cout << "    是自赋值，跳过操作..." << std::endl;
        }
        return *this;
    }
    
    const std::string& getData() const { return *data; }
};

void demonstrate_self_assignment() {
    std::cout << "=== 自赋值问题演示 ===" << std::endl;
    
    // 1. 危险的赋值操作符
    std::cout << "1. 危险的赋值操作符（没有自赋值检查）:" << std::endl;
    BadAssignment bad1("hello");
    std::cout << "   原始数据: " << bad1.getData() << std::endl;
    
    try {
        bad1 = bad1;  // 自赋值
        std::cout << "   自赋值后数据: " << bad1.getData() << std::endl;
    } catch (...) {
        std::cout << "   自赋值导致程序崩溃！" << std::endl;
    }
    
    // 2. 安全的赋值操作符
    std::cout << "\n2. 安全的赋值操作符（有自赋值检查）:" << std::endl;
    GoodAssignment good1("world");
    std::cout << "   原始数据: " << good1.getData() << std::endl;
    
    good1 = good1;  // 自赋值
    std::cout << "   自赋值后数据: " << good1.getData() << std::endl;
}

void demonstrate_mystl_exception_assignment() {
    std::cout << "\n=== MyTinySTL 异常类赋值演示 ===" << std::endl;
    
    // 1. 正常赋值
    std::cout << "1. 正常赋值:" << std::endl;
    mystl::mystl_exception ex1("第一个异常");
    mystl::mystl_exception ex2("第二个异常");
    
    std::cout << "   赋值前 ex1: " << ex1.what() << std::endl;
    std::cout << "   赋值前 ex2: " << ex2.what() << std::endl;
    
    ex1 = ex2;  // 赋值
    
    std::cout << "   赋值后 ex1: " << ex1.what() << std::endl;
    std::cout << "   赋值后 ex2: " << ex2.what() << std::endl;
    
    // 2. 自赋值
    std::cout << "\n2. 自赋值:" << std::endl;
    mystl::mystl_exception ex3("自赋值测试");
    std::cout << "   自赋值前: " << ex3.what() << std::endl;
    
    ex3 = ex3;  // 自赋值
    
    std::cout << "   自赋值后: " << ex3.what() << std::endl;
}

void demonstrate_assignment_chain() {
    std::cout << "\n=== 赋值链演示 ===" << std::endl;
    
    mystl::mystl_exception ex1("异常1");
    mystl::mystl_exception ex2("异常2");
    mystl::mystl_exception ex3("异常3");
    
    std::cout << "原始状态:" << std::endl;
    std::cout << "  ex1: " << ex1.what() << std::endl;
    std::cout << "  ex2: " << ex2.what() << std::endl;
    std::cout << "  ex3: " << ex3.what() << std::endl;
    
    // 赋值链
    ex1 = ex2 = ex3;
    
    std::cout << "\n赋值链后:" << std::endl;
    std::cout << "  ex1: " << ex1.what() << std::endl;
    std::cout << "  ex2: " << ex2.what() << std::endl;
    std::cout << "  ex3: " << ex3.what() << std::endl;
}

void demonstrate_noexcept_benefits() {
    std::cout << "\n=== noexcept 的好处 ===" << std::endl;
    
    // 1. 异常安全
    std::cout << "1. 异常安全:" << std::endl;
    mystl::mystl_exception ex1("测试1");
    mystl::mystl_exception ex2("测试2");
    
    // 赋值操作不会抛出异常
    static_assert(noexcept(ex1 = ex2), "赋值操作应该是 noexcept");
    std::cout << "   赋值操作是 noexcept 的" << std::endl;
    
    // 2. 性能优化
    std::cout << "\n2. 性能优化:" << std::endl;
    std::cout << "   编译器可以优化 noexcept 函数" << std::endl;
    std::cout << "   不需要生成异常处理代码" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 赋值操作符重载演示 ===" << std::endl;
    
    demonstrate_self_assignment();
    demonstrate_mystl_exception_assignment();
    demonstrate_assignment_chain();
    demonstrate_noexcept_benefits();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}
