#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

// 简化的内存管理实现
namespace mystl {

// 简化的 construct.h 功能
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

// 简化的 uninitialized.h 功能
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    ForwardIterator current = result;
    try {
        for (; first != last; ++first, ++current) {
            construct(&*current, *first);
        }
        return current;
    } catch (...) {
        // 异常安全：析构已构造的对象
        for (ForwardIterator it = result; it != current; ++it) {
            destroy(&*it);
        }
        throw;
    }
}

// 简化的 functional.h 功能
template<typename T>
struct plus {
    T operator()(const T& x, const T& y) const {
        return x + y;
    }
};

template<typename T>
struct multiplies {
    T operator()(const T& x, const T& y) const {
        return x * y;
    }
};

} // namespace mystl

// 测试用的类
class TestClass {
private:
    int value_;
    std::string name_;

public:
    TestClass(int v, const std::string& n) : value_(v), name_(n) {
        std::cout << "构造 TestClass(" << v << ", \"" << n << "\")" << std::endl;
    }
    
    TestClass(const TestClass& other) : value_(other.value_), name_(other.name_) {
        std::cout << "拷贝构造 TestClass(" << value_ << ", \"" << name_ << "\")" << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass(" << value_ << ", \"" << name_ << "\")" << std::endl;
    }
    
    int getValue() const { return value_; }
    std::string getName() const { return name_; }
    
    TestClass operator+(const TestClass& other) const {
        return TestClass(value_ + other.value_, name_ + "+" + other.name_);
    }
    
    TestClass operator*(const TestClass& other) const {
        return TestClass(value_ * other.value_, name_ + "*" + other.name_);
    }
};

void demonstrate_construct_destroy() {
    std::cout << "=== construct.h 演示 ===" << std::endl;
    
    // 分配原始内存
    char buffer[sizeof(TestClass) * 3];
    TestClass* objects = reinterpret_cast<TestClass*>(buffer);
    
    std::cout << "1. 构造对象:" << std::endl;
    mystl::construct(&objects[0], 10, "Alice");
    mystl::construct(&objects[1], 20, "Bob");
    mystl::construct(&objects[2], 30, "Charlie");
    
    std::cout << "2. 使用对象:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "   objects[" << i << "] = (" << objects[i].getValue() 
                  << ", \"" << objects[i].getName() << "\")" << std::endl;
    }
    
    std::cout << "3. 析构对象:" << std::endl;
    mystl::destroy(&objects[0]);
    mystl::destroy(&objects[1]);
    mystl::destroy(&objects[2]);
}

void demonstrate_uninitialized_copy() {
    std::cout << "\n=== uninitialized.h 演示 ===" << std::endl;
    
    // 源数据
    std::vector<TestClass> source;
    source.emplace_back(1, "One");
    source.emplace_back(2, "Two");
    source.emplace_back(3, "Three");
    
    // 目标缓冲区
    char buffer[sizeof(TestClass) * 3];
    TestClass* dest = reinterpret_cast<TestClass*>(buffer);
    
    std::cout << "1. 未初始化拷贝:" << std::endl;
    mystl::uninitialized_copy(source.begin(), source.end(), dest);
    
    std::cout << "2. 使用拷贝的对象:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "   dest[" << i << "] = (" << dest[i].getValue() 
                  << ", \"" << dest[i].getName() << "\")" << std::endl;
    }
    
    std::cout << "3. 析构拷贝的对象:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        mystl::destroy(&dest[i]);
    }
}

void demonstrate_functional_usage() {
    std::cout << "\n=== functional.h 演示 ===" << std::endl;
    
    // 使用函数对象
    mystl::plus<int> add;
    mystl::multiplies<int> mul;
    
    int a = 5, b = 3;
    std::cout << "add(" << a << ", " << b << ") = " << add(a, b) << std::endl;
    std::cout << "mul(" << a << ", " << b << ") = " << mul(a, b) << std::endl;
    
    // 使用函数对象处理对象
    TestClass obj1(10, "A");
    TestClass obj2(20, "B");
    
    std::cout << "obj1 + obj2 = (" << (obj1 + obj2).getValue() 
              << ", \"" << (obj1 + obj2).getName() << "\")" << std::endl;
}

void demonstrate_integration() {
    std::cout << "\n=== 三层集成演示 ===" << std::endl;
    
    // 1. 分配内存
    char buffer[sizeof(TestClass) * 2];
    TestClass* objects = reinterpret_cast<TestClass*>(buffer);
    
    // 2. 构造对象
    mystl::construct(&objects[0], 5, "First");
    mystl::construct(&objects[1], 10, "Second");
    
    // 3. 使用函数对象操作
    mystl::plus<TestClass> add_objects;
    TestClass result = add_objects(objects[0], objects[1]);
    
    std::cout << "result = (" << result.getValue() 
              << ", \"" << result.getName() << "\")" << std::endl;
    
    // 4. 析构对象
    mystl::destroy(&objects[0]);
    mystl::destroy(&objects[1]);
}

void demonstrate_memory_lifecycle() {
    std::cout << "\n=== 内存生命周期演示 ===" << std::endl;
    
    std::cout << "1. 分配原始内存" << std::endl;
    char* raw_memory = new char[sizeof(TestClass) * 2];
    TestClass* objects = reinterpret_cast<TestClass*>(raw_memory);
    
    std::cout << "2. 构造对象（construct.h）" << std::endl;
    mystl::construct(&objects[0], 100, "Memory");
    mystl::construct(&objects[1], 200, "Management");
    
    std::cout << "3. 使用函数对象（functional.h）" << std::endl;
    mystl::plus<TestClass> add;
    TestClass sum = add(objects[0], objects[1]);
    std::cout << "   求和结果: (" << sum.getValue() 
              << ", \"" << sum.getName() << "\")" << std::endl;
    
    std::cout << "4. 析构对象（construct.h）" << std::endl;
    mystl::destroy(&objects[0]);
    mystl::destroy(&objects[1]);
    
    std::cout << "5. 释放原始内存" << std::endl;
    delete[] raw_memory;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL 内存管理系统演示 ===" << std::endl;
    
    demonstrate_construct_destroy();
    demonstrate_uninitialized_copy();
    demonstrate_functional_usage();
    demonstrate_integration();
    demonstrate_memory_lifecycle();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    
    return 0;
}










