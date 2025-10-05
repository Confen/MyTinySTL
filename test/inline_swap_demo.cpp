#include <iostream>
#include <string>
#include <windows.h>

// 类内定义友元函数
class InlineClass {
private:
    int value;
    std::string name;
    
public:
    InlineClass(int v, const std::string& n) : value(v), name(n) {}
    
    int getValue() const { return value; }
    const std::string& getName() const { return name; }
    
    // 类内定义友元函数 - 自动内联
    friend void swap(InlineClass& a, InlineClass& b) noexcept {
        std::cout << "  类内定义友元函数：可以访问私有成员" << std::endl;
        std::swap(a.value, b.value);
        std::swap(a.name, b.name);
    }
};

// 类外定义友元函数
class OutlineClass {
private:
    int value;
    std::string name;
    
public:
    OutlineClass(int v, const std::string& n) : value(v), name(n) {}
    
    int getValue() const { return value; }
    const std::string& getName() const { return name; }
    
    // 声明友元函数
    friend void swap(OutlineClass& a, OutlineClass& b) noexcept;
};

// 类外定义友元函数
void swap(OutlineClass& a, OutlineClass& b) noexcept {
    std::cout << "  类外定义友元函数：可以访问私有成员" << std::endl;
    std::swap(a.value, b.value);
    std::swap(a.name, b.name);
}

// 内联类外定义
class InlineOutlineClass {
private:
    int value;
    std::string name;
    
public:
    InlineOutlineClass(int v, const std::string& n) : value(v), name(n) {}
    
    int getValue() const { return value; }
    const std::string& getName() const { return name; }
    
    // 声明友元函数
    friend void swap(InlineOutlineClass& a, InlineOutlineClass& b) noexcept;
};

// 内联类外定义
inline void swap(InlineOutlineClass& a, InlineOutlineClass& b) noexcept {
    std::cout << "  内联类外定义友元函数：可以访问私有成员" << std::endl;
    std::swap(a.value, b.value);
    std::swap(a.name, b.name);
}

void demonstrate_inline_vs_outline() {
    std::cout << "=== 类内定义 vs 类外定义演示 ===" << std::endl;
    
    // 1. 类内定义测试
    std::cout << "1. 类内定义友元函数:" << std::endl;
    InlineClass obj1(10, "Alice");
    InlineClass obj2(20, "Bob");
    
    std::cout << "   交换前: obj1=(" << obj1.getValue() << ", " << obj1.getName() << ")" << std::endl;
    std::cout << "   交换前: obj2=(" << obj2.getValue() << ", " << obj2.getName() << ")" << std::endl;
    
    using std::swap;
    swap(obj1, obj2);
    
    std::cout << "   交换后: obj1=(" << obj1.getValue() << ", " << obj1.getName() << ")" << std::endl;
    std::cout << "   交换后: obj2=(" << obj2.getValue() << ", " << obj2.getName() << ")" << std::endl;
    
    // 2. 类外定义测试
    std::cout << "\n2. 类外定义友元函数:" << std::endl;
    OutlineClass obj3(30, "Charlie");
    OutlineClass obj4(40, "David");
    
    std::cout << "   交换前: obj3=(" << obj3.getValue() << ", " << obj3.getName() << ")" << std::endl;
    std::cout << "   交换前: obj4=(" << obj4.getValue() << ", " << obj4.getName() << ")" << std::endl;
    
    swap(obj3, obj4);
    
    std::cout << "   交换后: obj3=(" << obj3.getValue() << ", " << obj3.getName() << ")" << std::endl;
    std::cout << "   交换后: obj4=(" << obj4.getValue() << ", " << obj4.getName() << ")" << std::endl;
    
    // 3. 内联类外定义测试
    std::cout << "\n3. 内联类外定义友元函数:" << std::endl;
    InlineOutlineClass obj5(50, "Eve");
    InlineOutlineClass obj6(60, "Frank");
    
    std::cout << "   交换前: obj5=(" << obj5.getValue() << ", " << obj5.getName() << ")" << std::endl;
    std::cout << "   交换前: obj6=(" << obj6.getValue() << ", " << obj6.getName() << ")" << std::endl;
    
    swap(obj5, obj6);
    
    std::cout << "   交换后: obj5=(" << obj5.getValue() << ", " << obj5.getName() << ")" << std::endl;
    std::cout << "   交换后: obj6=(" << obj6.getValue() << ", " << obj6.getName() << ")" << std::endl;
}

