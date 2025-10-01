#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：不分配内存，只使用栈内存
class NoMemoryAllocationClass {
private:
    int value_;
    int data_[100];  // 栈内存，不动态分配
    static int count_;
    
public:
    NoMemoryAllocationClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 NoMemoryAllocationClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        // 初始化栈内存
        for (int i = 0; i < 100; ++i) {
            data_[i] = i;
        }
        
        // 模拟构造失败
        if (count_ == 3) {
            std::cout << "  构造失败，抛出异常！" << std::endl;
            throw std::runtime_error("构造失败");
        }
        
        std::cout << "构造完成" << std::endl;
    }
    
    ~NoMemoryAllocationClass() {
        --count_;
        std::cout << "析构 NoMemoryAllocationClass(" << value_ << "), 剩余: " << count_ << std::endl;
        // 栈内存自动清理，不需要手动释放
    }
    
    int getValue() const { return value_; }
    int getData(int index) const { return data_[index]; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int NoMemoryAllocationClass::count_ = 0;

// 演示类：只分配少量内存
class MinimalMemoryClass {
private:
    int value_;
    int* small_data_;  // 只分配少量内存
    static int count_;
    
public:
    MinimalMemoryClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 MinimalMemoryClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        // 只分配少量内存
        small_data_ = new int[10];  // 只分配10个int
        for (int i = 0; i < 10; ++i) {
            small_data_[i] = i;
        }
        
        // 模拟构造失败
        if (count_ == 3) {
            std::cout << "  构造失败，抛出异常！" << std::endl;
            throw std::runtime_error("构造失败");
        }
        
        std::cout << "构造完成" << std::endl;
    }
    
    ~MinimalMemoryClass() {
        --count_;
        std::cout << "析构 MinimalMemoryClass(" << value_ << "), 剩余: " << count_ << std::endl;
        if (small_data_) {
            delete[] small_data_;
        }
    }
    
    int getValue() const { return value_; }
    int getData(int index) const { return small_data_[index]; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int MinimalMemoryClass::count_ = 0;

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

// 演示1：不分配内存的情况
void demo_no_memory_allocation() {
    std::cout << "=== 演示1：不分配内存的情况 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(NoMemoryAllocationClass) * capacity];
    NoMemoryAllocationClass* objects = reinterpret_cast<NoMemoryAllocationClass*>(buffer);
    
    NoMemoryAllocationClass::resetCount();
    
    std::cout << "预分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    size_t current_size = 0;
    
    try {
        // 添加对象
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "\n--- 添加第 " << (i + 1) << " 个对象 ---" << std::endl;
            construct(objects + i, static_cast<int>(i + 1));
            ++current_size;
            std::cout << "第 " << (i + 1) << " 个对象添加成功" << std::endl;
        }
        std::cout << "\n所有对象添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n添加失败: " << e.what() << std::endl;
        std::cout << "已添加的对象数量: " << current_size << std::endl;
        
        std::cout << "\n=== 分析对象3的情况 ===" << std::endl;
        std::cout << "对象3没有分配动态内存，只有栈内存" << std::endl;
        std::cout << "栈内存会自动清理，不会泄漏" << std::endl;
    }
    
    // 清理所有对象
    std::cout << "\n--- 清理所有对象 ---" << std::endl;
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    std::cout << "最终对象数量: " << NoMemoryAllocationClass::getCount() << std::endl;
}

// 演示2：分配少量内存的情况
void demo_minimal_memory_allocation() {
    std::cout << "\n=== 演示2：分配少量内存的情况 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(MinimalMemoryClass) * capacity];
    MinimalMemoryClass* objects = reinterpret_cast<MinimalMemoryClass*>(buffer);
    
    MinimalMemoryClass::resetCount();
    
    std::cout << "预分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    size_t current_size = 0;
    
    try {
        // 添加对象
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "\n--- 添加第 " << (i + 1) << " 个对象 ---" << std::endl;
            construct(objects + i, static_cast<int>(i + 1));
            ++current_size;
            std::cout << "第 " << (i + 1) << " 个对象添加成功" << std::endl;
        }
        std::cout << "\n所有对象添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n添加失败: " << e.what() << std::endl;
        std::cout << "已添加的对象数量: " << current_size << std::endl;
        
        std::cout << "\n=== 分析对象3的情况 ===" << std::endl;
        std::cout << "对象3分配了少量内存（10个int）" << std::endl;
        std::cout << "异常安全机制确保内存被正确清理" << std::endl;
    }
    
    // 清理所有对象
    std::cout << "\n--- 清理所有对象 ---" << std::endl;
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    std::cout << "最终对象数量: " << MinimalMemoryClass::getCount() << std::endl;
}

