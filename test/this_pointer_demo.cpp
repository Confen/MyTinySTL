#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：详细跟踪 this 指针
class ThisPointerDemo {
private:
    int value_;
    int* data_;
    static int count_;
    
public:
    ThisPointerDemo(int v) : value_(v) {
        ++count_;
        data_ = new int[5];
        std::cout << "构造 ThisPointerDemo(" << value_ << ")" << std::endl;
        std::cout << "  this 地址: " << this << std::endl;
        std::cout << "  对象地址: " << &(*this) << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 拷贝构造函数
    ThisPointerDemo(const ThisPointerDemo& other) : value_(other.value_) {
        ++count_;
        data_ = new int[5];
        std::cout << "拷贝构造 ThisPointerDemo(" << value_ << ")" << std::endl;
        std::cout << "  this 地址: " << this << std::endl;
        std::cout << "  对象地址: " << &(*this) << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    ThisPointerDemo(ThisPointerDemo&& other) noexcept : value_(other.value_), data_(other.data_) {
        ++count_;
        std::cout << "移动构造 ThisPointerDemo(" << value_ << ")" << std::endl;
        std::cout << "  this 地址: " << this << std::endl;
        std::cout << "  对象地址: " << &(*this) << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
        
        // 转移资源所有权
        other.data_ = nullptr;
        other.value_ = -1;
        
        std::cout << "  原对象 this 地址: " << &other << std::endl;
        std::cout << "  原对象数据地址: " << other.data_ << std::endl;
    }
    
    ~ThisPointerDemo() {
        --count_;
        std::cout << "析构 ThisPointerDemo(" << value_ << ")" << std::endl;
        std::cout << "  this 地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        if (data_) {
            delete[] data_;
        }
    }
    
    int getValue() const { return value_; }
    int* getData() const { return data_; }
    
    // 打印 this 指针信息
    void printThisInfo() const {
        std::cout << "printThisInfo() 调用：" << std::endl;
        std::cout << "  this 地址: " << this << std::endl;
        std::cout << "  对象地址: " << &(*this) << std::endl;
        std::cout << "  值: " << value_ << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
    }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int ThisPointerDemo::count_ = 0;

// 演示1：this 指针在对象生命周期中的稳定性
void demo_this_pointer_stability() {
    std::cout << "=== 演示1：this 指针在对象生命周期中的稳定性 ===" << std::endl;
    
    ThisPointerDemo::resetCount();
    
    // 创建对象
    ThisPointerDemo obj1(1);
    std::cout << "\n对象1创建完成" << std::endl;
    obj1.printThisInfo();
    
    // 调用成员函数
    std::cout << "\n调用成员函数：" << std::endl;
    obj1.printThisInfo();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    ThisPointerDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后对象1的状态：" << std::endl;
    obj1.printThisInfo();
    
    std::cout << "\n移动后对象2的状态：" << std::endl;
    obj2.printThisInfo();
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. obj1 的 this 地址在整个生命周期中不变" << std::endl;
    std::cout << "2. obj2 的 this 地址是新的地址" << std::endl;
    std::cout << "3. 移动只是转移资源，不改变对象地址" << std::endl;
}

// 演示2：this 指针与对象地址的关系
void demo_this_pointer_relationship() {
    std::cout << "\n=== 演示2：this 指针与对象地址的关系 ===" << std::endl;
    
    ThisPointerDemo::resetCount();
    
    ThisPointerDemo obj(2);
    
    std::cout << "\n对象地址分析：" << std::endl;
    std::cout << "this 地址: " << &obj << std::endl;
    std::cout << "对象地址: " << &obj << std::endl;
    std::cout << "this == &obj: " << (&obj == &obj) << std::endl;
    
    // 在成员函数中访问 this
    std::cout << "\n在成员函数中访问 this：" << std::endl;
    obj.printThisInfo();
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. this 指针指向对象本身" << std::endl;
    std::cout << "2. this 地址等于对象地址" << std::endl;
    std::cout << "3. this 地址在对象生命周期中不变" << std::endl;
}

// 演示3：移动前后 this 指针的对比
void demo_this_pointer_comparison() {
    std::cout << "\n=== 演示3：移动前后 this 指针的对比 ===" << std::endl;
    
    ThisPointerDemo::resetCount();
    
    ThisPointerDemo obj1(3);
    
    std::cout << "\n移动前 obj1：" << std::endl;
    std::cout << "this 地址: " << &obj1 << std::endl;
    std::cout << "数据地址: " << obj1.getData() << std::endl;
    
    // 移动构造
    ThisPointerDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1：" << std::endl;
    std::cout << "this 地址: " << &obj1 << std::endl;
    std::cout << "数据地址: " << obj1.getData() << std::endl;
    
    std::cout << "\n移动后 obj2：" << std::endl;
    std::cout << "this 地址: " << &obj2 << std::endl;
    std::cout << "数据地址: " << obj2.getData() << std::endl;
    
    std::cout << "\n对比结果：" << std::endl;
    std::cout << "1. obj1 的 this 地址不变" << std::endl;
    std::cout << "2. obj1 的数据地址变为 nullptr" << std::endl;
    std::cout << "3. obj2 的 this 地址是新的" << std::endl;
    std::cout << "4. obj2 的数据地址是 obj1 的原始地址" << std::endl;
}

// 演示4：this 指针在数组中的表现
void demo_this_pointer_in_array() {
    std::cout << "\n=== 演示4：this 指针在数组中的表现 ===" << std::endl;
    
    ThisPointerDemo::resetCount();
    
    // 创建对象数组
    ThisPointerDemo objects[3] = {ThisPointerDemo(1), ThisPointerDemo(2), ThisPointerDemo(3)};
    
    std::cout << "\n数组中的对象地址：" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "objects[" << i << "] 地址: " << &objects[i] << std::endl;
        objects[i].printThisInfo();
    }
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. 数组中的每个对象都有固定的 this 地址" << std::endl;
    std::cout << "2. this 地址等于对象在数组中的位置" << std::endl;
    std::cout << "3. this 地址在对象生命周期中不变" << std::endl;
}

// 演示5：this 指针的理论解释
void demo_this_pointer_theory() {
    std::cout << "\n=== 演示5：this 指针的理论解释 ===" << std::endl;
    
    std::cout << "this 指针不变的理论原因：" << std::endl;
    std::cout << "1. 对象内存分配：对象在内存中的位置是固定的" << std::endl;
    std::cout << "2. 移动语义：只转移资源所有权，不移动对象本身" << std::endl;
    std::cout << "3. 内存布局：对象的内存布局在构造时确定" << std::endl;
    std::cout << "4. 生命周期：this 指针在对象生命周期中不变" << std::endl;
    
    std::cout << "\n移动语义的本质：" << std::endl;
    std::cout << "1. 不是移动对象本身" << std::endl;
    std::cout << "2. 而是转移对象持有的资源" << std::endl;
    std::cout << "3. 原对象仍然存在，但资源被转移" << std::endl;
    std::cout << "4. 新对象获得资源，但地址不同" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "ThisPointerDemo obj1(1);" << std::endl;
    std::cout << "ThisPointerDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "// obj1 的 this 地址不变" << std::endl;
    std::cout << "// obj2 的 this 地址是新的" << std::endl;
    std::cout << "// 只是资源从 obj1 转移到 obj2" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ThisPointerDemo::resetCount();
    
    demo_this_pointer_stability();
    demo_this_pointer_relationship();
    demo_this_pointer_comparison();
    demo_this_pointer_in_array();
    demo_this_pointer_theory();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "this 地址不变的原因：" << std::endl;
    std::cout << "1. 对象内存位置固定：对象在内存中的位置是固定的" << std::endl;
    std::cout << "2. 移动语义本质：只转移资源，不移动对象本身" << std::endl;
    std::cout << "3. 内存布局稳定：对象的内存布局在构造时确定" << std::endl;
    std::cout << "4. 生命周期一致：this 指针在对象生命周期中不变" << std::endl;
    std::cout << "5. 地址唯一性：每个对象都有唯一的 this 地址" << std::endl;
    
    return 0;
}

















