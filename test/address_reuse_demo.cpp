#include <iostream>
#include <memory>
#include <windows.h>

// 演示类：跟踪对象状态
class AddressReuseDemo {
private:
    int value_;
    int* data_;
    std::string name_;
    static int count_;
    
public:
    AddressReuseDemo(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        data_ = new int[3];
        std::cout << "构造 AddressReuseDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    AddressReuseDemo(AddressReuseDemo&& other) noexcept 
        : value_(other.value_), data_(other.data_), name_(std::move(other.name_)) {
        ++count_;
        std::cout << "移动构造 AddressReuseDemo(" << value_ << ", " << name_ << ")" << std::endl;
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
    
    // 拷贝构造函数
    AddressReuseDemo(const AddressReuseDemo& other) : value_(other.value_), name_(other.name_) {
        ++count_;
        data_ = new int[3];
        for (int i = 0; i < 3; ++i) {
            data_[i] = other.data_[i];
        }
        std::cout << "拷贝构造 AddressReuseDemo(" << value_ << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  对象大小: " << sizeof(*this) << " 字节" << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    // 赋值操作符
    AddressReuseDemo& operator=(const AddressReuseDemo& other) {
        if (this != &other) {
            value_ = other.value_;
            name_ = other.name_;
            if (data_) {
                delete[] data_;
            }
            data_ = new int[3];
            for (int i = 0; i < 3; ++i) {
                data_[i] = other.data_[i];
            }
            std::cout << "赋值操作 AddressReuseDemo(" << value_ << ", " << name_ << ")" << std::endl;
            std::cout << "  对象地址: " << this << std::endl;
            std::cout << "  数据地址: " << data_ << std::endl;
        }
        return *this;
    }
    
    ~AddressReuseDemo() {
        --count_;
        std::cout << "析构 AddressReuseDemo(" << value_ << ", " << name_ << ")" << std::endl;
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

int AddressReuseDemo::count_ = 0;

// 演示1：原对象地址的状态
void demo_original_object_address_state() {
    std::cout << "=== 演示1：原对象地址的状态 ===" << std::endl;
    
    AddressReuseDemo::resetCount();
    
    // 创建对象
    AddressReuseDemo obj1(1, "original");
    std::cout << "\n移动前 obj1：" << std::endl;
    obj1.printState();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    AddressReuseDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1：" << std::endl;
    obj1.printState();
    
    std::cout << "\n移动后 obj2：" << std::endl;
    obj2.printState();
    
    std::cout << "\n关键发现：" << std::endl;
    std::cout << "1. obj1 的对象地址不变: " << &obj1 << std::endl;
    std::cout << "2. obj1 的成员被转移: data_=nullptr, name_=moved_away" << std::endl;
    std::cout << "3. obj1 的对象地址仍然存在" << std::endl;
    std::cout << "4. obj1 的对象地址理论上可以复用" << std::endl;
}

// 演示2：尝试复用原对象地址
void demo_try_reuse_original_address() {
    std::cout << "\n=== 演示2：尝试复用原对象地址 ===" << std::endl;
    
    AddressReuseDemo::resetCount();
    
    AddressReuseDemo obj1(2, "source");
    AddressReuseDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n尝试复用 obj1 的地址：" << std::endl;
    std::cout << "1. obj1 的地址: " << &obj1 << std::endl;
    std::cout << "2. obj1 的成员已被转移" << std::endl;
    std::cout << "3. obj1 处于未定义状态" << std::endl;
    std::cout << "4. 直接使用 obj1 是危险的" << std::endl;
    
    // 尝试重新初始化 obj1
    std::cout << "\n尝试重新初始化 obj1：" << std::endl;
    try {
        // 这是危险的！obj1 已经被移动，处于未定义状态
        // 直接使用 obj1 可能导致未定义行为
        std::cout << "obj1.getValue(): " << obj1.getValue() << std::endl;
        std::cout << "obj1.getName(): " << obj1.getName() << std::endl;
        std::cout << "obj1.getData(): " << obj1.getData() << std::endl;
    } catch (...) {
        std::cout << "访问 obj1 时发生异常" << std::endl;
    }
}

// 演示3：安全的地址复用方法
void demo_safe_address_reuse() {
    std::cout << "\n=== 演示3：安全的地址复用方法 ===" << std::endl;
    
    AddressReuseDemo::resetCount();
    
    AddressReuseDemo obj1(3, "source");
    AddressReuseDemo obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n安全的地址复用方法：" << std::endl;
    std::cout << "1. 显式析构 obj1" << std::endl;
    obj1.~AddressReuseDemo();
    
    std::cout << "\n2. 使用 placement new 重新构造" << std::endl;
    new(&obj1) AddressReuseDemo(4, "reused");
    
    std::cout << "\n3. 重新构造后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n4. obj2 仍然有效：" << std::endl;
    obj2.printState();
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. 必须先显式析构原对象" << std::endl;
    std::cout << "2. 使用 placement new 重新构造" << std::endl;
    std::cout << "3. 这是高级技术，需要小心使用" << std::endl;
    std::cout << "4. 通常不建议这样做" << std::endl;
}

// 演示4：为什么通常不建议复用地址
void demo_why_not_recommend_reuse() {
    std::cout << "\n=== 演示4：为什么通常不建议复用地址 ===" << std::endl;
    
    std::cout << "不建议复用原对象地址的原因：" << std::endl;
    std::cout << "1. 复杂性：需要手动管理对象生命周期" << std::endl;
    std::cout << "2. 风险性：容易导致未定义行为" << std::endl;
    std::cout << "3. 维护性：代码难以理解和维护" << std::endl;
    std::cout << "4. 异常安全：难以保证异常安全" << std::endl;
    std::cout << "5. 标准库：标准库不保证这种用法" << std::endl;
    
    std::cout << "\n更好的替代方案：" << std::endl;
    std::cout << "1. 使用新对象：创建新对象而不是复用地址" << std::endl;
    std::cout << "2. 使用容器：让容器管理对象生命周期" << std::endl;
    std::cout << "3. 使用智能指针：自动管理对象生命周期" << std::endl;
    std::cout << "4. 使用 RAII：让编译器管理对象生命周期" << std::endl;
}

// 演示5：实际应用中的最佳实践
void demo_best_practices() {
    std::cout << "\n=== 演示5：实际应用中的最佳实践 ===" << std::endl;
    
    std::cout << "最佳实践：" << std::endl;
    std::cout << "1. 移动后不要使用原对象" << std::endl;
    std::cout << "2. 让原对象自然析构" << std::endl;
    std::cout << "3. 使用新对象存储移动的结果" << std::endl;
    std::cout << "4. 遵循 RAII 原则" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "// 好的做法" << std::endl;
    std::cout << "AddressReuseDemo obj1(1, \"source\");" << std::endl;
    std::cout << "AddressReuseDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "// obj1 不再使用，让它在作用域结束时自然析构" << std::endl;
    std::cout << "// obj2 继续使用" << std::endl;
    
    std::cout << "\n// 不好的做法" << std::endl;
    std::cout << "AddressReuseDemo obj1(1, \"source\");" << std::endl;
    std::cout << "AddressReuseDemo obj2 = std::move(obj1);" << std::endl;
    std::cout << "// 尝试复用 obj1 的地址" << std::endl;
    std::cout << "// 这是危险的，不推荐" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    AddressReuseDemo::resetCount();
    
    demo_original_object_address_state();
    demo_try_reuse_original_address();
    demo_safe_address_reuse();
    demo_why_not_recommend_reuse();
    demo_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "原对象地址可以复用吗？" << std::endl;
    std::cout << "1. 理论上可以：使用 placement new 重新构造" << std::endl;
    std::cout << "2. 实际中不建议：存在风险和复杂性" << std::endl;
    std::cout << "3. 更好的方案：使用新对象或让原对象自然析构" << std::endl;
    std::cout << "4. 遵循最佳实践：让编译器管理对象生命周期" << std::endl;
    
    return 0;
}






