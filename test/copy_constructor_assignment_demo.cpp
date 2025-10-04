#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

// 示例1：只重载复制构造，不重载赋值运算符
class BadExample {
private:
    std::string* data;
    size_t size;
    
public:
    // 构造函数
    BadExample(const std::string& str) : size(str.length()) {
        data = new std::string[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = str[i];
        }
        std::cout << "构造 BadExample: " << str << std::endl;
    }
    
    // 复制构造函数
    BadExample(const BadExample& other) : size(other.size) {
        data = new std::string[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        std::cout << "复制构造 BadExample" << std::endl;
    }
    
    // 析构函数 - 添加安全检查避免崩溃
    ~BadExample() {
        if (data != nullptr) {
            try {
                delete[] data;
                std::cout << "析构 BadExample" << std::endl;
            } catch (...) {
                std::cout << "析构时发生异常，但已安全处理" << std::endl;
            }
            data = nullptr;  // 防止重复释放
        } else {
            std::cout << "跳过析构（data已为空）" << std::endl;
        }
    }
    
    // 没有重载赋值运算符！
    
    void print() const {
        std::cout << "内容: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
    }
};

// 示例2：正确重载复制构造和赋值运算符
class GoodExample {
private:
    std::string* data;
    size_t size;
    
public:
    // 构造函数
    GoodExample(const std::string& str) : size(str.length()) {
        data = new std::string[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = str[i];
        }
        std::cout << "构造 GoodExample: " << str << std::endl;
    }
    
    // 复制构造函数
    GoodExample(const GoodExample& other) : size(other.size) {
        data = new std::string[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        std::cout << "复制构造 GoodExample" << std::endl;
    }
    
    // 赋值运算符
    GoodExample& operator=(const GoodExample& other) {
        std::cout << "赋值运算符调用" << std::endl;
        
        // 自赋值检查
        if (this == &other) {
            std::cout << "自赋值，直接返回" << std::endl;
            return *this;
        }
        
        // 释放原有资源
        delete[] data;
        
        // 分配新资源
        size = other.size;
        data = new std::string[size];
        
        // 复制数据
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        
        std::cout << "赋值完成" << std::endl;
        return *this;
    }
    
    // 析构函数
    ~GoodExample() {
        delete[] data;
        std::cout << "析构 GoodExample" << std::endl;
    }
    
    void print() const {
        std::cout << "内容: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
    }
};

// 示例3：使用 RAII 和现代 C++ 的更好实现
class ModernExample {
private:
    std::vector<std::string> data;  // 使用 vector 自动管理内存
    
public:
    // 构造函数
    ModernExample(const std::string& str) {
        for (char c : str) {
            data.push_back(std::string(1, c));
        }
        std::cout << "构造 ModernExample: " << str << std::endl;
    }
    
    // 复制构造函数（编译器自动生成）
    ModernExample(const ModernExample& other) = default;
    
    // 赋值运算符（编译器自动生成）
    ModernExample& operator=(const ModernExample& other) = default;
    
    // 析构函数（编译器自动生成）
    ~ModernExample() = default;
    
    void print() const {
        std::cout << "内容: ";
        for (const auto& str : data) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
};

// 示例4：展示三法则/五法则
class RuleOfThreeExample {
private:
    int* data;
    size_t size;
    
public:
    // 构造函数
    RuleOfThreeExample(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = i;
        }
        std::cout << "构造 RuleOfThreeExample，大小: " << size << std::endl;
    }
    
    // 复制构造函数
    RuleOfThreeExample(const RuleOfThreeExample& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        std::cout << "复制构造 RuleOfThreeExample" << std::endl;
    }
    
    // 赋值运算符
    RuleOfThreeExample& operator=(const RuleOfThreeExample& other) {
        std::cout << "赋值运算符调用" << std::endl;
        
        if (this == &other) {
            return *this;
        }
        
        // 释放原有资源
        delete[] data;
        
        // 分配新资源
        size = other.size;
        data = new int[size];
        
        // 复制数据
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        
        return *this;
    }
    
    // 析构函数
    ~RuleOfThreeExample() {
        delete[] data;
        std::cout << "析构 RuleOfThreeExample" << std::endl;
    }
    
