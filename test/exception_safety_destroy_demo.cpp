#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：会在构造时可能抛出异常
class ResourceClass {
private:
    int value_;
    std::string* resource_;
    static int count_;
    
public:
    ResourceClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 ResourceClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        // 模拟资源分配
        resource_ = new std::string("Resource_" + std::to_string(value_));
        
        // 在第3个对象构造时抛出异常
        if (count_ == 3) {
            std::cout << "  抛出异常！" << std::endl;
            throw std::runtime_error("资源分配失败");
        }
    }
    
    ~ResourceClass() {
        --count_;
        std::cout << "析构 ResourceClass(" << value_ << "), 剩余: " << count_ << std::endl;
        delete resource_;
    }
    
    int getValue() const { return value_; }
    const std::string& getResource() const { return *resource_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int ResourceClass::count_ = 0;

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

// 演示1：异常安全的基本原理
void demo_exception_safety_basic() {
    std::cout << "=== 演示1：异常安全的基本原理 ===" << std::endl;
    
    // 分配原始内存
    char buffer[sizeof(ResourceClass) * 5];
    ResourceClass* objects = reinterpret_cast<ResourceClass*>(buffer);
    
    ResourceClass::resetCount();
    
    try {
        std::cout << "尝试构造5个对象，第3个会抛出异常" << std::endl;
        
        // 逐个构造对象
        for (int i = 0; i < 5; ++i) {
            std::cout << "  构造第 " << (i + 1) << " 个对象..." << std::endl;
            construct(objects + i, i + 1);
        }
        
        std::cout << "所有对象构造成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "已构造的对象数量: " << ResourceClass::getCount() << std::endl;
        
        // 关键：析构已构造的对象但不释放内存
        std::cout << "开始清理已构造的对象..." << std::endl;
        for (int i = 0; i < ResourceClass::getCount(); ++i) {
            destroy(objects + i);
        }
        std::cout << "清理完成，内存仍然有效" << std::endl;
    }
    
    std::cout << "最终对象数量: " << ResourceClass::getCount() << std::endl;
}

// 演示2：容器扩容中的异常安全
void demo_container_expansion() {
    std::cout << "\n=== 演示2：容器扩容中的异常安全 ===" << std::endl;
    
    // 模拟容器扩容
    const size_t old_capacity = 3;
    const size_t new_capacity = 6;
    
    // 旧容器
    char old_buffer[sizeof(ResourceClass) * old_capacity];
    ResourceClass* old_objects = reinterpret_cast<ResourceClass*>(old_buffer);
    
    // 新容器
    char new_buffer[sizeof(ResourceClass) * new_capacity];
    ResourceClass* new_objects = reinterpret_cast<ResourceClass*>(new_buffer);
    
    ResourceClass::resetCount();
    
    // 构造旧容器中的对象
    for (size_t i = 0; i < old_capacity; ++i) {
        construct(old_objects + i, static_cast<int>(i + 1));
    }
    std::cout << "旧容器构造完成，对象数量: " << ResourceClass::getCount() << std::endl;
    
    try {
        std::cout << "开始扩容操作..." << std::endl;
        
        // 拷贝旧对象到新容器
        for (size_t i = 0; i < old_capacity; ++i) {
            construct(new_objects + i, old_objects[i].getValue());
        }
        std::cout << "旧对象拷贝完成" << std::endl;
        
        // 尝试构造新对象（会抛出异常）
        for (size_t i = old_capacity; i < new_capacity; ++i) {
            construct(new_objects + i, static_cast<int>(i + 1));
        }
        
        std::cout << "扩容成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "扩容失败: " << e.what() << std::endl;
        std::cout << "已构造的对象数量: " << ResourceClass::getCount() << std::endl;
        
        // 异常安全：清理新容器中已构造的对象
        std::cout << "清理新容器中已构造的对象..." << std::endl;
        for (size_t i = 0; i < ResourceClass::getCount() - old_capacity; ++i) {
            destroy(new_objects + i);
        }
        
        std::cout << "扩容失败，但旧容器状态不变" << std::endl;
    }
    
    // 清理旧容器
    for (size_t i = 0; i < old_capacity; ++i) {
        destroy(old_objects + i);
    }
    
    std::cout << "最终对象数量: " << ResourceClass::getCount() << std::endl;
}

// 演示3：RAII 与异常安全的结合
void demo_raii_exception_safety() {
    std::cout << "\n=== 演示3：RAII 与异常安全的结合 ===" << std::endl;
    
    // RAII 资源管理器
    class ResourceManager {
    private:
        char* buffer_;
        size_t capacity_;
        size_t size_;
        ResourceClass* objects_;
        
    public:
        ResourceManager(size_t cap) : capacity_(cap), size_(0) {
            buffer_ = new char[sizeof(ResourceClass) * capacity_];
            objects_ = reinterpret_cast<ResourceClass*>(buffer_);
            std::cout << "ResourceManager 构造，容量: " << capacity_ << std::endl;
        }
        
        ~ResourceManager() {
            // 析构所有对象
            for (size_t i = 0; i < size_; ++i) {
                destroy(objects_ + i);
            }
            // 释放内存
            delete[] buffer_;
            std::cout << "ResourceManager 析构" << std::endl;
        }
        
        void addResource(int value) {
            if (size_ >= capacity_) {
                throw std::runtime_error("容量不足");
            }
            
            try {
                construct(objects_ + size_, value);
                ++size_;
                std::cout << "添加资源成功，当前大小: " << size_ << std::endl;
            } catch (...) {
                // 如果构造失败，不需要清理，因为 size_ 没有增加
                std::cout << "添加资源失败" << std::endl;
                throw;
            }
        }
        
        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }
    };
    
    ResourceClass::resetCount();
    
    try {
        ResourceManager manager(5);
        
        // 添加一些资源
        manager.addResource(1);
        manager.addResource(2);
        manager.addResource(3);  // 这个会抛出异常
        
        std::cout << "所有资源添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "RAII 确保资源被正确清理" << std::endl;
    }
    
    std::cout << "最终对象数量: " << ResourceClass::getCount() << std::endl;
}