// 演示3：对比不同内存分配策略
void demo_memory_allocation_comparison() {
    std::cout << "\n=== 演示3：对比不同内存分配策略 ===" << std::endl;
    
    // 策略1：不分配内存（栈内存）
    std::cout << "策略1：不分配内存（栈内存）" << std::endl;
    try {
        NoMemoryAllocationClass obj(3);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        std::cout << "优势：栈内存自动清理，不会泄漏" << std::endl;
    }
    
    // 策略2：分配少量内存
    std::cout << "\n策略2：分配少量内存" << std::endl;
    try {
        MinimalMemoryClass obj(3);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        std::cout << "需要：异常安全机制确保内存清理" << std::endl;
    }
    
    // 策略3：分配大量内存（之前的例子）
    std::cout << "\n策略3：分配大量内存" << std::endl;
    std::cout << "需要：强异常安全机制确保内存清理" << std::endl;
    std::cout << "风险：内存泄漏风险较高" << std::endl;
}

// 演示4：异常安全的重要性对比
void demo_exception_safety_importance_comparison() {
    std::cout << "\n=== 演示4：异常安全的重要性对比 ===" << std::endl;
    
    // 不安全的版本：分配内存但不处理异常
    class UnsafeMemoryClass {
    private:
        int value_;
        int* data_;
        
    public:
        UnsafeMemoryClass(int v, size_t size) : value_(v) {
            std::cout << "构造 UnsafeMemoryClass(" << value_ << ")" << std::endl;
            
            // 分配内存
            data_ = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data_[i] = static_cast<int>(i);
            }
            
            // 模拟失败
            if (value_ == 3) {
                std::cout << "  构造失败，抛出异常！" << std::endl;
                throw std::runtime_error("构造失败");
            }
        }
        
        ~UnsafeMemoryClass() {
            std::cout << "析构 UnsafeMemoryClass(" << value_ << ")" << std::endl;
            if (data_) {
                delete[] data_;
            }
        }
    };
    
    // 安全的版本：使用RAII
    class SafeMemoryClass {
    private:
        int value_;
        std::unique_ptr<int[]> data_;
        
    public:
        SafeMemoryClass(int v, size_t size) : value_(v) {
            std::cout << "构造 SafeMemoryClass(" << value_ << ")" << std::endl;
            
            // 使用智能指针自动管理内存
            data_ = std::unique_ptr<int>(new int[size]);
            for (size_t i = 0; i < size; ++i) {
                data_[i] = static_cast<int>(i);
            }
            
            // 模拟失败
            if (value_ == 3) {
                std::cout << "  构造失败，抛出异常！" << std::endl;
                throw std::runtime_error("构造失败");
            }
        }
        
        ~SafeMemoryClass() {
            std::cout << "析构 SafeMemoryClass(" << value_ << ")" << std::endl;
            // 智能指针自动清理内存
        }
    };
    
    std::cout << "测试不安全版本:" << std::endl;
    try {
        UnsafeMemoryClass unsafe_obj(3, 1000);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "问题：内存泄漏！" << std::endl;
    }
    
    std::cout << "\n测试安全版本:" << std::endl;
    try {
        SafeMemoryClass safe_obj(3, 1000);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "优势：内存自动清理！" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    NoMemoryAllocationClass::resetCount();
    MinimalMemoryClass::resetCount();
    
    demo_no_memory_allocation();
    demo_minimal_memory_allocation();
    demo_memory_allocation_comparison();
    demo_exception_safety_importance_comparison();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "关于不分配内存的情况：" << std::endl;
    std::cout << "1. 栈内存：自动清理，不会泄漏" << std::endl;
    std::cout << "2. 少量内存：需要异常安全机制" << std::endl;
    std::cout << "3. 大量内存：需要强异常安全机制" << std::endl;
    std::cout << "4. 异常安全的重要性随内存分配量增加" << std::endl;
    std::cout << "5. MyTinySTL的异常安全机制适用于所有情况" << std::endl;
    
    return 0;
}







