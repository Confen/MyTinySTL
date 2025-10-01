#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：跟踪资源管理
class ResourceClass {
private:
    int* data_;
    int size_;
    static int count_;
    
public:
    ResourceClass(int size) : size_(size) {
        ++count_;
        data_ = new int[size];
        std::cout << "构造 ResourceClass，大小: " << size << ", 地址: " << this << ", 数据地址: " << data_ << ", 总数: " << count_ << std::endl;
        for (int i = 0; i < size; ++i) {
            data_[i] = i;
        }
    }
    
    // 拷贝构造函数
    ResourceClass(const ResourceClass& other) : size_(other.size_) {
        ++count_;
        data_ = new int[size_];
        std::cout << "拷贝构造 ResourceClass，地址: " << this << ", 数据地址: " << data_ << ", 总数: " << count_ << std::endl;
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    // 移动构造函数
    ResourceClass(ResourceClass&& other) noexcept : size_(other.size_), data_(other.data_) {
        ++count_;
        std::cout << "移动构造 ResourceClass，地址: " << this << ", 数据地址: " << data_ << ", 总数: " << count_ << std::endl;
        other.data_ = nullptr;  // 转移所有权
        other.size_ = 0;
    }
    
    ~ResourceClass() {
        --count_;
        std::cout << "析构 ResourceClass，地址: " << this << ", 数据地址: " << data_ << ", 剩余: " << count_ << std::endl;
        if (data_) {
            delete[] data_;
            std::cout << "  释放数据内存: " << data_ << std::endl;
        }
    }
    
    int getSize() const { return size_; }
    int* getData() const { return data_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int ResourceClass::count_ = 0;

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
    
    // 方式1：不析构原位置（错误方式）
    void erase_no_destructor(size_t index) {
        if (index >= size_) return;
        
        std::cout << "\n=== 方式1：不析构原位置（错误方式） ===" << std::endl;
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 只析构要删除的元素
        data_[index].~T();
        
        // 移动后面的元素
        for (size_t i = index; i < size_ - 1; ++i) {
            std::cout << "  移动元素 " << (i + 1) << " 到位置 " << i << std::endl;
            
            // 在位置i构造新对象（移动构造）
            new(data_ + i) T(std::move(data_[i + 1]));
            
            // 不析构原位置的对象！
            std::cout << "    错误：不析构原位置 " << (i + 1) << " 的对象" << std::endl;
        }
        
        --size_;
        std::cout << "删除完成，当前大小: " << size_ << std::endl;
        std::cout << "问题：原位置的对象没有被析构！" << std::endl;
    }
    
    // 方式2：析构原位置（正确方式）
    void erase_with_destructor(size_t index) {
        if (index >= size_) return;
        
        std::cout << "\n=== 方式2：析构原位置（正确方式） ===" << std::endl;
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 析构要删除的元素
        data_[index].~T();
        
        // 移动后面的元素
        for (size_t i = index; i < size_ - 1; ++i) {
            std::cout << "  移动元素 " << (i + 1) << " 到位置 " << i << std::endl;
            
            // 在位置i构造新对象（移动构造）
            new(data_ + i) T(std::move(data_[i + 1]));
            
            // 析构原位置的对象
            std::cout << "    正确：析构原位置 " << (i + 1) << " 的对象" << std::endl;
            data_[i + 1].~T();
        }
        
        --size_;
        std::cout << "删除完成，当前大小: " << size_ << std::endl;
        std::cout << "结果：原位置的对象被正确析构！" << std::endl;
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
            std::cout << data_[i].getSize() << " ";
        }
        std::cout << std::endl;
    }
};

// 演示1：为什么需要析构原位置
void demo_why_destructor_original_position() {
    std::cout << "=== 演示1：为什么需要析构原位置 ===" << std::endl;
    
    SimpleVector<ResourceClass> vec;
    ResourceClass::resetCount();
    
    // 添加元素
    std::cout << "添加元素：" << std::endl;
    for (int i = 1; i <= 3; ++i) {
        vec.push_back(ResourceClass(i * 10));
    }
    
    vec.printStatus();
    std::cout << "对象总数: " << ResourceClass::getCount() << std::endl;
    
    // 删除中间元素（不析构原位置）
    std::cout << "\n删除中间元素（不析构原位置）：" << std::endl;
    vec.erase_no_destructor(1);
    
    vec.printStatus();
    std::cout << "对象总数: " << ResourceClass::getCount() << std::endl;
    
    std::cout << "\n问题分析：" << std::endl;
    std::cout << "1. 原位置的对象没有被析构" << std::endl;
    std::cout << "2. 导致内存泄漏" << std::endl;
    std::cout << "3. 对象计数不正确" << std::endl;
    std::cout << "4. 资源没有被释放" << std::endl;
}

// 演示2：正确析构原位置
void demo_correct_destructor_original_position() {
    std::cout << "\n=== 演示2：正确析构原位置 ===" << std::endl;
    
    SimpleVector<ResourceClass> vec;
    ResourceClass::resetCount();
    
    // 添加元素
    std::cout << "添加元素：" << std::endl;
    for (int i = 1; i <= 3; ++i) {
        vec.push_back(ResourceClass(i * 10));
    }
    
    vec.printStatus();
    std::cout << "对象总数: " << ResourceClass::getCount() << std::endl;
    
    // 删除中间元素（析构原位置）
    std::cout << "\n删除中间元素（析构原位置）：" << std::endl;
    vec.erase_with_destructor(1);
    
    vec.printStatus();
    std::cout << "对象总数: " << ResourceClass::getCount() << std::endl;
    
    std::cout << "\n优势分析：" << std::endl;
    std::cout << "1. 原位置的对象被正确析构" << std::endl;
    std::cout << "2. 没有内存泄漏" << std::endl;
    std::cout << "3. 对象计数正确" << std::endl;
    std::cout << "4. 资源被正确释放" << std::endl;
}

// 演示3：内存状态分析
void demo_memory_state_analysis() {
    std::cout << "\n=== 演示3：内存状态分析 ===" << std::endl;
    
    std::cout << "移动元素的内存状态变化：" << std::endl;
    std::cout << "1. 原始状态：" << std::endl;
    std::cout << "   [对象1] [对象2] [对象3]" << std::endl;
    std::cout << "     0       1       2" << std::endl;
    
    std::cout << "\n2. 删除对象2后：" << std::endl;
    std::cout << "   [对象1] [空隙] [对象3]" << std::endl;
    std::cout << "     0       1       2" << std::endl;
    
    std::cout << "\n3. 移动对象3到位置1：" << std::endl;
    std::cout << "   [对象1] [对象3] [对象3]" << std::endl;
    std::cout << "     0       1       2" << std::endl;
    std::cout << "            ↑新对象  ↑原对象（已移动）" << std::endl;
    
    std::cout << "\n4. 析构原位置的对象3：" << std::endl;
    std::cout << "   [对象1] [对象3] [已析构]" << std::endl;
    std::cout << "     0       1       2" << std::endl;
    std::cout << "            ↑新对象  ↑已析构" << std::endl;
    
    std::cout << "\n关键点：原位置的对象必须被析构！" << std::endl;
}

// 演示4：不析构原位置的后果
void demo_consequences_no_destructor() {
    std::cout << "\n=== 演示4：不析构原位置的后果 ===" << std::endl;
    
    std::cout << "不析构原位置的后果：" << std::endl;
    std::cout << "1. 内存泄漏：原位置的对象没有被释放" << std::endl;
    std::cout << "2. 资源泄漏：原位置对象的资源没有被释放" << std::endl;
    std::cout << "3. 对象计数错误：对象计数不准确" << std::endl;
    std::cout << "4. 未定义行为：原位置对象处于未定义状态" << std::endl;
    std::cout << "5. 程序崩溃：可能导致程序崩溃" << std::endl;
    
    std::cout << "\n示例：" << std::endl;
    std::cout << "ResourceClass obj1(100);  // 分配100个int" << std::endl;
    std::cout << "ResourceClass obj2 = std::move(obj1);  // 移动构造" << std::endl;
    std::cout << "// 如果不析构obj1，100个int的内存不会被释放！" << std::endl;
}

// 演示5：析构原位置的正确性
void demo_correctness_destructor_original_position() {
    std::cout << "\n=== 演示5：析构原位置的正确性 ===" << std::endl;
    
    std::cout << "析构原位置的正确性：" << std::endl;
    std::cout << "1. 符合RAII原则：资源获取即初始化" << std::endl;
    std::cout << "2. 符合异常安全：确保资源被正确释放" << std::endl;
    std::cout << "3. 符合内存管理：避免内存泄漏" << std::endl;
    std::cout << "4. 符合对象生命周期：对象被正确销毁" << std::endl;
    std::cout << "5. 符合C++标准：符合C++标准的要求" << std::endl;
    
    std::cout << "\n代码示例：" << std::endl;
    std::cout << "new(data_ + i) T(std::move(data_[i + 1]));" << std::endl;
    std::cout << "// 在位置i构造新对象（移动构造）" << std::endl;
    std::cout << "data_[i + 1].~T();" << std::endl;
    std::cout << "// 析构原位置的对象" << std::endl;
    std::cout << "// 这是必须的！" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ResourceClass::resetCount();
    
    demo_why_destructor_original_position();
    demo_correct_destructor_original_position();
    demo_memory_state_analysis();
    demo_consequences_no_destructor();
    demo_correctness_destructor_original_position();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么原来位置要析构：" << std::endl;
    std::cout << "1. 防止内存泄漏：原位置对象必须被释放" << std::endl;
    std::cout << "2. 防止资源泄漏：原位置对象的资源必须被释放" << std::endl;
    std::cout << "3. 保持对象计数正确：确保对象计数准确" << std::endl;
    std::cout << "4. 符合RAII原则：资源获取即初始化" << std::endl;
    std::cout << "5. 符合异常安全：确保资源被正确释放" << std::endl;
    std::cout << "6. 符合C++标准：符合C++标准的要求" << std::endl;
    
    return 0;
}







