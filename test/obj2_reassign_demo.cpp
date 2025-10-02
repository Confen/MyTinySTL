#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 演示类：跟踪析构过程
class LifecycleDemo {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    LifecycleDemo(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        for (int i = 0; i < 3; ++i) {
            data_[i] = v + i;
        }
        std::cout << "构造 LifecycleDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    LifecycleDemo(LifecycleDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 LifecycleDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  新对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
        
        // 转移资源所有权
        other.data_ = nullptr;
        other.value_ = -1;
        other.name_ = "moved_away";
        
        std::cout << "  原对象地址: " << &other << std::endl;
        std::cout << "  原对象数据地址: " << other.data_ << std::endl;
    }
    
    ~LifecycleDemo() {
        --count_;
        std::cout << "析构 LifecycleDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        
        if (data_) {
            std::cout << "  释放数据内存: " << data_ << std::endl;
            delete[] data_;
        } else {
            std::cout << "  数据指针为nullptr，无需释放数据" << std::endl;
        }
    }
    
    int getValue() const { return value_; }
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    void printState() const {
        std::cout << "对象状态: 地址=" << this << ", 值=" << value_ 
                  << ", 名称=" << name_ << ", 数据=" << data_ << std::endl;
    }
};

int LifecycleDemo::count_ = 0;

// 演示1：obj1 和 obj2 的不同状态
void demo_obj1_vs_obj2_difference() {
    std::cout << "=== 演示1：obj1 和 obj2 的不同状态 ===" << std::endl;
    
    LifecycleDemo::resetCount();
    
    LifecycleDemo obj1(1, "source");
    LifecycleDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后状态：" << std::endl;
    std::cout << "obj1 状态：" << std::endl;
    obj1.printState();
    std::cout << "obj2 状态：" << std::endl;
    obj2.printState();
    
    std::cout << "\n关键区别：" << std::endl;
    std::cout << "1. obj1: 已被移动，处于未定义状态" << std::endl;
    std::cout << "2. obj2: 正常对象，处于有效状态" << std::endl;
    std::cout << "3. obj1 需要显式析构才能重新使用" << std::endl;
    std::cout << "4. obj2 可以直接重新赋值" << std::endl;
}

// 演示2：obj1 需要显式析构的原因
void demo_why_obj1_needs_explicit_destructor() {
    std::cout << "\n=== 演示2：obj1 需要显式析构的原因 ===" << std::endl;
    
    LifecycleDemo::resetCount();
    
    LifecycleDemo obj1(2, "source");
    LifecycleDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n为什么 obj1 需要显式析构：" << std::endl;
    std::cout << "1. obj1 已被移动，处于未定义状态" << std::endl;
    std::cout << "2. obj1 的对象内存仍然被占用" << std::endl;
    std::cout << "3. obj1 的成员已被转移，但对象本身仍存在" << std::endl;
    std::cout << "4. 必须显式析构才能清理 obj1 的状态" << std::endl;
    
    std::cout << "\n显式析构 obj1：" << std::endl;
    obj1.~LifecycleDemo();
    
    std::cout << "\n使用 placement new 重新构造 obj1：" << std::endl;
    new(&obj1) LifecycleDemo(5, "reconstructed");
    
    std::cout << "\n重新构造后 obj1 状态：" << std::endl;
    obj1.printState();
}

// 演示3：obj2 可以直接重新赋值的原因
void demo_why_obj2_can_reassign_directly() {
    std::cout << "\n=== 演示3：obj2 可以直接重新赋值的原因 ===" << std::endl;
    
    LifecycleDemo::resetCount();
    
    LifecycleDemo obj1(3, "source");
    LifecycleDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj2 状态：" << std::endl;
    obj2.printState();
    
    std::cout << "\n为什么 obj2 可以直接重新赋值：" << std::endl;
    std::cout << "1. obj2 是正常对象，处于有效状态" << std::endl;
    std::cout << "2. obj2 没有被移动，对象状态完整" << std::endl;
    std::cout << "3. obj2 可以直接调用赋值操作符" << std::endl;
    std::cout << "4. 赋值操作符会自动处理资源管理" << std::endl;
    
    std::cout << "\n直接重新赋值 obj2：" << std::endl;
    obj2 = LifecycleDemo(6, "reassigned");
    
    std::cout << "\n重新赋值后 obj2 状态：" << std::endl;
    obj2.printState();
}

