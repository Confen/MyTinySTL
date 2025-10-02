#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 演示类：展示内存分配与对象初始化的区别
class MemoryVsObjectDemo {
private:
    int* data_;
    std::string name_;
    static int count_;
    
public:
    // 构造函数：对象初始化
    MemoryVsObjectDemo(int size, const std::string& n) : name_(n) {
        ++count_;
        data_ = new int[size];  // 这是对象初始化，不仅仅是内存分配！
        
        // 初始化数组内容
        for (int i = 0; i < size; ++i) {
            data_[i] = i;
        }
        
        std::cout << "对象初始化完成：" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  数组内容: ";
        for (int i = 0; i < size; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "  对象名称: " << name_ << std::endl;
        std::cout << "  对象总数: " << count_ << std::endl;
    }
    
    // 析构函数：对象析构
    ~MemoryVsObjectDemo() {
        --count_;
        std::cout << "对象析构：" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象名称: " << name_ << std::endl;
        std::cout << "  剩余对象: " << count_ << std::endl;
        
        if (data_) {
            std::cout << "  释放内存: " << data_ << std::endl;
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    void printState() const {
        std::cout << "对象状态: 地址=" << this << ", 名称=" << name_ 
                  << ", 数据=" << data_ << std::endl;
    }
};

int MemoryVsObjectDemo::count_ = 0;

// 演示1：内存分配 vs 对象初始化
void demo_memory_allocation_vs_object_initialization() {
    std::cout << "=== 演示1：内存分配 vs 对象初始化 ===" << std::endl;
    
    std::cout << "关键区别：" << std::endl;
    std::cout << "1. 内存分配：只是分配内存空间" << std::endl;
    std::cout << "2. 对象初始化：设置对象状态，建立对象结构" << std::endl;
    
    std::cout << "\n内存分配示例：" << std::endl;
    void* rawMemory = malloc(sizeof(MemoryVsObjectDemo));
    std::cout << "  分配内存: " << rawMemory << std::endl;
    std::cout << "  内存状态: 未初始化，不能使用" << std::endl;
    
    std::cout << "\n对象初始化示例：" << std::endl;
    MemoryVsObjectDemo* obj = new(rawMemory) MemoryVsObjectDemo(5, "test");
    std::cout << "  对象状态: 已初始化，可以使用" << std::endl;
    
    // 清理
    obj->~MemoryVsObjectDemo();
    free(rawMemory);
}

// 演示2：data_ = new int[10] 的完整过程
void demo_data_assignment_process() {
    std::cout << "\n=== 演示2：data_ = new int[10] 的完整过程 ===" << std::endl;
    
    std::cout << "data_ = new int[10] 的完整过程：" << std::endl;
    std::cout << "1. new int[10] 执行以下操作：" << std::endl;
    std::cout << "   - 分配内存空间（10个int的大小）" << std::endl;
    std::cout << "   - 调用int的默认构造函数（对于基本类型，这是无操作）" << std::endl;
    std::cout << "   - 返回指向分配内存的指针" << std::endl;
    
    std::cout << "\n2. data_ = ... 执行以下操作：" << std::endl;
    std::cout << "   - 将指针赋值给成员变量data_" << std::endl;
    std::cout << "   - 建立对象与内存的关联" << std::endl;
    std::cout << "   - 设置对象状态" << std::endl;
    
    std::cout << "\n3. 这不是简单的内存分配，而是：" << std::endl;
    std::cout << "   - 对象状态建立" << std::endl;
    std::cout << "   - 资源获取" << std::endl;
    std::cout << "   - 对象初始化" << std::endl;
}

// 演示3：对象初始化的组成部分
void demo_object_initialization_components() {
    std::cout << "\n=== 演示3：对象初始化的组成部分 ===" << std::endl;
    
    std::cout << "对象初始化的组成部分：" << std::endl;
    std::cout << "1. 内存分配：" << std::endl;
    std::cout << "   - 为对象分配内存空间" << std::endl;
    std::cout << "   - 为成员变量分配内存空间" << std::endl;
    
    std::cout << "\n2. 状态设置：" << std::endl;
    std::cout << "   - 设置成员变量的值" << std::endl;
    std::cout << "   - 建立对象内部状态" << std::endl;
    
    std::cout << "\n3. 资源获取：" << std::endl;
    std::cout << "   - 获取外部资源（如动态内存）" << std::endl;
    std::cout << "   - 建立资源与对象的关联" << std::endl;
    
    std::cout << "\n4. 对象结构建立：" << std::endl;
    std::cout << "   - 建立对象的内存布局" << std::endl;
    std::cout << "   - 设置对象的基本属性" << std::endl;
    
    std::cout << "\n5. 可用性确保：" << std::endl;
    std::cout << "   - 确保对象可以安全使用" << std::endl;
    std::cout << "   - 确保对象状态有效" << std::endl;
}

// 演示4：实际演示对象初始化
void demo_actual_object_initialization() {
    std::cout << "\n=== 演示4：实际演示对象初始化 ===" << std::endl;
    
    MemoryVsObjectDemo::resetCount();
    
    std::cout << "创建对象前的状态：" << std::endl;
    std::cout << "  对象总数: " << MemoryVsObjectDemo::getCount() << std::endl;
    
    std::cout << "\n执行 data_ = new int[5] 的过程：" << std::endl;
    MemoryVsObjectDemo obj(5, "demo");
    
    std::cout << "\n对象创建后的状态：" << std::endl;
    obj.printState();
    std::cout << "  对象总数: " << MemoryVsObjectDemo::getCount() << std::endl;
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. data_ = new int[5] 不仅仅是内存分配" << std::endl;
    std::cout << "2. 它建立了对象与内存的关联" << std::endl;
    std::cout << "3. 它设置了对象的状态" << std::endl;
    std::cout << "4. 它使对象变得可用" << std::endl;
}

// 演示5：与纯内存分配的区别
void demo_difference_from_pure_memory_allocation() {
    std::cout << "\n=== 演示5：与纯内存分配的区别 ===" << std::endl;
    
    std::cout << "纯内存分配：" << std::endl;
    void* rawMemory = malloc(10 * sizeof(int));
    std::cout << "  内存地址: " << rawMemory << std::endl;
    std::cout << "  内存状态: 未初始化，不能使用" << std::endl;
    std::cout << "  对象状态: 不存在" << std::endl;
    
    std::cout << "\ndata_ = new int[10]：" << std::endl;
    int* data = new int[10];
    std::cout << "  内存地址: " << data << std::endl;
    std::cout << "  内存状态: 已初始化，可以使用" << std::endl;
    std::cout << "  对象状态: 已建立关联" << std::endl;
    
    std::cout << "\n关键区别：" << std::endl;
    std::cout << "1. 纯内存分配：只是分配空间" << std::endl;
    std::cout << "2. data_ = new int[10]：分配空间 + 建立关联 + 设置状态" << std::endl;
    std::cout << "3. 纯内存分配：对象不存在" << std::endl;
    std::cout << "4. data_ = new int[10]：对象状态已建立" << std::endl;
    
    // 清理
    delete[] data;
    free(rawMemory);
}

// 演示6：RAII中的初始化概念
void demo_initialization_concept_in_raii() {
    std::cout << "\n=== 演示6：RAII中的初始化概念 ===" << std::endl;
    
    std::cout << "RAII中的初始化概念：" << std::endl;
    std::cout << "1. 资源获取即初始化：" << std::endl;
    std::cout << "   - 不仅仅是获取资源" << std::endl;
    std::cout << "   - 还包括建立对象状态" << std::endl;
    std::cout << "   - 确保对象可用" << std::endl;
    
    std::cout << "\n2. data_ = new int[10] 在RAII中的作用：" << std::endl;
    std::cout << "   - 获取资源（动态内存）" << std::endl;
    std::cout << "   - 建立对象状态" << std::endl;
    std::cout << "   - 确保对象可用" << std::endl;
    std::cout << "   - 为后续使用做准备" << std::endl;
    
    std::cout << "\n3. 这不是简单的内存分配，而是：" << std::endl;
    std::cout << "   - 对象生命周期的开始" << std::endl;
    std::cout << "   - 对象状态的建立" << std::endl;
    std::cout << "   - 对象可用性的确保" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    MemoryVsObjectDemo::resetCount();
    
    demo_memory_allocation_vs_object_initialization();
    demo_data_assignment_process();
    demo_object_initialization_components();
    demo_actual_object_initialization();
    demo_difference_from_pure_memory_allocation();
    demo_initialization_concept_in_raii();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "data_ = new int[10] 不只是初始化了内存：" << std::endl;
    std::cout << "1. 它确实分配了内存" << std::endl;
    std::cout << "2. 但它还做了更多事情：" << std::endl;
    std::cout << "   - 建立了对象与内存的关联" << std::endl;
    std::cout << "   - 设置了对象的状态" << std::endl;
    std::cout << "   - 使对象变得可用" << std::endl;
    std::cout << "   - 建立了对象的生命周期" << std::endl;
    std::cout << "3. 这是完整的对象初始化过程" << std::endl;
    std::cout << "4. 不仅仅是内存分配，而是对象状态建立" << std::endl;
    
    return 0;
}