void demonstrate_inline_characteristics() {
    std::cout << "\n=== 内联特性演示 ===" << std::endl;
    
    std::cout << "1. 类内定义的特点:" << std::endl;
    std::cout << "   - 自动成为内联函数" << std::endl;
    std::cout << "   - 编译器可能内联优化" << std::endl;
    std::cout << "   - 在每个编译单元中都会生成代码" << std::endl;
    
    std::cout << "\n2. 类外定义的特点:" << std::endl;
    std::cout << "   - 需要显式声明 inline 才能内联" << std::endl;
    std::cout << "   - 只在一个编译单元中生成代码" << std::endl;
    std::cout << "   - 链接时可能被优化掉重复定义" << std::endl;
    
    std::cout << "\n3. 性能考虑:" << std::endl;
    std::cout << "   - 内联函数：编译时展开，运行时无函数调用开销" << std::endl;
    std::cout << "   - 非内联函数：运行时函数调用，有调用开销" << std::endl;
}

void demonstrate_compilation_units() {
    std::cout << "\n=== 编译单元演示 ===" << std::endl;
    
    std::cout << "1. 类内定义的问题:" << std::endl;
    std::cout << "   - 在头文件中定义" << std::endl;
    std::cout << "   - 每个包含该头文件的 .cpp 都会生成代码" << std::endl;
    std::cout << "   - 可能导致重复定义错误" << std::endl;
    
    std::cout << "\n2. 类外定义的优势:" << std::endl;
    std::cout << "   - 在 .cpp 文件中定义" << std::endl;
    std::cout << "   - 只在一个编译单元中生成代码" << std::endl;
    std::cout << "   - 避免重复定义问题" << std::endl;
    
    std::cout << "\n3. 最佳实践:" << std::endl;
    std::cout << "   - 在头文件中声明" << std::endl;
    std::cout << "   - 在 .cpp 文件中定义" << std::endl;
    std::cout << "   - 使用 inline 关键字（如果需要内联）" << std::endl;
}

void demonstrate_recommendations() {
    std::cout << "\n=== 推荐做法 ===" << std::endl;
    
    std::cout << "1. 对于 MyTinySTL 项目:" << std::endl;
    std::cout << "   - 使用类内定义（内联）" << std::endl;
    std::cout << "   - 因为都是头文件实现" << std::endl;
    std::cout << "   - 性能更好" << std::endl;
    
    std::cout << "\n2. 对于大型项目:" << std::endl;
    std::cout << "   - 使用类外定义" << std::endl;
    std::cout << "   - 避免重复定义问题" << std::endl;
    std::cout << "   - 更好的代码组织" << std::endl;
    
    std::cout << "\n3. 代码示例:" << std::endl;
    std::cout << "   // 推荐：类内定义（MyTinySTL）" << std::endl;
    std::cout << "   class MyClass {" << std::endl;
    std::cout << "   private:" << std::endl;
    std::cout << "       int value;" << std::endl;
    std::cout << "   public:" << std::endl;
    std::cout << "       friend void swap(MyClass& a, MyClass& b) noexcept {" << std::endl;
    std::cout << "           std::swap(a.value, b.value);" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "   };" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 类内定义 swap 函数演示 ===" << std::endl;
    
    demonstrate_inline_vs_outline();
    demonstrate_inline_characteristics();
    demonstrate_compilation_units();
    demonstrate_recommendations();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}




















