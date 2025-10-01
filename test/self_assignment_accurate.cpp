#include <iostream>
#include <string>
#include <windows.h>
#include "../exceptdef.h"

// 演示类：展示自赋值的真正问题
class ResourceClass {
    int* data;
    size_t size;
    static int copy_count;  // 统计复制次数
    
public:
    ResourceClass(size_t s) : size(s) {
        data = new int[s];
        for (size_t i = 0; i < s; ++i) {
            data[i] = i;
        }
        std::cout << "  构造 ResourceClass，大小: " << size << std::endl;
    }
    
    ~ResourceClass() {
        delete[] data;
        std::cout << "  析构 ResourceClass" << std::endl;
    }
    
    // 有自赋值检查的赋值操作符
    ResourceClass& operator=(const ResourceClass& other) {
        std::cout << "  执行赋值操作（有自赋值检查）..." << std::endl;
        copy_count++;        ///1000次
        
        if (this != &other) {  // 自赋值检查
            std::cout << "    不是自赋值，执行复制..." << std::endl;
            if (size != other.size) {
                delete[] data;
                size = other.size;
                data = new int[size];
            }
            
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            std::cout << "    是自赋值，跳过操作..." << std::endl;
        }
        
        return *this;
    }
    
    size_t getSize() const { return size; }
    static int getCopyCount() { return copy_count; }
    static void resetCopyCount() { copy_count = 0; }
};

int ResourceClass::copy_count = 0;

void demonstrate_performance_difference() {
    std::cout << "=== 自赋值性能差异演示 ===" << std::endl;
    
    ResourceClass obj(1000);
    
    // 测试自赋值检查
    std::cout << "1. 自赋值检查测试:" << std::endl;
    ResourceClass::resetCopyCount();
    
    obj = obj;  // 自赋值
    std::cout << "   复制次数: " << ResourceClass::getCopyCount() << std::endl; ///////1000次
    
    // 测试正常赋值
    std::cout << "\n2. 正常赋值测试:" << std::endl;
    ResourceClass obj2(500);
    ResourceClass::resetCopyCount();
    
    obj = obj2;  // 正常赋值
    std::cout << "   复制次数: " << ResourceClass::getCopyCount() << std::endl; ///////500次
}

void demonstrate_mystl_exception_self_assignment() {
    std::cout << "\n=== MyTinySTL 异常类自赋值演示 ===" << std::endl;
    
    // 1. 正常赋值
    std::cout << "1. 正常赋值:" << std::endl;
    mystl::mystl_exception ex1("第一个异常");
    mystl::mystl_exception ex2("第二个异常");
    
    std::cout << "   赋值前 ex1: " << ex1.what() << std::endl;
    std::cout << "   赋值前 ex2: " << ex2.what() << std::endl;
    
    ex1 = ex2;  // 正常赋值
    
    std::cout << "   赋值后 ex1: " << ex1.what() << std::endl;
    std::cout << "   赋值后 ex2: " << ex2.what() << std::endl;
    
    // 2. 自赋值
    std::cout << "\n2. 自赋值:" << std::endl;
    mystl::mystl_exception ex3("自赋值测试");
    std::cout << "   自赋值前: " << ex3.what() << std::endl;
    
    ex3 = ex3;  // 自赋值
    
    std::cout << "   自赋值后: " << ex3.what() << std::endl;
    std::cout << "   自赋值不会抛出异常，但会执行不必要的操作" << std::endl;
}
void demonstrate_why_self_assignment_check() {
    std::cout << "\n=== 为什么需要自赋值检查 ===" << std::endl;
    
    // 1. 性能考虑
    std::cout << "1. 性能考虑:" << std::endl;
    std::cout << "   自赋值时，数据已经是正确的，不需要重新赋值" << std::endl;
    std::cout << "   避免不必要的内存操作和计算" << std::endl;
    
    // 2. 代码清晰性
    std::cout << "\n2. 代码清晰性:" << std::endl;
    std::cout << "   明确表达意图：只有在需要时才执行赋值" << std::endl;
    std::cout << "   符合防御性编程原则" << std::endl;
    
    // 3. 标准库兼容性
    std::cout << "\n3. 标准库兼容性:" << std::endl;
    std::cout << "   标准库中的类都实现了自赋值检查" << std::endl;
    std::cout << "   保持与标准库的一致性" << std::endl;
    
    // 4. 未来扩展性
    std::cout << "\n4. 未来扩展性:" << std::endl;
    std::cout << "   如果将来类变得更复杂，自赋值检查仍然有效" << std::endl;
    std::cout << "   避免在复杂操作中出现问题" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 自赋值问题准确分析 ===" << std::endl;
    
    demonstrate_performance_difference();
    demonstrate_mystl_exception_self_assignment();
    demonstrate_why_self_assignment_check();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}
