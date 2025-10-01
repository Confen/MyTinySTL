#include <iostream>
#include <memory>
#include <windows.h>

// 演示类：详细跟踪析构过程
class ExplicitDestructorDemo {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    ExplicitDestructorDemo(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        for (int i = 0; i < 3; ++i) {
            data_[i] = v + i;
        }
        std::cout << "构造 ExplicitDestructorDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    ExplicitDestructorDemo(ExplicitDestructorDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 ExplicitDestructorDemo(" << value_ << ", " << name_ << ")" << std::endl;
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
    
    ~ExplicitDestructorDemo() {
        --count_;
        std::cout << "析构 ExplicitDestructorDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        
        if (data_) {
            std::cout << "  释放数据内存: " << data_ << std::endl;
            delete[] data_;
        } else {
            std::cout << "  数据指针为nullptr，无需释放数据" << std::endl;
        }
        
        std::cout << "  析构函数完成，对象地址 " << this << " 仍然存在" << std::endl;
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

int ExplicitDestructorDemo::count_ = 0;

// 演示1：基本的显式析构调用
void demo_basic_explicit_destructor() {
    std::cout << "=== 演示1：基本的显式析构调用 ===" << std::endl;
    
    ExplicitDestructorDemo::resetCount();
    
    // 创建对象
    ExplicitDestructorDemo obj1(1, "original");
    std::cout << "\n移动前 obj1：" << std::endl;
    obj1.printState();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    ExplicitDestructorDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1：" << std::endl;
    obj1.printState();
    
    std::cout << "\n移动后 obj2：" << std::endl;
    obj2.printState();
    
    std::cout << "\n主动调用 obj1 的析构函数：" << std::endl;
    obj1.~ExplicitDestructorDemo();
    
    std::cout << "\n显式析构后 obj1：" << std::endl;
    obj1.printState();
    
    std::cout << "\n显式析构后 obj2：" << std::endl;
    obj2.printState();
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. obj1.~ExplicitDestructorDemo() 成功调用" << std::endl;
    std::cout << "2. obj1 的成员被清理" << std::endl;
    std::cout << "3. obj1 的对象地址仍然存在" << std::endl;
    std::cout << "4. obj2 仍然有效" << std::endl;
}

// 演示2：显式析构的语法和规则
void demo_explicit_destructor_syntax() {
    std::cout << "\n=== 演示2：显式析构的语法和规则 ===" << std::endl;
    
    ExplicitDestructorDemo::resetCount();
    
    ExplicitDestructorDemo obj1(2, "source");
    ExplicitDestructorDemo obj2 = std::move(obj1);
    
    std::cout << "\n显式析构的语法：" << std::endl;
    std::cout << "1. 基本语法: object.~ClassName()" << std::endl;
    std::cout << "2. 示例: obj1.~ExplicitDestructorDemo()" << std::endl;
    std::cout << "3. 注意: 类名必须完整" << std::endl;
    
    std::cout << "\n显式析构的规则：" << std::endl;
    std::cout << "1. 只能对已构造的对象调用" << std::endl;
    std::cout << "2. 不能对已析构的对象再次调用" << std::endl;
    std::cout << "3. 调用后对象处于未定义状态" << std::endl;
    std::cout << "4. 必须手动管理对象生命周期" << std::endl;
    
    std::cout << "\n执行显式析构：" << std::endl;
    obj1.~ExplicitDestructorDemo();
    
    std::cout << "\n显式析构后的状态：" << std::endl;
    std::cout << "1. obj1 的成员被清理" << std::endl;
    std::cout << "2. obj1 的对象地址仍然存在" << std::endl;
    std::cout << "3. obj1 处于未定义状态" << std::endl;
    std::cout << "4. 不能再次调用析构函数" << std::endl;
}

// 演示3：显式析构的应用场景
void demo_explicit_destructor_use_cases() {
    std::cout << "\n=== 演示3：显式析构的应用场景 ===" << std::endl;
    
    std::cout << "显式析构的应用场景：" << std::endl;
    std::cout << "1. 异常安全：在异常处理中清理资源" << std::endl;
    std::cout << "2. 内存管理：在自定义内存管理中" << std::endl;
    std::cout << "3. 容器实现：在容器中管理对象生命周期" << std::endl;
    std::cout << "4. 移动语义：在移动后清理原对象" << std::endl;
    std::cout << "5. 高级编程：在模板元编程中" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "try {" << std::endl;
    std::cout << "    ExplicitDestructorDemo obj1(1, \"source\");" << std::endl;
    std::cout << "    ExplicitDestructorDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // 移动成功，清理原对象" << std::endl;
    std::cout << "    obj1.~ExplicitDestructorDemo();" << std::endl;
    std::cout << "} catch (...) {" << std::endl;
    std::cout << "    // 异常处理" << std::endl;
    std::cout << "    obj1.~ExplicitDestructorDemo();" << std::endl;
    std::cout << "    throw;" << std::endl;
    std::cout << "}" << std::endl;
}

// 演示4：显式析构的注意事项
void demo_explicit_destructor_precautions() {
    std::cout << "\n=== 演示4：显式析构的注意事项 ===" << std::endl;
    
    std::cout << "显式析构的注意事项：" << std::endl;
    std::cout << "1. 只能调用一次：不能重复调用" << std::endl;
    std::cout << "2. 对象状态：调用后对象处于未定义状态" << std::endl;
    std::cout << "3. 内存管理：对象地址的内存由编译器管理" << std::endl;
    std::cout << "4. 异常安全：必须考虑异常安全" << std::endl;
    std::cout << "5. 调试困难：容易导致调试困难" << std::endl;
    
    std::cout << "\n错误示例：" << std::endl;
    std::cout << "ExplicitDestructorDemo obj1(1, \"source\");" << std::endl;
    std::cout << "obj1.~ExplicitDestructorDemo();" << std::endl;
    std::cout << "obj1.~ExplicitDestructorDemo(); // 错误！重复调用" << std::endl;
    std::cout << "obj1.getValue(); // 错误！对象已析构" << std::endl;
    
    std::cout << "\n正确示例：" << std::endl;
    std::cout << "ExplicitDestructorDemo obj1(1, \"source\");" << std::endl;
    std::cout << "ExplicitDestructorDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~ExplicitDestructorDemo(); // 正确" << std::endl;
    std::cout << "// 不再使用 obj1" << std::endl;
}

// 演示5：实际应用中的最佳实践
void demo_best_practices_explicit_destructor() {
    std::cout << "\n=== 演示5：实际应用中的最佳实践 ===" << std::endl;
    
    std::cout << "最佳实践：" << std::endl;
    std::cout << "1. 避免使用：除非必要，避免显式析构" << std::endl;
    std::cout << "2. 异常安全：确保异常安全" << std::endl;
    std::cout << "3. 文档说明：在代码中说明原因" << std::endl;
    std::cout << "4. 测试充分：充分测试相关代码" << std::endl;
    std::cout << "5. 代码审查：进行代码审查" << std::endl;
    
    std::cout << "\n替代方案：" << std::endl;
    std::cout << "1. 使用 RAII：让编译器管理对象生命周期" << std::endl;
    std::cout << "2. 使用智能指针：自动管理对象生命周期" << std::endl;
    std::cout << "3. 使用容器：让容器管理对象生命周期" << std::endl;
    std::cout << "4. 使用作用域：让作用域管理对象生命周期" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "// 推荐做法" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    ExplicitDestructorDemo obj1(1, \"source\");" << std::endl;
    std::cout << "    ExplicitDestructorDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // obj1 在作用域结束时自动析构" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n// 特殊情况" << std::endl;
    std::cout << "ExplicitDestructorDemo obj1(1, \"source\");" << std::endl;
    std::cout << "ExplicitDestructorDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~ExplicitDestructorDemo(); // 特殊情况" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ExplicitDestructorDemo::resetCount();
    
    demo_basic_explicit_destructor();
    demo_explicit_destructor_syntax();
    demo_explicit_destructor_use_cases();
    demo_explicit_destructor_precautions();
    demo_best_practices_explicit_destructor();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "Move 后如何主动调用析构：" << std::endl;
    std::cout << "1. 语法: object.~ClassName()" << std::endl;
    std::cout << "2. 示例: obj1.~ExplicitDestructorDemo()" << std::endl;
    std::cout << "3. 规则: 只能调用一次，调用后对象处于未定义状态" << std::endl;
    std::cout << "4. 应用: 异常安全、内存管理、容器实现等" << std::endl;
    std::cout << "5. 注意: 避免使用，除非必要" << std::endl;
    
    return 0;
}