// 演示4：对象状态 vs 对象生命周期
void demo_object_state_vs_lifecycle() {
    std::cout << "\n=== 演示4：对象状态 vs 对象生命周期 ===" << std::endl;
    
    std::cout << "对象状态：" << std::endl;
    std::cout << "1. 有效状态：对象可以正常使用" << std::endl;
    std::cout << "2. 未定义状态：对象已被移动，不能使用" << std::endl;
    std::cout << "3. 已析构状态：对象已被析构，不能使用" << std::endl;
    
    std::cout << "\n对象生命周期：" << std::endl;
    std::cout << "1. 构造：对象被创建" << std::endl;
    std::cout << "2. 使用：对象处于有效状态" << std::endl;
    std::cout << "3. 析构：对象被销毁" << std::endl;
    
    std::cout << "\n关键理解：" << std::endl;
    std::cout << "1. obj1 被移动后：状态变为未定义，但生命周期仍在继续" << std::endl;
    std::cout << "2. obj2 获得资源后：状态为有效，生命周期正常" << std::endl;
    std::cout << "3. 未定义状态的对象：需要显式析构才能重新使用" << std::endl;
    std::cout << "4. 有效状态的对象：可以直接重新赋值" << std::endl;
}

// 演示5：实际应用中的最佳实践
void demo_best_practices() {
    std::cout << "\n=== 演示5：实际应用中的最佳实践 ===" << std::endl;
    
    std::cout << "最佳实践：" << std::endl;
    std::cout << "1. 避免显式析构：除非必要，避免显式析构" << std::endl;
    std::cout << "2. 使用 RAII：让编译器管理对象生命周期" << std::endl;
    std::cout << "3. 移动后不要使用原对象：让原对象自然析构" << std::endl;
    std::cout << "4. 直接重新赋值：对于有效对象，直接重新赋值" << std::endl;
    
    std::cout << "\n推荐做法：" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    LifecycleDemo obj1(1, \"source\");" << std::endl;
    std::cout << "    LifecycleDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // obj1 在作用域结束时自动析构" << std::endl;
    std::cout << "    // obj2 可以直接重新赋值" << std::endl;
    std::cout << "    obj2 = LifecycleDemo(2, \"new_value\");" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n特殊情况：" << std::endl;
    std::cout << "LifecycleDemo obj1(1, \"source\");" << std::endl;
    std::cout << "LifecycleDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~LifecycleDemo(); // 显式析构" << std::endl;
    std::cout << "new(&obj1) LifecycleDemo(3, \"reconstructed\"); // 重新构造" << std::endl;
    std::cout << "obj2 = LifecycleDemo(4, \"reassigned\"); // 直接重新赋值" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    LifecycleDemo::resetCount();
    
    demo_obj1_vs_obj2_difference();
    demo_why_obj1_needs_explicit_destructor();
    demo_why_obj2_can_reassign_directly();
    demo_object_state_vs_lifecycle();
    demo_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么 obj2 不需要显式析构就可以重新分配值：" << std::endl;
    std::cout << "1. obj2 是正常对象：处于有效状态，可以正常使用" << std::endl;
    std::cout << "2. obj1 已被移动：处于未定义状态，需要显式析构" << std::endl;
    std::cout << "3. 对象状态不同：有效状态 vs 未定义状态" << std::endl;
    std::cout << "4. 生命周期管理：有效对象可以直接重新赋值" << std::endl;
    std::cout << "5. 资源管理：赋值操作符会自动处理资源管理" << std::endl;
    
    return 0;
}







