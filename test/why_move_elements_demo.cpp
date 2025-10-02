#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：显示内存地址
class AddressClass {
private:
    int value_;
    
public:
    AddressClass(int v) : value_(v) {
        std::cout << "构造 AddressClass(" << value_ << "), 地址: " << this << std::endl;
    }
    
    ~AddressClass() {
        std::cout << "析构 AddressClass(" << value_ << "), 地址: " << this << std::endl;
    }
    
    int getValue() const { return value_; }
    void* getAddress() const { return const_cast<void*>(static_cast<const void*>(this)); }
};

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
    
    // 方式1：不移动元素（错误方式）
    void erase_no_move(size_t index) {
        if (index >= size_) return;
        
        std::cout << "\n=== 方式1：不移动元素（错误方式） ===" << std::endl;
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 只析构要删除的元素
        data_[index].~T();
        
        // 不移动后面的元素
        --size_;
        
        std::cout << "删除后的大小: " << size_ << std::endl;
        std::cout << "问题：数组中有空隙！" << std::endl;
    }
    
    // 方式2：移动元素（正确方式）
    void erase_with_move(size_t index) {
        if (index >= size_) return;
        
        std::cout << "\n=== 方式2：移动元素（正确方式） ===" << std::endl;
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 析构要删除的元素
        data_[index].~T();
        
        // 移动后面的元素
        for (size_t i = index; i < size_ - 1; ++i) {
            std::cout << "移动元素 " << (i + 1) << " 到位置 " << i << std::endl;
            new(data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        
        --size_;
        std::cout << "删除后的大小: " << size_ << std::endl;
        std::cout << "结果：数组保持连续！" << std::endl;
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
    
    void printAddresses() {
        std::cout << "内存地址: ";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data_[i].getAddress() << " ";
        }
        std::cout << std::endl;
    }
};

// 演示1：为什么需要移动元素
void demo_why_move_elements() {
    std::cout << "=== 演示1：为什么需要移动元素 ===" << std::endl;
    
    SimpleVector<AddressClass> vec;
    
    // 添加元素
    std::cout << "\n添加元素：" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(AddressClass(i));
    }
    
    vec.printStatus();
    vec.printAddresses();
    
    // 删除中间元素（不移动）
    std::cout << "\n删除中间元素（不移动）：" << std::endl;
    vec.erase_no_move(2);
    
    vec.printStatus();
    vec.printAddresses();
    
    std::cout << "\n问题分析：" << std::endl;
    std::cout << "1. 数组中有空隙（索引2位置）" << std::endl;
    std::cout << "2. 破坏了数组的连续性" << std::endl;
    std::cout << "3. 访问索引2会出错" << std::endl;
    std::cout << "4. 内存利用率降低" << std::endl;
}

// 演示2：移动元素的正确方式
void demo_correct_move_elements() {
    std::cout << "\n=== 演示2：移动元素的正确方式 ===" << std::endl;
    
    SimpleVector<AddressClass> vec;
    
    // 添加元素
    std::cout << "\n添加元素：" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(AddressClass(i));
    }
    
    vec.printStatus();
    vec.printAddresses();
    
    // 删除中间元素（移动）
    std::cout << "\n删除中间元素（移动）：" << std::endl;
    vec.erase_with_move(2);
    
    vec.printStatus();
    vec.printAddresses();
    
    std::cout << "\n优势分析：" << std::endl;
    std::cout << "1. 数组保持连续" << std::endl;
    std::cout << "2. 所有索引都有效" << std::endl;
    std::cout << "3. 内存利用率高" << std::endl;
    std::cout << "4. 符合数组的语义" << std::endl;
}

// 演示3：移动元素的性能考虑
void demo_move_performance() {
    std::cout << "\n=== 演示3：移动元素的性能考虑 ===" << std::endl;
    
    const int iterations = 1000;
    
    // 方式1：移动元素
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<int> vec1;
    for (int i = 0; i < iterations; ++i) {
        vec1.push_back(i);
    }
    for (int i = 0; i < iterations / 2; ++i) {
        vec1.erase(vec1.begin());
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 方式2：不移动元素（模拟）
    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<int> vec2;
    for (int i = 0; i < iterations; ++i) {
        vec2.push_back(i);
    }
    // 模拟不移动的情况（实际上不可能）
    vec2.resize(iterations / 2);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "移动元素方式耗时: " << duration1.count() << " 微秒" << std::endl;
    std::cout << "不移动元素方式耗时: " << duration2.count() << " 微秒" << std::endl;
    std::cout << "移动元素确实有性能开销，但保证了数组的连续性" << std::endl;
}

// 演示4：不同删除策略的对比
void demo_deletion_strategies() {
    std::cout << "\n=== 演示4：不同删除策略的对比 ===" << std::endl;
    
    std::cout << "策略1：移动元素（std::vector）" << std::endl;
    std::cout << "  优点：保持连续性，索引有效" << std::endl;
    std::cout << "  缺点：移动开销大" << std::endl;
    
    std::cout << "\n策略2：标记删除（std::list）" << std::endl;
    std::cout << "  优点：删除速度快" << std::endl;
    std::cout << "  缺点：内存碎片，需要额外标记" << std::endl;
    
    std::cout << "\n策略3：延迟删除（std::deque）" << std::endl;
    std::cout << "  优点：平衡性能和连续性" << std::endl;
    std::cout << "  缺点：实现复杂" << std::endl;
}

// 演示5：移动元素的具体实现
void demo_move_implementation() {
    std::cout << "\n=== 演示5：移动元素的具体实现 ===" << std::endl;
    
    std::cout << "移动元素的关键步骤：" << std::endl;
    std::cout << "1. 析构要删除的元素" << std::endl;
    std::cout << "2. 移动后面的元素到前面" << std::endl;
    std::cout << "3. 析构移动后的原位置元素" << std::endl;
    std::cout << "4. 减少数组大小" << std::endl;
    
    std::cout << "\n代码实现：" << std::endl;
    std::cout << "for (size_t i = index; i < size_ - 1; ++i) {" << std::endl;
    std::cout << "    new(data_ + i) T(std::move(data_[i + 1]));" << std::endl;
    std::cout << "    data_[i + 1].~T();" << std::endl;
    std::cout << "}" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demo_why_move_elements();
    demo_correct_move_elements();
    demo_move_performance();
    demo_deletion_strategies();
    demo_move_implementation();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "为什么需要移动元素：" << std::endl;
    std::cout << "1. 保持数组连续性：避免空隙" << std::endl;
    std::cout << "2. 保证索引有效性：所有索引都有效" << std::endl;
    std::cout << "3. 符合数组语义：数组应该是连续的" << std::endl;
    std::cout << "4. 内存效率：充分利用内存空间" << std::endl;
    std::cout << "5. 访问安全：避免访问无效位置" << std::endl;
    
    return 0;
}








