#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 演示类：展示内存分配与对象初始化的区别
class MemoryVsInitialization {
private:
    int value_;
    std::string name_;
    static int count_;
    
public:
    // 默认构造函数
    MemoryVsInitialization() : value_(0), name_("default") {
        ++count_;
        std::cout << "默认构造: value_=" << value_ << ", name_=" << name_ << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 带参数构造函数
    MemoryVsInitialization(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        std::cout << "带参构造: value_=" << value_ << ", name_=" << name_ << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 析构函数
    ~MemoryVsInitialization() {
        --count_;
        std::cout << "析构: value_=" << value_ << ", name_=" << name_ << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    const std::string& getName() const { return name_; }
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    void printState() const {
        std::cout << "对象状态: 地址=" << this << ", 值=" << value_ 
                  << ", 名称=" << name_ << std::endl;
    }
};

int MemoryVsInitialization::count_ = 0;

// 演示1：内存分配 vs 对象初始化
void demo_memory_allocation_vs_initialization() {
    std::cout << "=== 演示1：内存分配 vs 对象初始化 ===" << std::endl;
    
    MemoryVsInitialization::resetCount();
    
    std::cout << "1. 栈内存分配（自动初始化）：" << std::endl;
    MemoryVsInitialization stackObj(42, "stack");
    stackObj.printState();
    
    std::cout << "\n2. 堆内存分配（需要显式初始化）：" << std::endl;
    MemoryVsInitialization* heapObj = new MemoryVsInitialization(100, "heap");
    heapObj->printState();
    
    std::cout << "\n3. 未初始化的内存（危险）：" << std::endl;
    void* rawMemory = malloc(sizeof(MemoryVsInitialization));
    std::cout << "  分配了内存: " << rawMemory << std::endl;
    std::cout << "  但对象未初始化！" << std::endl;
    
    // 使用placement new初始化
    std::cout << "\n4. 使用placement new初始化：" << std::endl;
    MemoryVsInitialization* initializedObj = new(rawMemory) MemoryVsInitialization(200, "placement");
    initializedObj->printState();
    
    // 清理
    initializedObj->~MemoryVsInitialization();
    free(rawMemory);
    delete heapObj;
}

// 演示2：不同初始化方式的区别
void demo_different_initialization_methods() {
    std::cout << "\n=== 演示2：不同初始化方式的区别 ===" << std::endl;
    
    std::cout << "初始化方式对比：" << std::endl;
    std::cout << "1. 栈对象（自动初始化）：" << std::endl;
    std::cout << "   MemoryVsInitialization obj(1, \"stack\");" << std::endl;
    std::cout << "   - 内存自动分配" << std::endl;
    std::cout << "   - 对象自动初始化" << std::endl;
    std::cout << "   - 自动调用构造函数" << std::endl;
    
    std::cout << "\n2. 堆对象（显式初始化）：" << std::endl;
    std::cout << "   MemoryVsInitialization* obj = new MemoryVsInitialization(2, \"heap\");" << std::endl;
    std::cout << "   - 内存显式分配" << std::endl;
    std::cout << "   - 对象显式初始化" << std::endl;
    std::cout << "   - 显式调用构造函数" << std::endl;
    
    std::cout << "\n3. 未初始化内存（危险）：" << std::endl;
    std::cout << "   void* ptr = malloc(sizeof(MemoryVsInitialization));" << std::endl;
    std::cout << "   - 内存已分配" << std::endl;
    std::cout << "   - 对象未初始化" << std::endl;
    std::cout << "   - 不能使用！" << std::endl;
    
    std::cout << "\n4. placement new（手动初始化）：" << std::endl;
    std::cout << "   new(ptr) MemoryVsInitialization(3, \"placement\");" << std::endl;
    std::cout << "   - 内存已分配" << std::endl;
    std::cout << "   - 对象手动初始化" << std::endl;
    std::cout << "   - 手动调用构造函数" << std::endl;
}

// 演示3：内存状态的不同阶段
void demo_memory_state_stages() {
    std::cout << "\n=== 演示3：内存状态的不同阶段 ===" << std::endl;
    
    std::cout << "内存状态阶段：" << std::endl;
    std::cout << "1. 未分配：内存不存在" << std::endl;
    std::cout << "2. 已分配但未初始化：内存存在，但对象未初始化" << std::endl;
    std::cout << "3. 已初始化：内存存在，对象已初始化" << std::endl;
    std::cout << "4. 已析构但未释放：对象已析构，但内存未释放" << std::endl;
    std::cout << "5. 已释放：内存不存在" << std::endl;
    
    std::cout << "\n实际演示：" << std::endl;
    
    // 阶段1：未分配
    std::cout << "阶段1：未分配" << std::endl;
    
    // 阶段2：已分配但未初始化
    std::cout << "\n阶段2：已分配但未初始化" << std::endl;
    void* rawMemory = malloc(sizeof(MemoryVsInitialization));
    std::cout << "  内存地址: " << rawMemory << std::endl;
    std::cout << "  对象状态: 未初始化，不能使用！" << std::endl;
    
    // 阶段3：已初始化
    std::cout << "\n阶段3：已初始化" << std::endl;
    MemoryVsInitialization* obj = new(rawMemory) MemoryVsInitialization(300, "staged");
    obj->printState();
    
    // 阶段4：已析构但未释放
    std::cout << "\n阶段4：已析构但未释放" << std::endl;
    obj->~MemoryVsInitialization();
    std::cout << "  内存地址: " << rawMemory << std::endl;
    std::cout << "  对象状态: 已析构，不能使用！" << std::endl;
    
    // 阶段5：已释放
    std::cout << "\n阶段5：已释放" << std::endl;
    free(rawMemory);
    std::cout << "  内存已释放" << std::endl;
}

// 演示4：为什么初始化很重要
void demo_why_initialization_matters() {
    std::cout << "\n=== 演示4：为什么初始化很重要 ===" << std::endl;
    
    std::cout << "为什么初始化很重要：" << std::endl;
    std::cout << "1. 对象状态：确保对象处于有效状态" << std::endl;
    std::cout << "2. 成员初始化：确保成员变量有正确的值" << std::endl;
    std::cout << "3. 资源管理：确保资源正确获取" << std::endl;
    std::cout << "4. 异常安全：确保异常时对象状态正确" << std::endl;
    std::cout << "5. 使用安全：确保可以安全使用对象" << std::endl;
    
    std::cout << "\n未初始化的危险：" << std::endl;
    std::cout << "1. 未定义行为：使用未初始化对象导致未定义行为" << std::endl;
    std::cout << "2. 程序崩溃：访问未初始化内存可能导致崩溃" << std::endl;
    std::cout << "3. 数据损坏：未初始化数据可能损坏程序状态" << std::endl;
    std::cout << "4. 安全漏洞：未初始化内存可能被恶意利用" << std::endl;
    
    std::cout << "\n正确做法：" << std::endl;
    std::cout << "1. 总是初始化对象" << std::endl;
    std::cout << "2. 使用构造函数初始化" << std::endl;
    std::cout << "3. 使用智能指针" << std::endl;
    std::cout << "4. 避免未初始化内存" << std::endl;
}

// 演示5：初始化与RAII的关系
void demo_initialization_and_raii() {
    std::cout << "\n=== 演示5：初始化与RAII的关系 ===" << std::endl;
    
    std::cout << "初始化与RAII的关系：" << std::endl;
    std::cout << "1. RAII要求：资源获取即初始化" << std::endl;
    std::cout << "2. 初始化时机：在构造函数中初始化" << std::endl;
    std::cout << "3. 资源管理：初始化时获取资源" << std::endl;
    std::cout << "4. 异常安全：初始化失败时清理资源" << std::endl;
    
    std::cout << "\nRAII示例：" << std::endl;
    std::cout << "class RAIIExample {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    RAIIExample() {" << std::endl;
    std::cout << "        data_ = new int[10];  // 资源获取即初始化" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    ~RAIIExample() {" << std::endl;
    std::cout << "        delete[] data_;       // 资源释放即析构" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. 初始化不仅仅是内存分配" << std::endl;
    std::cout << "2. 初始化包括对象状态设置" << std::endl;
    std::cout << "3. 初始化包括资源获取" << std::endl;
    std::cout << "4. 初始化确保对象可用" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    MemoryVsInitialization::resetCount();
    
    demo_memory_allocation_vs_initialization();
    demo_different_initialization_methods();
    demo_memory_state_stages();
    demo_why_initialization_matters();
    demo_initialization_and_raii();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "初始化内存 vs 初始化：" << std::endl;
    std::cout << "1. 内存分配 ≠ 对象初始化" << std::endl;
    std::cout << "2. 内存分配：只是分配内存空间" << std::endl;
    std::cout << "3. 对象初始化：设置对象状态，调用构造函数" << std::endl;
    std::cout << "4. 初始化包括：成员变量设置、资源获取、状态建立" << std::endl;
    std::cout << "5. 未初始化的内存：危险，不能使用" << std::endl;
    std::cout << "6. RAII原则：资源获取即初始化" << std::endl;
    
    return 0;
}





