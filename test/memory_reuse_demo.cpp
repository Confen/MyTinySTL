#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <windows.h>

// 演示类：模拟资源密集型对象
class ResourceIntensiveClass {
private:
    int value_;
    std::vector<int> data_;
    static int count_;
    
public:
    ResourceIntensiveClass(int v, size_t size) : value_(v) {
        ++count_;
        std::cout << "构造 ResourceIntensiveClass(" << value_ << "), 大小: " << size << ", 总数: " << count_ << std::endl;
        
        // 分配大量内存
        data_.resize(size);
        for (size_t i = 0; i < size; ++i) {
            data_[i] = static_cast<int>(i);
        }
        
        // 模拟构造失败
        if (count_ == 3) {
            std::cout << "  构造失败，抛出异常！" << std::endl;
            throw std::runtime_error("资源分配失败");
        }
    }
    
    ~ResourceIntensiveClass() {
        --count_;
        std::cout << "析构 ResourceIntensiveClass(" << value_ << "), 剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    size_t getDataSize() const { return data_.size(); }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int ResourceIntensiveClass::count_ = 0;

// MyTinySTL 风格的 construct 和 destroy
template<typename T, typename... Args>
void construct(T* ptr, Args&&... args) {
    if (ptr != nullptr) {
        new(ptr) T(std::forward<Args>(args)...);
    }
}

template<typename T>
void destroy(T* ptr) {
    if (ptr != nullptr) {
        ptr->~T();
    }
}

template<typename ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) {
    for (; first != last; ++first) {
        destroy(&*first);
    }
}

// 演示1：内存复用的基本概念
void demo_memory_reuse_basic() {
    std::cout << "=== 演示1：内存复用的基本概念 ===" << std::endl;
    
    // 预分配一块内存
    char buffer[sizeof(ResourceIntensiveClass)];
    ResourceIntensiveClass* obj = reinterpret_cast<ResourceIntensiveClass*>(buffer);
    
    ResourceIntensiveClass::resetCount();
    
    // 第一次使用
    std::cout << "第一次使用内存..." << std::endl;
    try {
        construct(obj, 1, 1000);
        std::cout << "第一次构造成功，值: " << obj->getValue() << std::endl;
        destroy(obj);
        std::cout << "第一次析构完成，内存仍然有效" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "第一次构造失败: " << e.what() << std::endl;
    }
    
    // 第二次使用同一块内存
    std::cout << "\n第二次使用同一块内存..." << std::endl;
    try {
        construct(obj, 2, 2000);
        std::cout << "第二次构造成功，值: " << obj->getValue() << std::endl;
        destroy(obj);
        std::cout << "第二次析构完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "第二次构造失败: " << e.what() << std::endl;
    }
    
    std::cout << "内存复用完成，没有额外的内存分配" << std::endl;
}

// 演示2：容器中的内存复用
void demo_container_memory_reuse() {
    std::cout << "\n=== 演示2：容器中的内存复用 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(ResourceIntensiveClass) * capacity];
    ResourceIntensiveClass* objects = reinterpret_cast<ResourceIntensiveClass*>(buffer);
    
    ResourceIntensiveClass::resetCount();
    
    std::cout << "预分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    // 模拟容器操作
    size_t current_size = 0;
    
    try {
        // 添加对象
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "添加第 " << (i + 1) << " 个对象..." << std::endl;
            construct(objects + i, static_cast<int>(i + 1), 500); //到这一步异常，所以从这中断，所以current_size不会增加
            ++current_size;
        }
        std::cout << "所有对象添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "添加失败: " << e.what() << std::endl;
        std::cout << "已添加的对象数量: " << current_size << std::endl;
    }
    
    // 清理所有对象
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    std::cout << "清理完成，内存可以重新使用" << std::endl;
    std::cout << "最终对象数量: " << ResourceIntensiveClass::getCount() << std::endl;
}

