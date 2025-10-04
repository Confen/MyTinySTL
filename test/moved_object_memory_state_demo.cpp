#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：详细跟踪资源状态
class DetailedResourceClass {
private:
    int* data_;
    int size_;
    std::string name_;
    static int count_;
    
public:
    DetailedResourceClass(int size, const std::string& name) 
        : size_(size), name_(name) {
        ++count_;
        data_ = new int[size];
        std::cout << "构造 DetailedResourceClass(" << name_ << "), 地址: " << this 
                  << ", 数据地址: " << data_ << ", 大小: " << size_ << ", 总数: " << count_ << std::endl;
        for (int i = 0; i < size_; ++i) {
            data_[i] = i;
        }
    }
    
    // 拷贝构造函数
    DetailedResourceClass(const DetailedResourceClass& other) 
        : size_(other.size_), name_(other.name_ + "_copy") {
        ++count_;
        data_ = new int[size_];
        std::cout << "拷贝构造 DetailedResourceClass(" << name_ << "), 地址: " << this 
                  << ", 数据地址: " << data_ << ", 大小: " << size_ << ", 总数: " << count_ << std::endl;
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    // 移动构造函数
    DetailedResourceClass(DetailedResourceClass&& other) noexcept 
        : size_(other.size_), name_(other.name_ + "_moved"), data_(other.data_) {
        ++count_;
        std::cout << "移动构造 DetailedResourceClass(" << name_ << "), 地址: " << this 
                  << ", 数据地址: " << data_ << ", 大小: " << size_ << ", 总数: " << count_ << std::endl;
        
        // 转移资源所有权
        other.data_ = nullptr;
        other.size_ = 0;
        other.name_ = "moved_away";
        
        std::cout << "  原对象状态: 地址=" << &other << ", 数据地址=" << other.data_ 
                  << ", 大小=" << other.size_ << ", 名称=" << other.name_ << std::endl;
    }
    
    ~DetailedResourceClass() {
        --count_;
        std::cout << "析构 DetailedResourceClass(" << name_ << "), 地址: " << this 
                  << ", 数据地址: " << data_ << ", 大小: " << size_ << ", 剩余: " << count_ << std::endl;
        if (data_) {
            delete[] data_;
            std::cout << "  释放数据内存: " << data_ << std::endl;
        } else {
            std::cout << "  数据指针为nullptr，无需释放" << std::endl;
        }
    }
    
    int getSize() const { return size_; }
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
    
    // 打印对象状态
    void printState() const {
        std::cout << "对象状态: 地址=" << this << ", 数据地址=" << data_ 
                  << ", 大小=" << size_ << ", 名称=" << name_ << std::endl;
    }
};

int DetailedResourceClass::count_ = 0;

// 演示1：移动后原对象的内存状态
void demo_moved_object_memory_state() {
    std::cout << "=== 演示1：移动后原对象的内存状态 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    // 创建对象
    DetailedResourceClass obj1(5, "obj1");
    std::cout << "\n移动前 obj1 状态：" << std::endl;
    obj1.printState();
    
    // 移动构造
    std::cout << "\n执行移动构造：" << std::endl;
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 状态：" << std::endl;
    obj1.printState();
    
    std::cout << "\n移动后 obj2 状态：" << std::endl;
    obj2.printState();
    
    std::cout << "\n分析：" << std::endl;
    std::cout << "1. obj1 的内存仍然被占用" << std::endl;
    std::cout << "2. 但 obj1 的数据指针已转移给 obj2" << std::endl;
    std::cout << "3. obj1 的数据指针现在是 nullptr" << std::endl;
    std::cout << "4. obj1 的大小被设置为 0" << std::endl;
    std::cout << "5. obj1 的名称被标记为 'moved_away'" << std::endl;
}

// 演示2：移动后原对象的资源状态
void demo_moved_object_resource_state() {
    std::cout << "\n=== 演示2：移动后原对象的资源状态 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    DetailedResourceClass obj1(10, "original");
    
    std::cout << "\n移动前：" << std::endl;
    std::cout << "obj1 数据地址: " << obj1.getData() << std::endl;
    std::cout << "obj1 大小: " << obj1.getSize() << std::endl;
    
    // 移动构造
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n移动后：" << std::endl;
    std::cout << "obj1 数据地址: " << obj1.getData() << std::endl;
    std::cout << "obj1 大小: " << obj1.getSize() << std::endl;
    std::cout << "obj2 数据地址: " << obj2.getData() << std::endl;
    std::cout << "obj2 大小: " << obj2.getSize() << std::endl;
    
    std::cout << "\n关键点：" << std::endl;
    std::cout << "1. obj1 的数据指针现在是 nullptr" << std::endl;
    std::cout << "2. obj2 获得了 obj1 的原始数据指针" << std::endl;
    std::cout << "3. 数据内存没有复制，只是转移了所有权" << std::endl;
    std::cout << "4. obj1 仍然存在，但资源已被转移" << std::endl;
}

// 演示3：移动后原对象的析构
void demo_moved_object_destruction() {
    std::cout << "\n=== 演示3：移动后原对象的析构 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    {
        DetailedResourceClass obj1(3, "temp");
        DetailedResourceClass obj2 = std::move(obj1);
        
        std::cout << "\n移动后 obj1 状态：" << std::endl;
        obj1.printState();
        
        std::cout << "\n显式析构 obj1：" << std::endl;
        obj1.~DetailedResourceClass();
        
        std::cout << "\n析构后 obj1 状态：" << std::endl;
        obj1.printState();
        
        std::cout << "\nobj2 仍然有效：" << std::endl;
        obj2.printState();
    }
    
