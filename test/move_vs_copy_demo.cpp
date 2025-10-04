#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：跟踪构造、复制、移动
class TrackedClass {
private:
    int value_;
    static int count_;
    
public:
    TrackedClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 TrackedClass(" << value_ << "), 地址: " << this << ", 总数: " << count_ << std::endl;
    }
    
    // 拷贝构造函数
    TrackedClass(const TrackedClass& other) : value_(other.value_) {
        ++count_;
        std::cout << "拷贝构造 TrackedClass(" << value_ << "), 地址: " << this << ", 总数: " << count_ << std::endl;
    }
    
    // 移动构造函数
    TrackedClass(TrackedClass&& other) noexcept : value_(other.value_) {
        ++count_;
        std::cout << "移动构造 TrackedClass(" << value_ << "), 地址: " << this << ", 总数: " << count_ << std::endl;
        other.value_ = -1;  // 标记为已移动
    }
    
    // 拷贝赋值操作符
    TrackedClass& operator=(const TrackedClass& other) {
        std::cout << "拷贝赋值 TrackedClass(" << value_ << ") = TrackedClass(" << other.value_ << ")" << std::endl;
        value_ = other.value_;
        return *this;
    }
    
    // 移动赋值操作符
    TrackedClass& operator=(TrackedClass&& other) noexcept {
        std::cout << "移动赋值 TrackedClass(" << value_ << ") = TrackedClass(" << other.value_ << ")" << std::endl;
        value_ = other.value_;
        other.value_ = -1;  // 标记为已移动
        return *this;
    }
    
    ~TrackedClass() {
        --count_;
        std::cout << "析构 TrackedClass(" << value_ << "), 地址: " << this << ", 剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int TrackedClass::count_ = 0;

// 简化的动态数组实现
template<typename T>
class SimpleVector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {
        std::cout << "SimpleVector 构造" << std::endl;
    }
    
    ~SimpleVector() {
        std::cout << "SimpleVector 析构" << std::endl;
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        delete[] reinterpret_cast<char*>(data_);
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        
        new(data_ + size_) T(value);
        ++size_;
    }
    
    // 详细演示移动元素的过程
    void erase_detailed(size_t index) {
        if (index >= size_) return;
        
        std::cout << "\n=== 详细演示移动元素的过程 ===" << std::endl;
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 步骤1：析构要删除的元素
        std::cout << "\n步骤1：析构要删除的元素 data_[" << index << "]" << std::endl;
        data_[index].~T();
        
        // 步骤2：移动后面的元素
        std::cout << "\n步骤2：移动后面的元素" << std::endl;
        for (size_t i = index; i < size_ - 1; ++i) {
            std::cout << "  移动元素 " << (i + 1) << " 到位置 " << i << std::endl;
            
            // 在位置i构造新对象（移动构造）
            std::cout << "    在位置 " << i << " 移动构造新对象" << std::endl;
            new(data_ + i) T(std::move(data_[i + 1]));
            
            // 析构原位置的对象
            std::cout << "    析构原位置 " << (i + 1) << " 的对象" << std::endl;
            data_[i + 1].~T();
        }
        
        --size_;
        std::cout << "\n删除完成，当前大小: " << size_ << std::endl;
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        
        T* new_data = reinterpret_cast<T*>(new char[sizeof(T) * new_capacity]);
        
        for (size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        delete[] reinterpret_cast<char*>(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    
    void printStatus() {
        std::cout << "数组状态：大小 " << size_ << ", 容量 " << capacity_ << std::endl;
        std::cout << "元素: ";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data_[i].getValue() << " ";
        }
        std::cout << std::endl;
    }
};

// 演示1：移动语义 vs 复制语义
void demo_move_vs_copy() {
    std::cout << "=== 演示1：移动语义 vs 复制语义 ===" << std::endl;
    
    TrackedClass::resetCount();
    
    // 创建对象
    TrackedClass obj1(1);
    TrackedClass obj2(2);
    
    std::cout << "\n对象1: " << obj1.getValue() << ", 地址: " << &obj1 << std::endl;
    std::cout << "对象2: " << obj2.getValue() << ", 地址: " << &obj2 << std::endl;
    
    // 复制语义
    std::cout << "\n=== 复制语义 ===" << std::endl;
    TrackedClass obj3 = obj1;  // 拷贝构造
    std::cout << "复制后：" << std::endl;
    std::cout << "对象1: " << obj1.getValue() << ", 地址: " << &obj1 << std::endl;
    std::cout << "对象3: " << obj3.getValue() << ", 地址: " << &obj3 << std::endl;
    std::cout << "对象1和对象3是不同的对象，但值相同" << std::endl;
    
    // 移动语义
    std::cout << "\n=== 移动语义 ===" << std::endl;
    TrackedClass obj4 = std::move(obj2);  // 移动构造
    std::cout << "移动后：" << std::endl;
    std::cout << "对象2: " << obj2.getValue() << ", 地址: " << &obj2 << std::endl;
    std::cout << "对象4: " << obj4.getValue() << ", 地址: " << &obj4 << std::endl;
    std::cout << "对象2的值被标记为-1（已移动），对象4获得原值" << std::endl;
}

// 演示2：移动语义在数组操作中的应用
void demo_move_in_array() {
    std::cout << "\n=== 演示2：移动语义在数组操作中的应用 ===" << std::endl;
    
    SimpleVector<TrackedClass> vec;
    TrackedClass::resetCount();
    
    // 添加元素
    std::cout << "添加元素：" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(TrackedClass(i));
    }
    
    vec.printStatus();
    std::cout << "对象总数: " << TrackedClass::getCount() << std::endl;
    
    // 删除中间元素
    vec.erase_detailed(2);
    
    vec.printStatus();
    std::cout << "对象总数: " << TrackedClass::getCount() << std::endl;
}

// 演示3：移动语义的优势
void demo_move_advantages() {
    std::cout << "\n=== 演示3：移动语义的优势 ===" << std::endl;
    
    // 包含大量数据的类
    class LargeDataClass {
    private:
        std::vector<int> data_;
        
    public:
        LargeDataClass(size_t size) : data_(size) {
            std::cout << "构造 LargeDataClass，大小: " << size << std::endl;
            for (size_t i = 0; i < size; ++i) {
                data_[i] = i;
            }
        }
        
        // 拷贝构造函数
        LargeDataClass(const LargeDataClass& other) : data_(other.data_) {
            std::cout << "拷贝构造 LargeDataClass" << std::endl;
        }
        
        // 移动构造函数
        LargeDataClass(LargeDataClass&& other) noexcept : data_(std::move(other.data_)) {
            std::cout << "移动构造 LargeDataClass" << std::endl;
        }
        
        size_t size() const { return data_.size(); }
    };
    
    // 创建大对象
    LargeDataClass large1(1000);
    
    // 复制（慢）
    std::cout << "\n复制大对象：" << std::endl;
    LargeDataClass large2 = large1;
    
    // 移动（快）
    std::cout << "\n移动大对象：" << std::endl;
    LargeDataClass large3 = std::move(large1);
    
    std::cout << "移动语义避免了不必要的复制！" << std::endl;
}

// 演示4：移动语义的实现原理
void demo_move_implementation() {
    std::cout << "\n=== 演示4：移动语义的实现原理 ===" << std::endl;
    
    std::cout << "移动语义的实现原理：" << std::endl;
    std::cout << "1. 移动构造函数接收右值引用参数" << std::endl;
    std::cout << "2. 直接转移资源所有权，不复制数据" << std::endl;
    std::cout << "3. 将源对象置于有效但未定义的状态" << std::endl;
    std::cout << "4. 避免深拷贝，提高性能" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "TrackedClass(TrackedClass&& other) noexcept : value_(other.value_) {" << std::endl;
    std::cout << "    other.value_ = -1;  // 标记为已移动" << std::endl;
    std::cout << "}" << std::endl;
}

// 演示5：移动语义的注意事项
void demo_move_precautions() {
    std::cout << "\n=== 演示5：移动语义的注意事项 ===" << std::endl;
    
    std::cout << "移动语义的注意事项：" << std::endl;
    std::cout << "1. 移动后源对象处于有效但未定义的状态" << std::endl;
    std::cout << "2. 不能假设移动后源对象的值" << std::endl;
    std::cout << "3. 移动构造函数应该标记为 noexcept" << std::endl;
    std::cout << "4. 移动后源对象仍然可以被析构" << std::endl;
    
    TrackedClass::resetCount();
    
    TrackedClass obj1(1);
    std::cout << "移动前: " << obj1.getValue() << std::endl;
    
    TrackedClass obj2 = std::move(obj1);
    std::cout << "移动后 obj1: " << obj1.getValue() << std::endl;
    std::cout << "移动后 obj2: " << obj2.getValue() << std::endl;
    
    std::cout << "obj1 仍然可以被析构，但值已改变" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    TrackedClass::resetCount();
    
    demo_move_vs_copy();
    demo_move_in_array();
    demo_move_advantages();
    demo_move_implementation();
    demo_move_precautions();
    
    std::cout << "\n=== 总结 ===" << endl;
    std::cout << "移动语义 vs 复制语义：" << std::endl;
    std::cout << "1. 移动语义不是复制，而是转移所有权" << std::endl;
    std::cout << "2. 移动避免了不必要的深拷贝" << std::endl;
    std::cout << "3. 移动后源对象处于有效但未定义的状态" << std::endl;
    std::cout << "4. 移动语义提高了性能" << std::endl;
    std::cout << "5. 在数组操作中，移动语义避免了大量复制" << std::endl;
    
    return 0;
}
