// 演示3：内存池的优势
void demo_memory_pool_advantage() {
    std::cout << "\n=== 演示3：内存池的优势 ===" << std::endl;
    
    // 内存池类
    class MemoryPool {
    private:
        char* buffer_;
        size_t capacity_;
        size_t size_;
        ResourceIntensiveClass* objects_;
        
    public:
        MemoryPool(size_t cap) : capacity_(cap), size_(0) {
            buffer_ = new char[sizeof(ResourceIntensiveClass) * capacity_];
            objects_ = reinterpret_cast<ResourceIntensiveClass*>(buffer_);
            std::cout << "内存池构造，容量: " << capacity_ << std::endl;
        }
        
        ~MemoryPool() {
            // 清理所有对象
            for (size_t i = 0; i < size_; ++i) {
                destroy(objects_ + i);
            }
            // 释放内存池
            delete[] buffer_;
            std::cout << "内存池析构" << std::endl;
        }
        
        bool addObject(int value, size_t data_size) {
            if (size_ >= capacity_) {
                return false;
            }
            
            try {
                construct(objects_ + size_, value, data_size);
                ++size_;
                std::cout << "添加对象成功，当前大小: " << size_ << std::endl;
                return true;
            } catch (const std::exception& e) {
                std::cout << "添加对象失败: " << e.what() << std::endl;
                return false;
            }
        }
        
        void removeObject(size_t index) {
            if (index < size_) {
                destroy(objects_ + index);
                // 移动后面的对象
                for (size_t i = index; i < size_ - 1; ++i) {
                    objects_[i] = objects_[i + 1];
                }
                --size_;
                std::cout << "移除对象成功，当前大小: " << size_ << std::endl;
            }
        }
        
        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }
    };
    
    ResourceIntensiveClass::resetCount();
    
    MemoryPool pool(5);
    
    // 添加一些对象
    pool.addObject(1, 100);
    pool.addObject(2, 200);
    pool.addObject(3, 300);  // 这个会失败
    
    std::cout << "当前池大小: " << pool.size() << std::endl;
    
    // 移除一个对象
    pool.removeObject(0);
    std::cout << "移除后池大小: " << pool.size() << std::endl;
    
    // 添加新对象到空出的位置
    pool.addObject(4, 400);
    std::cout << "添加新对象后池大小: " << pool.size() << std::endl;
}

// 演示4：性能对比
void demo_performance_comparison() {
    std::cout << "\n=== 演示4：性能对比 ===" << std::endl;
    
    const int iterations = 1000;
    
    // 方式1：每次 new/delete
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        try {
            ResourceIntensiveClass* obj = new ResourceIntensiveClass(i, 100);
            delete obj;
        } catch (const std::exception&) {
            // 忽略异常
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 方式2：内存复用
    char buffer[sizeof(ResourceIntensiveClass)];
    ResourceIntensiveClass* obj = reinterpret_cast<ResourceIntensiveClass*>(buffer);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        try {
            construct(obj, i, 100);
            destroy(obj);
        } catch (const std::exception&) {
            // 忽略异常
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "new/delete 方式耗时: " << duration1.count() << " 微秒" << std::endl;
    std::cout << "内存复用方式耗时: " << duration2.count() << " 微秒" << std::endl;
    std::cout << "性能提升: " << (double)duration1.count() / duration2.count() << " 倍" << std::endl;
}

// 演示5：内存使用量对比
void demo_memory_usage_comparison() {
    std::cout << "\n=== 演示5：内存使用量对比 ===" << std::endl;
    
    const size_t object_count = 1000;
    const size_t data_size = 10000;
    
    // 方式1：动态分配
    std::cout << "方式1：动态分配" << std::endl;
    std::vector<std::unique_ptr<ResourceIntensiveClass>> dynamic_objects;
    
    for (size_t i = 0; i < object_count; ++i) {
        try {
            dynamic_objects.push_back(
                std::unique_ptr<ResourceIntensiveClass>(
                    new ResourceIntensiveClass(static_cast<int>(i), data_size)
                )
            );
        } catch (const std::exception&) {
            // 忽略异常
        }
    }
    
    std::cout << "动态分配创建了 " << dynamic_objects.size() << " 个对象" << std::endl;
    
    // 清理
    dynamic_objects.clear();
    
    // 方式2：内存池
    std::cout << "\n方式2：内存池" << std::endl;
    char pool_buffer[sizeof(ResourceIntensiveClass) * object_count];
    ResourceIntensiveClass* pool_objects = reinterpret_cast<ResourceIntensiveClass*>(pool_buffer);
    
    size_t pool_count = 0;
    for (size_t i = 0; i < object_count; ++i) {
        try {
            construct(pool_objects + i, static_cast<int>(i), data_size);
            ++pool_count;
        } catch (const std::exception&) {
            // 忽略异常
        }
    }
    
    std::cout << "内存池创建了 " << pool_count << " 个对象" << std::endl;
    
    // 清理
    for (size_t i = 0; i < pool_count; ++i) {
        destroy(pool_objects + i);
    }
    
    std::cout << "内存池方式的内存使用更加可预测和高效" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ResourceIntensiveClass::resetCount();
    
    demo_memory_reuse_basic();
    demo_container_memory_reuse();
    demo_memory_pool_advantage();
    demo_performance_comparison();
    demo_memory_usage_comparison();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "强异常保证与内存复用的关系：" << std::endl;
    std::cout << "1. 内存复用：同一块内存可以多次使用，减少内存分配/释放" << std::endl;
    std::cout << "2. 性能提升：避免频繁的系统调用，提高程序性能" << std::endl;
    std::cout << "3. 内存效率：预分配内存，减少内存碎片" << std::endl;
    std::cout << "4. 异常安全：即使操作失败，内存仍然可以重新使用" << std::endl;
    std::cout << "5. 资源管理：更好的资源控制和内存使用预测" << std::endl;
    
    return 0;
}
















