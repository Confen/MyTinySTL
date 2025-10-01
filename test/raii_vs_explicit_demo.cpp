#include <iostream>
#include <memory>
#include <windows.h>

// 演示类：跟踪析构过程
class RAIIvsExplicitDemo {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    RAIIvsExplicitDemo(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        for (int i = 0; i < 3; ++i) {
            data_[i] = v + i;
        }
        std::cout << "构造 RAIIvsExplicitDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    RAIIvsExplicitDemo(RAIIvsExplicitDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 RAIIvsExplicitDemo(" << value_ << ", " << name_ << ")" << std::endl;
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
    
    ~RAIIvsExplicitDemo() {
        --count_;
        std::cout << "析构 RAIIvsExplicitDemo(" << value_ << ", " << name_ << ")" << std::endl;
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

int RAIIvsExplicitDemo::count_ = 0;

// 演示1：RAII 隐式析构
void demo_raii_implicit_destructor() {
    std::cout << "=== 演示1：RAII 隐式析构 ===" << std::endl;
    
    RAIIvsExplicitDemo::resetCount();
    
    std::cout << "\nRAII 隐式析构的特点：" << std::endl;
    std::cout << "1. 编译器自动管理对象生命周期" << std::endl;
    std::cout << "2. 在作用域结束时自动调用析构函数" << std::endl;
    std::cout << "3. 不需要手动调用析构函数" << std::endl;
    std::cout << "4. 异常安全，即使发生异常也会自动析构" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj1(1, \"raii_source\");" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // obj1 在作用域结束时自动析构" << std::endl;
    std::cout << "    // 不需要手动调用 obj1.~RAIIvsExplicitDemo()" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n实际执行：" << std::endl;
    {
        RAIIvsExplicitDemo obj1(1, "raii_source");
        RAIIvsExplicitDemo obj2 = std::move(obj1);
        
        std::cout << "\n作用域内状态：" << std::endl;
        std::cout << "obj1 地址: " << &obj1 << std::endl;
        std::cout << "obj2 地址: " << &obj2 << std::endl;
        std::cout << "对象计数: " << RAIIvsExplicitDemo::getCount() << std::endl;
        
        std::cout << "\n作用域结束，自动析构：" << std::endl;//// 编译器自动管理对象生命周期
        // 在作用域结束时自动调用析构函数
        // 不需要手动调用
    }
    
    std::cout << "\n作用域外状态：" << std::endl;
    std::cout << "对象计数: " << RAIIvsExplicitDemo::getCount() << std::endl;
}

// 演示2：显式析构
void demo_explicit_destructor() {
    std::cout << "\n=== 演示2：显式析构 ===" << std::endl;
    
    RAIIvsExplicitDemo::resetCount();
    
    std::cout << "\n显式析构的特点：" << std::endl;
    std::cout << "1. 手动调用析构函数" << std::endl;
    std::cout << "2. 需要手动管理对象生命周期" << std::endl;
    std::cout << "3. 调用后对象处于未定义状态" << std::endl;
    std::cout << "4. 必须小心使用，容易出错" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "RAIIvsExplicitDemo obj1(1, \"explicit_source\");" << std::endl;
    std::cout << "RAIIvsExplicitDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~RAIIvsExplicitDemo(); // 显式调用析构函数" << std::endl;
    
    std::cout << "\n实际执行：" << std::endl;
    RAIIvsExplicitDemo obj1(1, "explicit_source");
    RAIIvsExplicitDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后状态：" << std::endl;
    std::cout << "obj1 地址: " << &obj1 << std::endl;
    std::cout << "obj2 地址: " << &obj2 << std::endl;
    std::cout << "对象计数: " << RAIIvsExplicitDemo::getCount() << std::endl;
    
    std::cout << "\n显式调用 obj1 的析构函数：" << std::endl;
    obj1.~RAIIvsExplicitDemo();
    
    std::cout << "\n显式析构后状态：" << std::endl;
    std::cout << "obj1 地址: " << &obj1 << std::endl;
    std::cout << "obj2 地址: " << &obj2 << std::endl;
    std::cout << "对象计数: " << RAIIvsExplicitDemo::getCount() << std::endl;
}

// 演示3：RAII vs 显式析构的对比
void demo_raii_vs_explicit_comparison() {
    std::cout << "\n=== 演示3：RAII vs 显式析构的对比 ===" << std::endl;
    
    std::cout << "RAII 隐式析构：" << std::endl;
    std::cout << "1. 编译器自动管理" << std::endl;
    std::cout << "2. 在作用域结束时自动调用" << std::endl;
    std::cout << "3. 异常安全" << std::endl;
    std::cout << "4. 代码简洁" << std::endl;
    std::cout << "5. 推荐使用" << std::endl;
    
    std::cout << "\n显式析构：" << std::endl;
    std::cout << "1. 手动调用" << std::endl;
    std::cout << "2. 需要手动管理" << std::endl;
    std::cout << "3. 容易出错" << std::endl;
    std::cout << "4. 代码复杂" << std::endl;
    std::cout << "5. 特殊情况使用" << std::endl;
    
    std::cout << "\n代码对比：" << std::endl;
    std::cout << "// RAII 隐式析构" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj1(1, \"raii\");" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // 自动析构，无需手动调用" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n// 显式析构" << std::endl;
    std::cout << "RAIIvsExplicitDemo obj1(1, \"explicit\");" << std::endl;
    std::cout << "RAIIvsExplicitDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~RAIIvsExplicitDemo(); // 手动调用" << std::endl;
}

// 演示4：RAII 的优势
void demo_raii_advantages() {
    std::cout << "\n=== 演示4：RAII 的优势 ===" << std::endl;
    
    std::cout << "RAII 的优势：" << std::endl;
    std::cout << "1. 自动管理：编译器自动管理对象生命周期" << std::endl;
    std::cout << "2. 异常安全：即使发生异常也会自动析构" << std::endl;
    std::cout << "3. 代码简洁：不需要手动调用析构函数" << std::endl;
    std::cout << "4. 维护性好：代码易于理解和维护" << std::endl;
    std::cout << "5. 错误少：减少手动管理导致的错误" << std::endl;
    
    std::cout << "\n异常安全示例：" << std::endl;
    std::cout << "try {" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj1(1, \"raii\");" << std::endl;
    std::cout << "    RAIIvsExplicitDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // 即使发生异常，obj1 和 obj2 也会自动析构" << std::endl;
    std::cout << "    throw std::runtime_error(\"异常\");" << std::endl;
    std::cout << "} catch (...) {" << std::endl;
    std::cout << "    // obj1 和 obj2 已经自动析构" << std::endl;
    std::cout << "}" << std::endl;
}

// 演示5：何时使用显式析构
void demo_when_to_use_explicit_destructor() {
    std::cout << "\n=== 演示5：何时使用显式析构 ===" << std::endl;
    
    std::cout << "何时使用显式析构：" << std::endl;
    std::cout << "1. 异常安全：在异常处理中清理资源" << std::endl;
    std::cout << "2. 内存管理：在自定义内存管理中" << std::endl;
    std::cout << "3. 容器实现：在容器中管理对象生命周期" << std::endl;
    std::cout << "4. 高级编程：在模板元编程中" << std::endl;
    std::cout << "5. 特殊情况：当 RAII 不适用时" << std::endl;
    
    std::cout << "\n何时使用 RAII：" << std::endl;
    std::cout << "1. 一般情况：大多数情况下使用 RAII" << std::endl;
    std::cout << "2. 异常安全：需要异常安全时" << std::endl;
    std::cout << "3. 代码简洁：希望代码简洁时" << std::endl;
    std::cout << "4. 维护性：希望代码易于维护时" << std::endl;
    std::cout << "5. 推荐做法：现代 C++ 推荐做法" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    RAIIvsExplicitDemo::resetCount();
    
    demo_raii_implicit_destructor();
    demo_explicit_destructor();
    demo_raii_vs_explicit_comparison();
    demo_raii_advantages();
    demo_when_to_use_explicit_destructor();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "使用 RAII 属于显式析构吗？" << std::endl;
    std::cout << "答案：不是！" << std::endl;
    std::cout << "1. RAII 是隐式析构：编译器自动管理" << std::endl;
    std::cout << "2. 显式析构是手动调用：object.~ClassName()" << std::endl;
    std::cout << "3. RAII 是推荐做法：现代 C++ 最佳实践" << std::endl;
    std::cout << "4. 显式析构是特殊情况：只在必要时使用" << std::endl;
    std::cout << "5. 两者完全不同：管理方式完全不同" << std::endl;
    
    return 0;
}






