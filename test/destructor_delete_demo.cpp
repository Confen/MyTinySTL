#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 演示类：展示析构函数中必须显式delete的原因
class ResourceManager {
private:
    int* data_;
    std::string name_;
    static int count_;
    
public:
    ResourceManager(int size, const std::string& n) : name_(n) {
        ++count_;
        data_ = new int[size];  // 动态分配内存
        for (int i = 0; i < size; ++i) {
            data_[i] = i;
        }
        std::cout << "构造 ResourceManager(" << size << ", " << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  总数: " << count_ << std::endl;
    }
    
    ~ResourceManager() {
        --count_;
        std::cout << "析构 ResourceManager(" << name_ << ")" << std::endl;
        std::cout << "  对象地址: " << this << std::endl;
        std::cout << "  数据地址: " << data_ << std::endl;
        std::cout << "  剩余: " << count_ << std::endl;
        
        // 必须显式delete！
        if (data_) {
            std::cout << "  释放数据内存: " << data_ << std::endl;
            delete[] data_;  // 显式释放动态分配的内存
            data_ = nullptr; // 防止重复释放
        } else {
            std::cout << "  数据指针为nullptr，无需释放数据" << std::endl;
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

int ResourceManager::count_ = 0;

// 演示1：为什么必须显式delete
void demo_why_explicit_delete_required() {
    std::cout << "=== 演示1：为什么必须显式delete ===" << std::endl;
    
    ResourceManager::resetCount();
    
    {
        ResourceManager obj(5, "test");
        std::cout << "\n对象创建后状态：" << std::endl;
        obj.printState();
        
        std::cout << "\n关键点：" << std::endl;
        std::cout << "1. 构造函数中使用 new 分配内存" << std::endl;
        std::cout << "2. 析构函数中必须使用 delete 释放内存" << std::endl;
        std::cout << "3. 这是 RAII 原则的基本要求" << std::endl;
        std::cout << "4. 编译器不会自动释放动态分配的内存" << std::endl;
    } // 对象离开作用域，自动调用析构函数
    
    std::cout << "\n对象离开作用域后，析构函数自动调用" << std::endl;
}

// 演示2：不显式delete的后果
void demo_consequences_of_no_explicit_delete() {
    std::cout << "\n=== 演示2：不显式delete的后果 ===" << std::endl;
    
    std::cout << "如果不显式delete会发生什么：" << std::endl;
    std::cout << "1. 内存泄漏：动态分配的内存不会被释放" << std::endl;
    std::cout << "2. 资源泄漏：系统资源不会被回收" << std::endl;
    std::cout << "3. 程序崩溃：长时间运行可能导致内存耗尽" << std::endl;
    std::cout << "4. 违反RAII：资源获取与释放不匹配" << std::endl;
    
    std::cout << "\n错误示例：" << std::endl;
    std::cout << "class BadClass {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    BadClass() { data_ = new int[10]; }" << std::endl;
    std::cout << "    ~BadClass() { /* 没有delete[] data_！ */ }" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n正确示例：" << std::endl;
    std::cout << "class GoodClass {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    GoodClass() { data_ = new int[10]; }" << std::endl;
    std::cout << "    ~GoodClass() { delete[] data_; } // 必须显式delete！" << std::endl;
    std::cout << "};" << std::endl;
}

// 演示3：不同类型的delete
void demo_different_types_of_delete() {
    std::cout << "\n=== 演示3：不同类型的delete ===" << std::endl;
    
    std::cout << "delete 的类型：" << std::endl;
    std::cout << "1. delete ptr：释放单个对象" << std::endl;
    std::cout << "2. delete[] ptr：释放数组对象" << std::endl;
    std::cout << "3. delete ptr：释放new分配的内存" << std::endl;
    std::cout << "4. delete[] ptr：释放new[]分配的内存" << std::endl;
    
    std::cout << "\n匹配原则：" << std::endl;
    std::cout << "new int → delete ptr" << std::endl;
    std::cout << "new int[10] → delete[] ptr" << std::endl;
    std::cout << "new MyClass → delete ptr" << std::endl;
    std::cout << "new MyClass[5] → delete[] ptr" << std::endl;
    
    std::cout << "\n错误匹配的后果：" << std::endl;
    std::cout << "int* arr = new int[10];" << std::endl;
    std::cout << "delete arr; // 错误！应该用delete[]" << std::endl;
    std::cout << "delete[] arr; // 正确！" << std::endl;
}

// 演示4：智能指针的自动管理
void demo_smart_pointers_auto_management() {
    std::cout << "\n=== 演示4：智能指针的自动管理 ===" << std::endl;
    
    std::cout << "智能指针自动管理：" << std::endl;
    std::cout << "1. std::unique_ptr：自动释放单个对象" << std::endl;
    std::cout << "2. std::shared_ptr：共享所有权，自动释放" << std::endl;
    std::cout << "3. std::unique_ptr<int[]>：自动释放数组" << std::endl;
    
    std::cout << "\n智能指针示例：" << std::endl;
    std::cout << "class SmartClass {" << std::endl;
    std::cout << "    std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    SmartClass() : data_(std::make_unique<int[]>(10)) {}" << std::endl;
    std::cout << "    // 析构函数中不需要显式delete！" << std::endl;
    std::cout << "    // unique_ptr会自动释放内存" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n优势：" << std::endl;
    std::cout << "1. 自动内存管理：无需手动delete" << std::endl;
    std::cout << "2. 异常安全：即使发生异常也能正确释放" << std::endl;
    std::cout << "3. 防止内存泄漏：自动释放资源" << std::endl;
    std::cout << "4. 简化代码：减少手动内存管理" << std::endl;
}

// 演示5：最佳实践
void demo_best_practices() {
    std::cout << "\n=== 演示5：最佳实践 ===" << std::endl;
    
    std::cout << "最佳实践：" << std::endl;
    std::cout << "1. 使用智能指针：优先使用智能指针" << std::endl;
    std::cout << "2. RAII原则：资源获取即初始化" << std::endl;
    std::cout << "3. 显式delete：如果必须手动管理，必须显式delete" << std::endl;
    std::cout << "4. 异常安全：确保异常情况下也能正确释放" << std::endl;
    
    std::cout << "\n推荐做法：" << std::endl;
    std::cout << "// 使用智能指针" << std::endl;
    std::cout << "std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "data_ = std::make_unique<int[]>(10);" << std::endl;
    
    std::cout << "\n// 如果必须手动管理" << std::endl;
    std::cout << "int* data_;" << std::endl;
    std::cout << "data_ = new int[10];" << std::endl;
    std::cout << "// 析构函数中必须：" << std::endl;
    std::cout << "delete[] data_;" << std::endl;
    
    std::cout << "\n// 异常安全版本" << std::endl;
    std::cout << "~MyClass() {" << std::endl;
    std::cout << "    if (data_) {" << std::endl;
    std::cout << "        delete[] data_;" << std::endl;
    std::cout << "        data_ = nullptr;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ResourceManager::resetCount();
    
    demo_why_explicit_delete_required();
    demo_consequences_of_no_explicit_delete();
    demo_different_types_of_delete();
    demo_smart_pointers_auto_management();
    demo_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "析构函数中是否必须显式delete所产生的new：" << std::endl;
    std::cout << "1. 是的，必须显式delete" << std::endl;
    std::cout << "2. 编译器不会自动释放动态分配的内存" << std::endl;
    std::cout << "3. 这是RAII原则的基本要求" << std::endl;
    std::cout << "4. 不显式delete会导致内存泄漏" << std::endl;
    std::cout << "5. 推荐使用智能指针自动管理内存" << std::endl;
    
    return 0;
}






