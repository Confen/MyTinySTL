#include <iostream>
#include <memory>
#include <windows.h>

// 演示类：详细跟踪析构过程
class DestructorScopeDemo {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    DestructorScopeDemo(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        std::cout << "构造 DestructorScopeDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    DestructorScopeDemo(DestructorScopeDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 DestructorScopeDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  新对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
        
        // 转移资源所有权
        other.data_ = nullptr;
        other.value_ = -1;
        other.name_ = "moved_away";
        
        std::cout << "  原对象地址: " << &other << std::endl;
        std::cout << "  原对象数据地址: " << other.data_ << std::endl;
        std::cout << "  原对象名称: " << other.name_ << std::endl;
    }
    
    ~DestructorScopeDemo() {
        --count_;
        std::cout << "析构 DestructorScopeDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        
        // 析构函数只析构成员，不析构对象地址
        if (data_) {
            delete[] data_;
            std::cout << "  释放数据内存: " << data_ << std::endl;
        } else {
            std::cout << "  数据指针为nullptr，无需释放数据" << std::endl;
        }
        
        std::cout << "  析构函数完成，但对象地址 " << this << " 仍然存在" << std::endl;
        std::cout << "  对象地址的内存由编译器管理，不是析构函数的职责" << std::endl;
    }
    
    int getValue() const { return value_; }
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    // 打印对象状态
    void printState() const {
        std::cout << "对象状态: 地址=" << this << ", 值=" << value_ 
                  << ", 名称=" << name_ << ", 数据=" << data_ << std::endl;
    }
};

int DestructorScopeDemo::count_ = 0;

// 演示1：析构函数的作用范围
void demo_destructor_scope() {
    std::cout << "=== 演示1：析构函数的作用范围 ===" << std::endl;
    
    DestructorScopeDemo::resetCount();
    
    // 创建对象
    DestructorScopeDemo obj1(1, "original");
    std::cout << "\n移动前 obj1：" << std::endl;
    obj1.printState();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    DestructorScopeDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1：" << std::endl;
    obj1.printState();
    
    std::cout << "\n移动后 obj2：" << std::endl;
    obj2.printState();
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. obj1 的对象地址不变: " << &obj1 << std::endl;
    std::cout << "2. obj1 的成员被转移: data_=nullptr, name_=moved_away" << std::endl;
    std::cout << "3. obj1 的对象地址仍然存在" << std::endl;
}

// 演示2：显式析构原对象
void demo_explicit_destructor() {
    std::cout << "\n=== 演示2：显式析构原对象 ===" << std::endl;
    
    DestructorScopeDemo::resetCount();
    
    DestructorScopeDemo obj1(2, "source");
    DestructorScopeDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n显式析构 obj1：" << std::endl;
    obj1.~DestructorScopeDemo();
    
    std::cout << "\n析构后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n重要观察：" << std::endl;
    std::cout << "1. obj1 的对象地址仍然存在: " << &obj1 << std::endl;
    std::cout << "2. obj1 的成员被析构: data_=nullptr, name_=moved_away" << std::endl;
    std::cout << "3. 对象地址的内存由编译器管理" << std::endl;
}

// 演示3：析构函数 vs 对象地址
void demo_destructor_vs_address() {
    std::cout << "\n=== 演示3：析构函数 vs 对象地址 ===" << std::endl;
    
    std::cout << "析构函数的作用：" << std::endl;
    std::cout << "1. 析构对象的成员变量" << std::endl;
    std::cout << "2. 释放对象持有的资源" << std::endl;
    std::cout << "3. 调用成员对象的析构函数" << std::endl;
    std::cout << "4. 执行清理工作" << std::endl;
    
    std::cout << "\n析构函数不负责：" << std::endl;
    std::cout << "1. 释放对象本身的内存地址" << std::endl;
    std::cout << "2. 管理对象地址的生命周期" << std::endl;
    std::cout << "3. 控制对象地址的分配和释放" << std::endl;
    
    std::cout << "\n对象地址的管理：" << std::endl;
    std::cout << "1. 由编译器自动管理" << std::endl;
    std::cout << "2. 在作用域结束时自动释放" << std::endl;
    std::cout << "3. 不是析构函数的职责" << std::endl;
}

