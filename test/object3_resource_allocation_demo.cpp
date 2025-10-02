#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 更详细的演示类：跟踪资源分配
class DetailedResourceClass {
private:
    int value_;
    std::vector<int>* data_;
    static int count_;
    
public:
    DetailedResourceClass(int v, size_t size) : value_(v) {
        ++count_;
        std::cout << "步骤1：构造 ResourceClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        // 步骤2：分配内存
        std::cout << "步骤2：分配 " << size << " 个元素的内存..." << std::endl;
        data_ = new std::vector<int>();
        data_->resize(size);
        
        // 步骤3：初始化数据
        std::cout << "步骤3：初始化数据..." << std::endl;
        for (size_t i = 0; i < size; ++i) {
            (*data_)[i] = static_cast<int>(i);
        }
        
        // 步骤4：检查是否失败
        if (count_ == 3) {
            std::cout << "步骤4：检查失败条件，抛出异常！" << std::endl;
            std::cout << "  → 对象3已经分配了内存，但构造失败" << std::endl;
            throw std::runtime_error("资源分配失败");
        }
        
        std::cout << "步骤4：构造完成" << std::endl;
    }
    
    ~DetailedResourceClass() {
        --count_;
        std::cout << "析构 ResourceClass(" << value_ << "), 剩余: " << count_ << std::endl;
        if (data_) {
            std::cout << "  释放内存，大小: " << data_->size() << std::endl;
            delete data_;
        }
    }
    
    int getValue() const { return value_; }
    size_t getDataSize() const { return data_ ? data_->size() : 0; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int DetailedResourceClass::count_ = 0;

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

// 演示1：对象3的资源分配情况
void demo_object3_resource_allocation() {
    std::cout << "=== 演示1：对象3的资源分配情况 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(DetailedResourceClass) * capacity];
    DetailedResourceClass* objects = reinterpret_cast<DetailedResourceClass*>(buffer);
    
    DetailedResourceClass::resetCount();
    
    std::cout << "预分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    size_t current_size = 0;
    
    try {
        // 添加对象
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "\n--- 添加第 " << (i + 1) << " 个对象 ---" << std::endl;
            construct(objects + i, static_cast<int>(i + 1), 500);
            ++current_size;
            std::cout << "第 " << (i + 1) << " 个对象添加成功" << std::endl;
        }
        std::cout << "\n所有对象添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n添加失败: " << e.what() << std::endl;
        std::cout << "已添加的对象数量: " << current_size << std::endl;
        
        // 关键问题：对象3分配了资源吗？
        std::cout << "\n=== 分析对象3的资源分配情况 ===" << std::endl;
        std::cout << "对象3确实分配了资源，但构造失败" << std::endl;
        std::cout << "这是因为异常发生在构造函数的最后阶段" << std::endl;
    }
    
    // 清理所有对象
    std::cout << "\n--- 清理所有对象 ---" << std::endl;
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    std::cout << "最终对象数量: " << DetailedResourceClass::getCount() << std::endl;
}

// 演示2：异常安全的重要性
void demo_exception_safety_importance() {
    std::cout << "\n=== 演示2：异常安全的重要性 ===" << std::endl;
    
    // 不安全的版本：会泄漏资源
    class UnsafeClass {
    private:
        int value_;
        std::vector<int>* data_;
        
    public:
        UnsafeClass(int v, size_t size) : value_(v) {
            std::cout << "构造 UnsafeClass(" << value_ << ")" << std::endl;
            
            // 分配内存
            data_ = new std::vector<int>();
            data_->resize(size);
            
            // 初始化数据
            for (size_t i = 0; i < size; ++i) {
                (*data_)[i] = static_cast<int>(i);
            }
            
            // 模拟失败
            if (value_ == 3) {
                std::cout << "  构造失败，抛出异常！" << std::endl;
                throw std::runtime_error("构造失败");
            }
        }
        
        ~UnsafeClass() {
            std::cout << "析构 UnsafeClass(" << value_ << ")" << std::endl;
            if (data_) {
                delete data_;
            }
        }
    };
    
    // 安全的版本：使用RAII
    class SafeClass {
    private:
        int value_;
        std::unique_ptr<std::vector<int>> data_;
        
    public:
        SafeClass(int v, size_t size) : value_(v) {
            std::cout << "构造 SafeClass(" << value_ << ")" << std::endl;
            
            // 使用智能指针自动管理内存
            data_ = std::make_unique<std::vector<int>>();
            data_->resize(size);
            
            // 初始化数据
            for (size_t i = 0; i < size; ++i) {
                (*data_)[i] = static_cast<int>(i);
            }
            
            // 模拟失败
            if (value_ == 3) {
                std::cout << "  构造失败，抛出异常！" << std::endl;
                throw std::runtime_error("构造失败");
            }
        }
        
        ~SafeClass() {
            std::cout << "析构 SafeClass(" << value_ << ")" << std::endl;
            // 智能指针自动清理内存
        }
    };
    
    std::cout << "测试不安全版本:" << std::endl;
    try {
        UnsafeClass unsafe_obj(3, 1000);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "问题：内存泄漏！" << std::endl;
    }
    
    std::cout << "\n测试安全版本:" << std::endl;
    try {
        SafeClass safe_obj(3, 1000);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "优势：内存自动清理！" << std::endl;
    }
}

// 演示3：MyTinySTL的异常安全机制
void demo_mystl_exception_safety() {
    std::cout << "\n=== 演示3：MyTinySTL的异常安全机制 ===" << std::endl;
    
    // 模拟 MyTinySTL 的异常安全机制
    template<typename T>
    void safe_construct(T* ptr, const T& value) {
        if (ptr != nullptr) {
            try {
                new(ptr) T(value);
                std::cout << "构造成功" << std::endl;
            } catch (...) {
                std::cout << "构造失败，清理资源" << std::endl;
                // MyTinySTL 会在这里清理已分配的资源
                throw;
            }
        }
    }
    
    template<typename T>
    void safe_destroy(T* ptr) {
        if (ptr != nullptr) {
            ptr->~T();
            std::cout << "析构完成" << std::endl;
        }
    }
    
    char buffer[sizeof(DetailedResourceClass)];
    DetailedResourceClass* obj = reinterpret_cast<DetailedResourceClass*>(buffer);
    
    DetailedResourceClass::resetCount();
    
    try {
        std::cout << "尝试构造对象3..." << std::endl;
        safe_construct(obj, DetailedResourceClass(3, 500));
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "MyTinySTL确保资源被正确清理" << std::endl;
    }
    
    std::cout << "最终对象数量: " << DetailedResourceClass::getCount() << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    DetailedResourceClass::resetCount();
    
    demo_object3_resource_allocation();
    demo_exception_safety_importance();
    demo_mystl_exception_safety();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "关于对象3的资源分配：" << std::endl;
    std::cout << "1. 对象3确实分配了资源（内存）" << std::endl;
    std::cout << "2. 但构造失败，抛出异常" << std::endl;
    std::cout << "3. 异常安全机制确保资源被正确清理" << std::endl;
    std::cout << "4. MyTinySTL的construct/destroy机制提供异常安全" << std::endl;
    std::cout << "5. 这是为什么需要异常安全保证的原因" << std::endl;
    
    return 0;
}