    std::cout << "\n作用域结束，obj2 自动析构" << std::endl;
}

// 演示4：移动后原对象的内存布局
void demo_moved_object_memory_layout() {
    std::cout << "\n=== 演示4：移动后原对象的内存布局 ===" << std::endl;
    
    DetailedResourceClass::resetCount();
    
    DetailedResourceClass obj1(4, "layout_test");
    
    std::cout << "\n移动前 obj1 内存布局：" << std::endl;
    std::cout << "对象地址: " << &obj1 << std::endl;
    std::cout << "数据指针: " << obj1.getData() << std::endl;
    std::cout << "大小: " << obj1.getSize() << std::endl;
    std::cout << "名称: " << obj1.getName() << std::endl;
    
    // 移动构造
    DetailedResourceClass obj2 = std::move(obj1);
    
    std::cout << "\n移动后 obj1 内存布局：" << std::endl;
    std::cout << "对象地址: " << &obj1 << std::endl;
    std::cout << "数据指针: " << obj1.getData() << std::endl;
    std::cout << "大小: " << obj1.getSize() << std::endl;
    std::cout << "名称: " << obj1.getName() << std::endl;
    
    std::cout << "\n移动后 obj2 内存布局：" << std::endl;
    std::cout << "对象地址: " << &obj2 << std::endl;
    std::cout << "数据指针: " << obj2.getData() << std::endl;
    std::cout << "大小: " << obj2.getSize() << std::endl;
    std::cout << "名称: " << obj2.getName() << std::endl;
    
    std::cout << "\n分析：" << std::endl;
    std::cout << "1. obj1 的内存仍然被占用" << std::endl;
    std::cout << "2. 但 obj1 的数据指针已转移" << std::endl;
    std::cout << "3. obj1 的其他成员变量被重置" << std::endl;
    std::cout << "4. obj2 获得了 obj1 的原始资源" << std::endl;
}

// 演示5：移动后原对象的资源类型
void demo_moved_object_resource_types() {
    std::cout << "\n=== 演示5：移动后原对象的资源类型 ===" << std::endl;
    
    std::cout << "移动后原对象内存中的资源类型：" << std::endl;
    std::cout << "1. 对象本身的内存：仍然被占用" << std::endl;
    std::cout << "2. 动态分配的资源：已转移给新对象" << std::endl;
    std::cout << "3. 成员变量：被重置为默认值" << std::endl;
    std::cout << "4. 指针成员：通常设置为 nullptr" << std::endl;
    std::cout << "5. 大小成员：通常设置为 0" << std::endl;
    
    std::cout << "\n具体示例：" << std::endl;
    std::cout << "class ResourceClass {" << std::endl;
    std::cout << "    int* data_;      // 移动后变为 nullptr" << std::endl;
    std::cout << "    int size_;       // 移动后变为 0" << std::endl;
    std::cout << "    string name_;    // 移动后变为 'moved_away'" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n移动后原对象的状态：" << std::endl;
    std::cout << "1. 对象内存：仍然存在" << std::endl;
    std::cout << "2. 动态资源：已转移" << std::endl;
    std::cout << "3. 成员变量：已重置" << std::endl;
    std::cout << "4. 对象状态：有效但未定义" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    DetailedResourceClass::resetCount();
    
    demo_moved_object_memory_state();
    demo_moved_object_resource_state();
    demo_moved_object_destruction();
    demo_moved_object_memory_layout();
    demo_moved_object_resource_types();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "移动后原位置对象内存中的资源：" << std::endl;
    std::cout << "1. 对象本身的内存：仍然被占用" << std::endl;
    std::cout << "2. 动态分配的资源：已转移给新对象" << std::endl;
    std::cout << "3. 成员变量：被重置为默认值" << std::endl;
    std::cout << "4. 指针成员：通常设置为 nullptr" << std::endl;
    std::cout << "5. 大小成员：通常设置为 0" << std::endl;
    std::cout << "6. 对象状态：有效但未定义" << std::endl;
    std::cout << "7. 必须被析构：释放对象本身的内存" << std::endl;
    
    return 0;
}
















