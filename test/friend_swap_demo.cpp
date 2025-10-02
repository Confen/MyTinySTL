#include <iostream>
#include <string>
#include <windows.h>

// 使用友元函数的类
class FriendClass {
private:
    int value;
    std::string name;
    
public:
    FriendClass(int v, const std::string& n) : value(v), name(n) {}
    
    int getValue() const { return value; }
    const std::string& getName() const { return name; }
    
    // 友元函数 - 可以访问私有成员
    friend void swap(FriendClass& a, FriendClass& b) noexcept {
        std::cout << "  友元函数：直接访问私有成员" << std::endl;
        std::swap(a.value, b.value);
        std::swap(a.name, b.name);
    }
    
    // 成员函数
    void memberSwap(FriendClass& other) noexcept {
        std::cout << "  成员函数：通过成员函数交换" << std::endl;
        std::swap(value, other.value);
        std::swap(name, other.name);
    }
};

// 不使用友元函数的类
class NonFriendClass {
private:
    int value;
    std::string name;
    
public:
    NonFriendClass(int v, const std::string& n) : value(v), name(n) {}
    
    int getValue() const { return value; }
    const std::string& getName() const { return name; }
    
    // 成员函数
    void swap(NonFriendClass& other) noexcept {
        std::cout << "  成员函数：交换私有成员" << std::endl;
        std::swap(value, other.value);
        std::swap(name, other.name);
    }
};

// 非友元函数 - 无法直接访问私有成员
void swap(NonFriendClass& a, NonFriendClass& b) noexcept {
    std::cout << "  非友元函数：通过成员函数交换" << std::endl;
    a.swap(b);  // 必须通过成员函数
}

void demonstrate_friend_vs_non_friend() {
    std::cout << "=== 友元函数 vs 非友元函数演示 ===" << std::endl;
    
    // 1. 友元函数演示
    std::cout << "1. 友元函数演示:" << std::endl;
    FriendClass obj1(10, "Alice");
    FriendClass obj2(20, "Bob");
    
    std::cout << "   交换前: obj1=(" << obj1.getValue() << ", " << obj1.getName() << ")" << std::endl;
    std::cout << "   交换前: obj2=(" << obj2.getValue() << ", " << obj2.getName() << ")" << std::endl;
    
    using std::swap;
    swap(obj1, obj2);  // 调用友元函数
    
    std::cout << "   交换后: obj1=(" << obj1.getValue() << ", " << obj1.getName() << ")" << std::endl;
    std::cout << "   交换后: obj2=(" << obj2.getValue() << ", " << obj2.getName() << ")" << std::endl;
    
    // 2. 非友元函数演示
    std::cout << "\n2. 非友元函数演示:" << std::endl;
    NonFriendClass obj3(30, "Charlie");
    NonFriendClass obj4(40, "David");
    
    std::cout << "   交换前: obj3=(" << obj3.getValue() << ", " << obj3.getName() << ")" << std::endl;
    std::cout << "   交换前: obj4=(" << obj4.getValue() << ", " << obj4.getName() << ")" << std::endl;
    
    using std::swap;
    swap(obj3, obj4);  // 调用非友元函数
    
    std::cout << "   交换后: obj3=(" << obj3.getValue() << ", " << obj3.getName() << ")" << std::endl;
    std::cout << "   交换后: obj4=(" << obj4.getValue() << ", " << obj4.getName() << ")" << std::endl;
}

void demonstrate_performance_difference() {
    std::cout << "\n=== 性能差异演示 ===" << std::endl;
    
    std::cout << "1. 友元函数性能:" << std::endl;
    std::cout << "   - 直接访问私有成员" << std::endl;
    std::cout << "   - 无需额外的函数调用" << std::endl;
    std::cout << "   - 编译器可以更好地优化" << std::endl;
    
    std::cout << "\n2. 非友元函数性能:" << std::endl;
    std::cout << "   - 必须通过成员函数访问" << std::endl;
    std::cout << "   - 有额外的函数调用开销" << std::endl;
    std::cout << "   - 可能影响编译器优化" << std::endl;
}

void demonstrate_encapsulation() {
    std::cout << "\n=== 封装性演示 ===" << std::endl;
    
    std::cout << "1. 友元函数的封装性:" << std::endl;
    std::cout << "   - 友元函数是类的特殊朋友" << std::endl;
    std::cout << "   - 可以访问私有成员，但仍然是外部函数" << std::endl;
    std::cout << "   - 不会破坏封装性，因为需要显式声明" << std::endl;
    
    std::cout << "\n2. 为什么需要友元？" << std::endl;
    std::cout << "   - 某些操作需要访问私有成员" << std::endl;
    std::cout << "   - 但又不适合作为成员函数" << std::endl;
    std::cout << "   - 比如对称操作：swap(a, b) 和 swap(b, a)" << std::endl;
}

void demonstrate_swap_design_pattern() {
    std::cout << "\n=== swap 设计模式 ===" << std::endl;
    
    std::cout << "1. 标准 swap 设计模式:" << std::endl;
    std::cout << "   - 提供成员函数 swap" << std::endl;
    std::cout << "   - 提供友元函数 swap" << std::cout << std::endl;
    std::cout << "   - 友元函数调用成员函数" << std::endl;
    
    std::cout << "\n2. 为什么这样设计？" << std::endl;
    std::cout << "   - 成员函数：内部使用" << std::endl;
    std::cout << "   - 友元函数：外部接口" << std::endl;
    std::cout << "   - 保持接口的一致性" << std::endl;
    
    std::cout << "\n3. 实际代码示例:" << std::endl;
    std::cout << "   class MyClass {" << std::endl;
    std::cout << "   private:" << std::endl;
    std::cout << "       int value;" << std::endl;
    std::cout << "   public:" << std::endl;
    std::cout << "       void swap(MyClass& other) { /* 实现 */ }" << std::endl;
    std::cout << "       friend void swap(MyClass& a, MyClass& b) {" << std::endl;
    std::cout << "           a.swap(b);" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "   };" << std::endl;
}

void demonstrate_alternatives() {
    std::cout << "\n=== 替代方案 ===" << std::endl;
    
    std::cout << "1. 不使用友元函数的替代方案:" << std::endl;
    std::cout << "   - 提供公共的 getter/setter 方法" << std::endl;
    std::cout << "   - 但这会破坏封装性" << std::endl;
    std::cout << "   - 性能也不如直接访问" << std::endl;
    
    std::cout << "\n2. 只使用成员函数:" << std::endl;
    std::cout << "   - 但这样不符合标准库的接口" << std::endl;
    std::cout << "   - 无法与 ADL 配合使用" << std::endl;
    std::cout << "   - 破坏了接口的一致性" << std::endl;
    
    std::cout << "\n3. 友元函数是最佳选择:" << std::endl;
    std::cout << "   - 保持封装性" << std::endl;
    std::cout << "   - 提供最佳性能" << std::endl;
    std::cout << "   - 符合标准库设计" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 为什么使用友元函数 ===" << std::endl;
    
    demonstrate_friend_vs_non_friend();
    demonstrate_performance_difference();
    demonstrate_encapsulation();
    demonstrate_swap_design_pattern();
    demonstrate_alternatives();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}











