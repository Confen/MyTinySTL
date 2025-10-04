#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：会在构造和析构时输出信息
class DemoClass {
private:
    int value_;
    static int count_;
    
public:
    DemoClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 DemoClass(" << value_ << "), 总数: " << count_ << std::endl;
    }
    
    ~DemoClass() {
        --count_;
        std::cout << "析构 DemoClass(" << value_ << "), 剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int DemoClass::count_ = 0;

// 模拟 MyTinySTL 的 construct 和 destroy 函数
template<typename T, typename... Args>
void construct(T* ptr, Args&&... args) {
    if (ptr != nullptr) {
        new(ptr) T(std::forward<Args>(args)...);  // placement new
    }
}

template<typename T>
void destroy(T* ptr) {
    if (ptr != nullptr) {
        ptr->~T();  // 显式调用析构函数
    }
}

// 演示1：基本概念
void demo_basic_concept() {
    std::cout << "=== 演示1：基本概念 ===" << std::endl;
    
    // 1. 分配原始内存（不构造对象）
    char buffer[sizeof(DemoClass)];
    std::cout << "分配了 " << sizeof(DemoClass) << " 字节的原始内存" << std::endl;
    
    // 2. 在内存上构造对象
    DemoClass* obj = reinterpret_cast<DemoClass*>(buffer);
    construct(obj, 42);
    std::cout << "在内存上构造了对象，值: " << obj->getValue() << std::endl;
    
    // 3. 析构对象但不释放内存
    destroy(obj);
    std::cout << "析构了对象，但内存仍然存在" << std::endl;
    
    // 4. 可以重新使用同一块内存
    construct(obj, 100);
    std::cout << "在同一块内存上重新构造对象，值: " << obj->getValue() << std::endl;
    
    destroy(obj);
    std::cout << "再次析构对象" << std::endl;
}

// 演示2：容器中的应用
void demo_container_usage() {
    std::cout << "\n=== 演示2：容器中的应用 ===" << std::endl;
    
    // 模拟容器的内存管理
    const size_t capacity = 5;
    char buffer[sizeof(DemoClass) * capacity];
    DemoClass* objects = reinterpret_cast<DemoClass*>(buffer);
    
    std::cout << "分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    // 构造前3个对象
    for (size_t i = 0; i < 3; ++i) {
        construct(objects + i, static_cast<int>(i + 1));
    }
    std::cout << "构造了3个对象" << std::endl;
    
    // 析构第2个对象
    destroy(objects + 1);
    std::cout << "析构了第2个对象" << std::endl;
    
    // 在第2个位置重新构造对象
    construct(objects + 1, 999);
    std::cout << "在第2个位置重新构造对象，值: " << objects[1].getValue() << std::endl;
    
    // 析构所有对象
    for (size_t i = 0; i < 3; ++i) {
        destroy(objects + i);
    }
    std::cout << "析构了所有对象，但内存仍然存在" << std::endl;
}

// 演示3：异常安全
void demo_exception_safety() {
    std::cout << "\n=== 演示3：异常安全 ===" << std::endl;
    
    // 会抛出异常的类
    class ThrowingClass {
    private:
        int value_;
        static int count_;
        
    public:
        ThrowingClass(int v) : value_(v) {
            ++count_;
            std::cout << "构造 ThrowingClass(" << value_ << "), 总数: " << count_ << std::endl;
            if (count_ == 3) {
                throw std::runtime_error("构造失败");
            }
        }
        
        ~ThrowingClass() {
            --count_;
            std::cout << "析构 ThrowingClass(" << value_ << "), 剩余: " << count_ << std::endl;
        }
        
        static void resetCount() { count_ = 0; }
        static int getCount() { return count_; }
    };
    
    int ThrowingClass::count_ = 0;
    
    char buffer[sizeof(ThrowingClass) * 5];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    try {
        std::cout << "尝试构造5个对象，第3个会抛出异常" << std::endl;
        for (int i = 0; i < 5; ++i) {
            construct(objects + i, i + 1);
        }
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "已构造的对象数量: " << ThrowingClass::getCount() << std::endl;
        
        // 异常安全：析构已构造的对象
        std::cout << "开始清理已构造的对象..." << std::endl;
        for (int i = 0; i < ThrowingClass::getCount(); ++i) {
            destroy(objects + i);
        }
        std::cout << "清理完成" << std::endl;
    }
}

// 演示4：与标准库的对比
void demo_standard_comparison() {
    std::cout << "\n=== 演示4：与标准库的对比 ===" << std::endl;
    
    std::cout << "标准库方式（new/delete）：" << std::endl;
    DemoClass* ptr1 = new DemoClass(1);
    delete ptr1;  // 同时析构对象和释放内存
    std::cout << "delete 后，指针变为悬空指针" << std::endl;
    
    std::cout << "\nMyTinySTL 方式（分离管理）：" << std::endl;
    char buffer[sizeof(DemoClass)];
    DemoClass* ptr2 = reinterpret_cast<DemoClass*>(buffer);
    
    construct(ptr2, 2);
    destroy(ptr2);  // 只析构对象，内存仍然有效
    std::cout << "destroy 后，内存仍然有效，可以重新使用" << std::endl;
    
    construct(ptr2, 3);
    std::cout << "重新使用同一块内存，值: " << ptr2->getValue() << std::endl;
    destroy(ptr2);
}

// 演示5：性能优势
void demo_performance_advantage() {
    std::cout << "\n=== 演示5：性能优势 ===" << std::endl;
    
    const int iterations = 1000;
    
    // 方式1：每次 new/delete
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        DemoClass* ptr = new DemoClass(i);
        delete ptr;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 方式2：预分配内存 + construct/destroy
    char buffer[sizeof(DemoClass)];
    DemoClass* ptr = reinterpret_cast<DemoClass*>(buffer);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        construct(ptr, i);
        destroy(ptr);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "new/delete 方式耗时: " << duration1.count() << " 微秒" << std::endl;
    std::cout << "construct/destroy 方式耗时: " << duration2.count() << " 微秒" << std::endl;
    std::cout << "性能提升: " << (double)duration1.count() / duration2.count() << " 倍" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    DemoClass::resetCount();
    
    demo_basic_concept();
    demo_container_usage();
    demo_exception_safety();
    demo_standard_comparison();
    demo_performance_advantage();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "析构对象但不释放内存的优势：" << std::endl;
    std::cout << "1. 内存复用：同一块内存可以多次使用" << std::endl;
    std::cout << "2. 性能提升：避免频繁的内存分配/释放" << std::endl;
    std::cout << "3. 异常安全：可以精确控制对象生命周期" << std::endl;
    std::cout << "4. 容器优化：容器的内存管理更加灵活" << std::endl;
    
    return 0;
}
















