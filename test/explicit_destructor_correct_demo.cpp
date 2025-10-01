#include <iostream>
#include <memory>
#include <windows.h>

// 演示类：跟踪析构过程
class DetailedResourceClass {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    DetailedResourceClass(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        for (int i = 0; i < 3; ++i) {
            data_[i] = v + i;
        }
        std::cout << "构造 DetailedResourceClass(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    DetailedResourceClass(DetailedResourceClass&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 DetailedResourceClass(" << value_ << ", " << name_ << ")" << std::endl;
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
    
    ~DetailedResourceClass() {
        --count_;
        std::cout << "析构 DetailedResourceClass(" << value_ << ", " << name_ << ")" << std::endl;
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

int DetailedResourceClass::count_ = 0;

// 演示1：错误的做法
void demo_wrong_approach() {
    std::cout << "=== 演示1：错误的做法 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    DetailedResourceClass obj1(1, "source");
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n显式析构 obj1：" << std::endl;
    obj1.~DetailedResourceClass();
    
    std::cout << "\n错误：尝试调用已析构对象的成员函数" << std::endl;
    std::cout << "obj1.printState(); // 错误！对象已析构" << std::endl;
    std::cout << "// 这会导致未定义行为" << std::endl;
    
    // 注释掉错误的代码
    // obj1.printState(); // 错误！不能调用已析构对象的成员函数
}

// 演示2：正确的做法
void demo_correct_approach() {
    std::cout << "\n=== 演示2：正确的做法 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    DetailedResourceClass obj1(2, "source");
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n显式析构 obj1：" << std::endl;
    obj1.~DetailedResourceClass();
    
    std::cout << "\n正确：使用 placement new 重新构造 obj1" << std::endl;
    new(&obj1) DetailedResourceClass(5, "hello");
    
    std::cout << "\n重新构造后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\nobj2 仍然有效：" << std::endl;
    obj2.printState();
}

// 演示3：完整的正确流程
void demo_complete_correct_flow() {
    std::cout << "\n=== 演示3：完整的正确流程 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    std::cout << "\n步骤1：创建对象" << std::endl;
    DetailedResourceClass obj1(3, "original");
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n步骤2：移动后状态" << std::endl;
    std::cout << "obj1 状态：" << std::endl;
    obj1.printState();
    std::cout << "obj2 状态：" << std::endl;
    obj2.printState();
    
    std::cout << "\n步骤3：显式析构 obj1" << std::endl;
    obj1.~DetailedResourceClass();
    std::cout << "obj1 已析构，处于未定义状态" << std::endl;
    
    std::cout << "\n步骤4：使用 placement new 重新构造 obj1" << std::endl;
    new(&obj1) DetailedResourceClass(5, "hello");
    std::cout << "obj1 重新构造完成" << std::endl;
    
    std::cout << "\n步骤5：重新构造后的状态" << std::endl;
    std::cout << "obj1 状态：" << std::endl;
    obj1.printState();
    std::cout << "obj2 状态：" << std::endl;
    obj2.printState();
    
    std::cout << "\n步骤6：作用域结束，自动析构" << std::endl;
}

// 演示4：异常安全处理
void demo_exception_safety() {
    std::cout << "\n=== 演示4：异常安全处理 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    try {
        DetailedResourceClass obj1(4, "source");
        DetailedResourceClass obj2 = std::move(obj1);
        
        std::cout << "\n移动后状态：" << std::endl;
        obj1.printState();
        obj2.printState();
        
        std::cout << "\n显式析构 obj1：" << std::endl;
        obj1.~DetailedResourceClass();
        
        std::cout << "\n使用 placement new 重新构造 obj1：" << std::endl;
        new(&obj1) DetailedResourceClass(6, "reconstructed");
        
        std::cout << "\n重新构造后状态：" << std::endl;
        obj1.printState();
        obj2.printState();
        
        std::cout << "\n模拟异常：" << std::endl;
        throw std::runtime_error("模拟异常");
        
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "obj1 和 obj2 将在作用域结束时自动析构" << std::endl;
    }
}

// 演示5：最佳实践
void demo_best_practices() {
    std::cout << "\n=== 演示5：最佳实践 ===" << std::endl;
    
    std::cout << "最佳实践：" << std::endl;
    std::cout << "1. 避免显式析构：除非必要，避免显式析构" << std::endl;
    std::cout << "2. 使用 RAII：让编译器管理对象生命周期" << std::endl;
    std::cout << "3. 异常安全：确保异常安全" << std::endl;
    std::cout << "4. 文档说明：在代码中说明原因" << std::endl;
    std::cout << "5. 测试充分：充分测试相关代码" << std::endl;
    
    std::cout << "\n推荐做法：" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    DetailedResourceClass obj1(1, \"source\");" << std::endl;
    std::cout << "    DetailedResourceClass obj2 = std::move(obj1);" << std::endl;
    std::cout << "    // obj1 在作用域结束时自动析构" << std::endl;
    std::cout << "    // obj2 继续使用" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n特殊情况：" << std::endl;
    std::cout << "DetailedResourceClass obj1(1, \"source\");" << std::endl;
    std::cout << "DetailedResourceClass obj2 = std::move(obj1);" << std::endl;
    std::cout << "obj1.~DetailedResourceClass(); // 显式析构" << std::endl;
    std::cout << "new(&obj1) DetailedResourceClass(5, \"hello\"); // 重新构造" << std::endl;
    std::cout << "obj1.printState(); // 现在可以安全调用" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    DetailedResourceClass::resetCount();
    
    demo_wrong_approach();
    demo_correct_approach();
    demo_complete_correct_flow();
    demo_exception_safety();
    demo_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么显式析构后调用 obj1.printState() 会报错：" << std::endl;
    std::cout << "1. 显式析构后对象处于未定义状态" << std::endl;
    std::cout << "2. 不能调用任何成员函数" << std::endl;
    std::cout << "3. 需要使用 placement new 重新构造" << std::endl;
    std::cout << "4. 重新构造后才能安全调用成员函数" << std::endl;
    std::cout << "5. 建议使用 RAII 避免这种复杂性" << std::endl;
    
    return 0;
}