// 演示4：异常安全的不同级别
void demo_exception_safety_levels() {
    std::cout << "\n=== 演示4：异常安全的不同级别 ===" << std::endl;
    
    // 无异常保证的版本
    class NoGuaranteeClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            // 直接添加，如果中途失败，对象状态不确定
            for (int elem : elements) {
                data_.push_back(elem);
                if (elem > 100) {
                    throw std::runtime_error("元素过大");
                }
            }
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    // 基本异常保证的版本
    class BasicGuaranteeClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            // 基本保证：对象始终处于有效状态
            for (int elem : elements) {
                if (elem > 100) {
                    throw std::runtime_error("元素过大");
                }
                data_.push_back(elem);
            }
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    // 强异常保证的版本
    class StrongGuaranteeClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            // 强保证：操作要么完全成功，要么完全失败
            std::vector<int> temp = data_;
            
            for (int elem : elements) {
                if (elem > 100) {
                    throw std::runtime_error("元素过大");
                }
                temp.push_back(elem);
            }
            
            data_ = std::move(temp);  // 只有全部成功才更新
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    std::vector<int> test_data = {1, 2, 150, 4};  // 第3个元素会抛出异常
    
    // 测试无异常保证
    std::cout << "测试无异常保证版本:" << std::endl;
    try {
        NoGuaranteeClass obj;
        obj.addElements(test_data);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        std::cout << "对象状态不确定" << std::endl;
    }
    
    // 测试基本异常保证
    std::cout << "\n测试基本异常保证版本:" << std::endl;
    try {
        BasicGuaranteeClass obj;
        obj.addElements(test_data);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        std::cout << "对象处于有效状态" << std::endl;
    }
    
    // 测试强异常保证
    std::cout << "\n测试强异常保证版本:" << std::endl;
    try {
        StrongGuaranteeClass obj;
        obj.addElements(test_data);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        std::cout << "对象状态完全不变" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ResourceClass::resetCount();
    
    demo_exception_safety_basic();
    demo_container_expansion();
    demo_raii_exception_safety();
    demo_exception_safety_levels();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "析构对象但不释放内存用于异常处理的关键点：" << std::endl;
    std::cout << "1. 实现强异常保证：操作失败时对象状态完全不变" << std::endl;
    std::cout << "2. 资源管理：确保已分配的资源被正确清理" << std::endl;
    std::cout << "3. 内存复用：同一块内存可以重新使用" << std::endl;
    std::cout << "4. 容器安全：容器操作失败时不会破坏原有数据" << std::endl;
    std::cout << "5. RAII 结合：与资源管理结合，确保异常安全" << std::endl;
    
    return 0;
}







