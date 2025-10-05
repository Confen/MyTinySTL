#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：跟踪内存分配
class TrackedClass {
private:
    int value_;
    static int count_;
    
public:
    TrackedClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 TrackedClass(" << value_ << "), 总数: " << count_ << std::endl;
    }
    
    ~TrackedClass() {
        --count_;
        std::cout << "析构 TrackedClass(" << value_ << "), 剩余: " << count_ << std::endl;
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
    T* data_;           // 数据指针
    size_t size_;       // 当前大小
    size_t capacity_;   // 容量
    
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {
        std::cout << "SimpleVector 构造" << std::endl;
    }
    
    ~SimpleVector() {
        std::cout << "SimpleVector 析构，容量: " << capacity_ << std::endl;
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
        std::cout << "添加元素，当前大小: " << size_ << ", 容量: " << capacity_ << std::endl;
    }
    
    void erase(size_t index) {
        if (index >= size_) return;
        
        std::cout << "删除索引 " << index << " 的元素" << std::endl;
        
        // 析构要删除的元素
        data_[index].~T();
        
        // 移动后面的元素
        for (size_t i = index; i < size_ - 1; ++i) {
            new(data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        
        --size_;
        std::cout << "删除完成，当前大小: " << size_ << ", 容量: " << capacity_ << std::endl;
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        
        std::cout << "扩容：从 " << capacity_ << " 到 " << new_capacity << std::endl;
        
        T* new_data = reinterpret_cast<T*>(new char[sizeof(T) * new_capacity]);
        
        // 移动现有元素
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

// 演示1：动态数组的内存复用
void demo_dynamic_array_memory_reuse() {
    std::cout << "=== 演示1：动态数组的内存复用 ===" << std::endl;
    
    SimpleVector<TrackedClass> vec;
    TrackedClass::resetCount();
    
    // 添加一些元素
    std::cout << "\n添加元素：" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(TrackedClass(i));
    }
    
    vec.printStatus();
    std::cout << "对象总数: " << TrackedClass::getCount() << std::endl;
    
    // 删除中间元素
    std::cout << "\n删除中间元素（索引2）：" << std::endl;
    vec.erase(2);
    
    vec.printStatus();
    std::cout << "对象总数: " << TrackedClass::getCount() << std::endl;
    
    // 添加新元素
    std::cout << "\n添加新元素：" << std::endl;
    vec.push_back(TrackedClass(6));
    
    vec.printStatus();
    std::cout << "对象总数: " << TrackedClass::getCount() << std::endl;
    
    std::cout << "\n关键点：删除元素后，内存可以复用！" << std::endl;
}

// 演示2：内存复用的详细过程
void demo_memory_reuse_process() {
    std::cout << "\n=== 演示2：内存复用的详细过程 ===" << std::endl;
    
    SimpleVector<TrackedClass> vec;
    TrackedClass::resetCount();
    
    // 添加元素
    std::cout << "添加元素 1, 2, 3, 4, 5：" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(TrackedClass(i));
    }
    
    vec.printStatus();
    
    // 删除元素2
    std::cout << "\n删除元素2（索引1）：" << std::endl;
    vec.erase(1);
    
    vec.printStatus();
    std::cout << "内存布局：[1] [3] [4] [5] [空闲]" << std::endl;
    
    // 添加新元素
    std::cout << "\n添加新元素6：" << std::endl;
    vec.push_back(TrackedClass(6));
    
    vec.printStatus();
    std::cout << "内存布局：[1] [3] [4] [5] [6]" << std::endl;
    std::cout << "新元素复用了删除元素的内存位置！" << std::endl;
}

// 演示3：与标准库的对比
void demo_standard_library_comparison() {
    std::cout << "\n=== 演示3：与标准库的对比 ===" << std::endl;
    
    TrackedClass::resetCount();
    
    // 标准库 vector
    std::cout << "标准库 vector：" << std::endl;
    std::vector<TrackedClass> std_vec;
    
    // 添加元素
    for (int i = 1; i <= 5; ++i) {
        std_vec.push_back(TrackedClass(i));
    }
    
    std::cout << "添加后：大小 " << std_vec.size() << ", 容量 " << std_vec.capacity() << std::endl;
    
    // 删除元素
    std_vec.erase(std_vec.begin() + 2);
    std::cout << "删除后：大小 " << std_vec.size() << ", 容量 " << std_vec.capacity() << std::endl;
    
    // 添加新元素
    std_vec.push_back(TrackedClass(6));
    std::cout << "添加后：大小 " << std_vec.size() << ", 容量 " << std_vec.capacity() << std::endl;
    
    std::cout << "标准库也支持内存复用！" << std::endl;
}

// 演示4：内存复用的优势
void demo_memory_reuse_advantages() {
    std::cout << "\n=== 演示4：内存复用的优势 ===" << std::endl;
    
    const int iterations = 1000;
    
    // 方式1：每次重新分配
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<int> vec1;
    for (int i = 0; i < iterations; ++i) {
        vec1.push_back(i);
        if (i % 2 == 0) {
            vec1.erase(vec1.begin());
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 方式2：内存复用
    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<int> vec2;
    for (int i = 0; i < iterations; ++i) {
        vec2.push_back(i);
        if (i % 2 == 0) {
            vec2.erase(vec2.begin());
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "内存复用方式耗时: " << duration1.count() << " 微秒" << std::endl;
    std::cout << "内存复用方式耗时: " << duration2.count() << " 微秒" << std::endl;
    std::cout << "内存复用避免了频繁的内存分配/释放" << std::endl;
}

// 演示5：内存复用的局限性
void demo_memory_reuse_limitations() {
    std::cout << "\n=== 演示5：内存复用的局限性 ===" << std::endl;
    
    SimpleVector<TrackedClass> vec;
    TrackedClass::resetCount();
    
    // 添加元素
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(TrackedClass(i));
    }
    
    std::cout << "初始状态：" << std::endl;
    vec.printStatus();
    
    // 删除所有元素
    std::cout << "\n删除所有元素：" << std::endl;
    while (vec.size() > 0) {
        vec.erase(0);
    }
    
    std::cout << "删除后：" << std::endl;
    vec.printStatus();
    std::cout << "容量仍然保持，内存可以复用" << std::endl;
    
    // 重新添加元素
    std::cout << "\n重新添加元素：" << std::endl;
    for (int i = 6; i <= 10; ++i) {
        vec.push_back(TrackedClass(i));
    }
    
    vec.printStatus();
    std::cout << "新元素复用了之前的内存！" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    TrackedClass::resetCount();
    
    demo_dynamic_array_memory_reuse();
    demo_memory_reuse_process();
    demo_standard_library_comparison();
    demo_memory_reuse_advantages();
    demo_memory_reuse_limitations();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "动态数组的内存复用：" << std::endl;
    std::cout << "1. 删除元素后，内存可以复用" << std::endl;
    std::cout << "2. 新元素可以复用到删除元素的位置" << std::endl;
    std::cout << "3. 避免了频繁的内存分配/释放" << std::endl;
    std::cout << "4. 提高了性能和内存利用率" << std::endl;
    std::cout << "5. 这是动态数组的重要优势" << std::endl;
    
    return 0;
}

