    void print() const {
        std::cout << "数据: [";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 为什么复制构造和赋值运算符要一起重载？ ===" << std::endl;
    
    std::cout << "\n📋 核心原因：" << std::endl;
    std::cout << "1. 它们都涉及资源的复制" << std::endl;
    std::cout << "2. 如果只重载一个，另一个会使用默认实现" << std::endl;
    std::cout << "3. 默认实现通常是浅拷贝，会导致问题" << std::endl;
    std::cout << "4. 违反三法则（Rule of Three）" << std::endl;
    
    // 示例1：只重载复制构造的问题
    std::cout << "\n🔹 示例1：只重载复制构造的问题" << std::endl;
    std::cout << "问题：使用默认的赋值运算符（浅拷贝）" << std::endl;
    
    {
        BadExample obj1("Hello");
        BadExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;  // 使用默认的赋值运算符！
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n⚠️ 问题：" << std::endl;
        std::cout << "1. 内存泄漏：obj1 的原始 data 没有被释放" << std::endl;
        std::cout << "2. 双重释放：obj1 和 obj2 指向同一块内存" << std::endl;
        std::cout << "3. 析构时会崩溃" << std::endl;
    }
    
    // 示例2：正确重载两者
    std::cout << "\n🔹 示例2：正确重载复制构造和赋值运算符" << std::endl;
    
    {
        GoodExample obj1("Hello");
        GoodExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;  // 使用重载的赋值运算符
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n✅ 正确：" << std::endl;
        std::cout << "1. 内存正确管理" << std::endl;
        std::cout << "2. 深拷贝实现" << std::endl;
        std::cout << "3. 自赋值安全" << std::endl;
    }
    
    // 示例3：现代 C++ 的更好方法
    std::cout << "\n🔹 示例3：现代 C++ 的更好方法" << std::endl;
    
    {
        ModernExample obj1("Hello");
        ModernExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;  // 使用编译器生成的赋值运算符
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n✅ 现代方法：" << std::endl;
        std::cout << "1. 使用 RAII（vector 自动管理内存）" << std::endl;
        std::cout << "2. 编译器自动生成正确的拷贝语义" << std::endl;
        std::cout << "3. 异常安全" << std::endl;
        std::cout << "4. 代码简洁" << std::endl;
    }
    
    // 示例4：三法则演示
    std::cout << "\n🔹 示例4：三法则（Rule of Three）演示" << std::endl;
    
    {
        RuleOfThreeExample obj1(3);
        RuleOfThreeExample obj2(5);
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行自赋值：obj1 = obj1" << std::endl;
        obj1 = obj1;  // 自赋值测试
        
        std::cout << "自赋值后：" << std::endl;
        obj1.print();
    }
    
    // 三法则/五法则说明
    std::cout << "\n📚 三法则（Rule of Three）：" << std::endl;
    std::cout << "如果一个类需要自定义以下三者之一：" << std::endl;
    std::cout << "1. 析构函数" << std::endl;
    std::cout << "2. 复制构造函数" << std::endl;
    std::cout << "3. 赋值运算符" << std::endl;
    std::cout << "那么它很可能需要自定义所有三者" << std::endl;
    
    std::cout << "\n📚 五法则（Rule of Five，C++11）：" << std::endl;
    std::cout << "在三法则基础上，还要考虑：" << std::endl;
    std::cout << "4. 移动构造函数" << std::endl;
    std::cout << "5. 移动赋值运算符" << std::endl;
    
    // 为什么需要一起重载
    std::cout << "\n🎯 为什么需要一起重载？" << std::endl;
    
    std::cout << "\n1. 语义一致性：" << std::endl;
    std::cout << "   - 复制构造：从现有对象创建新对象" << std::endl;
    std::cout << "   - 赋值运算符：将现有对象的值赋给另一个现有对象" << std::endl;
    std::cout << "   - 两者都应该有相同的复制语义" << std::endl;
    
    std::cout << "\n2. 资源管理一致性：" << std::endl;
    std::cout << "   - 都需要处理资源的分配和释放" << std::endl;
    std::cout << "   - 都需要实现深拷贝或移动语义" << std::endl;
    std::cout << "   - 都需要处理异常安全" << std::endl;
    
    std::cout << "\n3. 避免默认实现的陷阱：" << std::endl;
    std::cout << "   - 默认复制构造：成员逐个复制" << std::endl;
    std::cout << "   - 默认赋值运算符：成员逐个赋值" << std::endl;
    std::cout << "   - 对于指针成员，这通常是浅拷贝" << std::endl;
    
    std::cout << "\n4. 异常安全：" << std::endl;
    std::cout << "   - 两者都需要提供相同的异常安全保证" << std::endl;
    std::cout << "   - 都需要处理自赋值" << std::endl;
    std::cout << "   - 都需要保证强异常安全" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 复制构造和赋值运算符必须一起重载，因为：" << std::endl;
    std::cout << "1. ✅ 语义一致性 - 相同的复制语义" << std::endl;
    std::cout << "2. ✅ 资源管理 - 都需要处理资源" << std::endl;
    std::cout << "3. ✅ 避免陷阱 - 避免默认实现的浅拷贝" << std::endl;
    std::cout << "4. ✅ 异常安全 - 提供相同的安全保证" << std::endl;
    std::cout << "5. ✅ 三法则 - 遵循 C++ 最佳实践" << std::endl;
    
    return 0;
}