// 演示4：内存布局分析
void demo_memory_layout_analysis() {
    std::cout << "\n=== 演示4：内存布局分析 ===" << std::endl;
    
    DestructorScopeDemo::resetCount();
    
    DestructorScopeDemo obj1(3, "test");
    
    std::cout << "\n移动前内存布局：" << std::endl;
    std::cout << "对象地址: " << &obj1 << " (大小: " << sizeof(obj1) << " 字节)" << std::endl;
    std::cout << "成员变量: value_=" << obj1.getValue() << ", name_=" << obj1.getName() << std::endl;
    std::cout << "数据指针: " << obj1.getData() << std::endl;
    
    DestructorScopeDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后内存布局：" << std::endl;
    std::cout << "obj1 对象地址: " << &obj1 << " (大小: " << sizeof(obj1) << " 字节)" << std::endl;
    std::cout << "obj1 成员变量: value_=" << obj1.getValue() << ", name_=" << obj1.getName() << std::endl;
    std::cout << "obj1 数据指针: " << obj1.getData() << std::endl;
    
    std::cout << "obj2 对象地址: " << &obj2 << " (大小: " << sizeof(obj2) << " 字节)" << std::endl;
    std::cout << "obj2 成员变量: value_=" << obj2.getValue() << ", name_=" << obj2.getName() << std::endl;
    std::cout << "obj2 数据指针: " << obj2.getData() << std::endl;
    
    std::cout << "\n分析：" << std::endl;
    std::cout << "1. obj1 的对象地址不变" << std::endl;
    std::cout << "2. obj1 的成员被转移给 obj2" << std::endl;
    std::cout << "3. obj1 的对象地址仍然存在" << std::endl;
    std::cout << "4. 析构函数只析构成员，不析构对象地址" << std::endl;
}

// 演示5：作用域结束时的自动析构
void demo_scope_end_destructor() {
    std::cout << "\n=== 演示5：作用域结束时的自动析构 ===" << std::endl;
    
    DestructorScopeDemo::resetCount();
    
    {
        DestructorScopeDemo obj1(4, "scope_test");
        DestructorScopeDemo obj2 = std::move(obj1);
        
        std::cout << "\n作用域内状态：" << std::endl;
        std::cout << "obj1 地址: " << &obj1 << std::endl;
        std::cout << "obj2 地址: " << &obj2 << std::endl;
        std::cout << "对象计数: " << DestructorScopeDemo::getCount() << std::endl;
    }
    
    std::cout << "\n作用域结束，自动析构：" << std::endl;
    std::cout << "1. obj1 和 obj2 都被自动析构" << std::endl;
    std::cout << "2. 析构函数被调用" << std::endl;
    std::cout << "3. 对象地址的内存被编译器释放" << std::endl;
    std::cout << "4. 对象计数: " << DestructorScopeDemo::getCount() << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    DestructorScopeDemo::resetCount();
    
    demo_destructor_scope();
    demo_explicit_destructor();
    demo_destructor_vs_address();
    demo_memory_layout_analysis();
    demo_scope_end_destructor();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "原对象移动后，析构原函数的作用：" << std::endl;
    std::cout << "1. 析构原对象的成员变量" << std::endl;
    std::cout << "2. 释放原对象持有的资源" << std::endl;
    std::cout << "3. 调用成员对象的析构函数" << std::endl;
    std::cout << "4. 执行清理工作" << std::endl;
    
    std::cout << "\n析构函数不负责：" << std::endl;
    std::cout << "1. 释放原对象本身的内存地址" << std::endl;
    std::cout << "2. 管理原对象地址的生命周期" << std::endl;
    std::cout << "3. 控制原对象地址的分配和释放" << std::endl;
    
    std::cout << "\n对象地址的管理：" << std::endl;
    std::cout << "1. 由编译器自动管理" << std::endl;
    std::cout << "2. 在作用域结束时自动释放" << std::endl;
    std::cout << "3. 不是析构函数的职责" << std::endl;
    
    return 0;
}















