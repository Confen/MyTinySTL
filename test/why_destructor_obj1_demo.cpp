#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：详细跟踪内存状态
class MemoryStateDemo {
private:
    int value_;
    int* data_;
    static int count_;
    
public:
    MemoryStateDemo(int v) : value_(v) {
        ++count_;
        data_ = new int[5];
        std::cout << "构造 MemoryStateDemo(" << value_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    MemoryStateDemo(MemoryStateDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_) {
        ++count_;
        std::cout << "移动构造 MemoryStateDemo(" << value_ << ")" << std::endl;
        std::cout << "  新对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
        
        // 转移资源所有权
        other.data_ = nullptr;
        other.value_ = -1;
        
        std::cout << "  原对象地址: " << &other << std::endl;
        std::cout << "  原对象数据地址: " << other.data_ << std::endl;
        std::cout << "  原对象大小: " << sizeof(other) << " 字节" << std::endl;
    }
    
    ~MemoryStateDemo() {
        --count_;
        std::cout << "析构 MemoryStateDemo(" << value_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        
        if (data_) {
            delete[] data_;
            std::cout << "  释放数据内存: " << data_ << std::endl;
        } else {
            std::cout << "  数据指针为nullptr，无需释放数据" << std::endl;
        }
        
        std::cout << "  对象本身的内存将被释放" << std::endl;
    }
    
    int getValue() const { return value_; }
    int* getData() const { return data_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    // 打印内存状态
    void printMemoryState() const {
        std::cout << "内存状态: 对象地址=" << this << ", 数据地址=" << data_ 
                  << ", 对象大小=" << sizeof(*this) << " 字节" << std::endl;
    }
};

int MemoryStateDemo::count_ = 0;

// 演示1：移动后原对象的内存状态
void demo_moved_object_memory_state() {
    std::cout << "=== 演示1：移动后原对象的内存状态 ===" << std::endl;
    
    MemoryStateDemo::resetCount();
    
    // 创建对象
    MemoryStateDemo obj1(1);
    std::cout << "\n移动前 obj1：" << std::endl;
    obj1.printMemoryState();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    MemoryStateDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1：" << std::endl;
    obj1.printMemoryState();
    
    std::cout << "\n移动后 obj2：" << std::endl;
    obj2.printMemoryState();
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. obj1 的对象地址不变" << std::endl;
    std::cout << "2. obj1 的对象大小不变" << std::endl;
    std::cout << "3. obj1 仍然占用对象本身的内存" << std::endl;
    std::cout << "4. obj1 必须被析构才能释放对象内存" << std::endl;
}

// 演示2：不析构原对象的后果
void demo_consequences_no_destructor() {
    std::cout << "\n=== 演示2：不析构原对象的后果 ===" << std::endl;
    
    MemoryStateDemo::resetCount();
    
    {
        MemoryStateDemo obj1(2);
        MemoryStateDemo obj2 = std::move(obj1);
        
        std::cout << "\n移动后状态：" << std::endl;
        std::cout << "obj1 对象地址: " << &obj1 << std::endl;
        std::cout << "obj1 对象大小: " << sizeof(obj1) << " 字节" << std::endl;
        std::cout << "obj1 仍然占用内存！" << std::endl;
        
        std::cout << "\n如果不析构 obj1：" << std::endl;
        std::cout << "1. obj1 的对象内存不会被释放" << std::endl;
        std::cout << "2. 导致内存泄漏" << std::endl;
        std::cout << "3. 对象计数不正确" << std::endl;
        std::cout << "4. 违反RAII原则" << std::endl;
    }
    
    std::cout << "\n作用域结束，obj1 和 obj2 都被析构" << std::endl;
}

// 演示3：显式析构原对象
void demo_explicit_destructor() {
    std::cout << "\n=== 演示3：显式析构原对象 ===" << std::endl;
    
    MemoryStateDemo::resetCount();
    
    MemoryStateDemo obj1(3);
    MemoryStateDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printMemoryState();
    
    std::cout << "\n显式析构 obj1：" << std::endl;
    obj1.~MemoryStateDemo();
    
    std::cout << "\n析构后 obj1 状态：" << std::endl;
    obj1.printMemoryState();
    
    std::cout << "\nobj2 仍然有效：" << std::endl;
    obj2.printMemoryState();
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. obj1 被显式析构" << std::endl;
    std::cout << "2. obj1 的对象内存被释放" << std::endl;
    std::cout << "3. obj2 仍然持有原始资源" << std::endl;
    std::cout << "4. 内存管理正确" << std::endl;
}

// 演示4：内存布局分析
void demo_memory_layout_analysis() {
    std::cout << "\n=== 演示4：内存布局分析 ===" << std::endl;
    
    MemoryStateDemo::resetCount();
    
    MemoryStateDemo obj1(4);
    
    std::cout << "\n移动前内存布局：" << std::endl;
    std::cout << "对象内存: " << &obj1 << " (大小: " << sizeof(obj1) << " 字节)" << std::endl;
    std::cout << "数据内存: " << obj1.getData() << " (大小: " << 5 * sizeof(int) << " 字节)" << std::endl;
    
    MemoryStateDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后内存布局：" << std::endl;
    std::cout << "obj1 对象内存: " << &obj1 << " (大小: " << sizeof(obj1) << " 字节)" << std::endl;
    std::cout << "obj1 数据内存: " << obj1.getData() << " (已转移)" << std::endl;
    std::cout << "obj2 对象内存: " << &obj2 << " (大小: " << sizeof(obj2) << " 字节)" << std::endl;
    std::cout << "obj2 数据内存: " << obj2.getData() << " (获得原数据)" << std::endl;
    
    std::cout << "\n分析：" << std::endl;
    std::cout << "1. obj1 的对象内存仍然被占用" << std::endl;
    std::cout << "2. obj1 的数据内存已转移给 obj2" << std::endl;
    std::cout << "3. obj1 必须被析构才能释放对象内存" << std::endl;
    std::cout << "4. obj2 获得数据内存，但对象内存是新的" << std::endl;
}

// 演示5：内存管理的重要性
void demo_memory_management_importance() {
    std::cout << "\n=== 演示5：内存管理的重要性 ===" << std::endl;
    
    std::cout << "内存管理的重要性：" << std::endl;
    std::cout << "1. 对象内存：每个对象都占用内存" << std::endl;
    std::cout << "2. 数据内存：对象可能持有动态分配的内存" << std::endl;
    std::cout << "3. 移动语义：只转移数据内存，不转移对象内存" << std::endl;
    std::cout << "4. 析构函数：必须释放所有内存" << std::endl;
    
    std::cout << "\n移动语义的内存管理：" << std::endl;
    std::cout << "1. 原对象：对象内存仍然存在，数据内存被转移" << std::endl;
    std::cout << "2. 新对象：对象内存是新的，数据内存是转移的" << std::endl;
    std::cout << "3. 析构：原对象必须被析构释放对象内存" << std::endl;
    std::cout << "4. 安全：新对象析构时释放数据内存" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "MemoryStateDemo obj1(1);" << std::endl;
    std::cout << "MemoryStateDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "// obj1 仍然占用对象内存" << std::endl;
    std::cout << "// obj1 必须被析构" << std::endl;
    std::cout << "obj1.~MemoryStateDemo();" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    MemoryStateDemo::resetCount();
    
    demo_moved_object_memory_state();
    demo_consequences_no_destructor();
    demo_explicit_destructor();
    demo_memory_layout_analysis();
    demo_memory_management_importance();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么还需要对 obj1 析构释放其对象内存：" << std::endl;
    std::cout << "1. 对象内存仍然被占用：obj1 仍然占用对象本身的内存" << std::endl;
    std::cout << "2. 移动语义只转移资源：不转移对象本身的内存" << std::endl;
    std::cout << "3. 内存管理要求：每个对象都必须被正确析构" << std::endl;
    std::cout << "4. RAII原则：资源获取即初始化，析构时必须清理" << std::endl;
    std::cout << "5. 异常安全：确保内存被正确释放" << std::endl;
    std::cout << "6. 对象计数：维护正确的对象计数" << std::endl;
    
    return 0;
}








