#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

// 测试类 - 用于观察析构函数调用
class TestClass {
private:
    int id;
    std::string name;
    
public:
    TestClass(int i, const std::string& n) : id(i), name(n) {
        std::cout << "构造 TestClass[" << id << "] - " << name << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass[" << id << "] - " << name << std::endl;
    }
    
    void print() const {
        std::cout << "TestClass[" << id << "] - " << name << std::endl;
    }
};

// 测试类 - 包含 string 成员变量，析构函数抛出异常
class StringContainerWithException {
private:
    std::string stringMember1;
    std::string stringMember2;
    std::string stringMember3;
    int id;
    
public:
    StringContainerWithException(int i) : id(i) {
        std::cout << "构造 StringContainerWithException[" << id << "]" << std::endl;
        
        stringMember1 = "第一个字符串成员";
        stringMember2 = "第二个字符串成员";
        stringMember3 = "第三个字符串成员";
        
        std::cout << "StringContainerWithException[" << id << "] 构造完成" << std::endl;
    }
    
    ~StringContainerWithException() {
        std::cout << "析构 StringContainerWithException[" << id << "]" << std::endl;
        
        // 模拟析构函数中的异常
        if (id == 1) {
            std::cout << "抛出异常，但 string 成员仍会析构" << std::endl;
            throw std::runtime_error("析构函数异常");
        }
        
        std::cout << "StringContainerWithException[" << id << "] 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringContainerWithException[" << id << "] 内容:" << std::endl;
        std::cout << "stringMember1: " << stringMember1 << std::endl;
        std::cout << "stringMember2: " << stringMember2 << std::endl;
        std::cout << "stringMember3: " << stringMember3 << std::endl;
    }
};

// 测试类 - 包含 string 数组，析构函数抛出异常
class StringArrayContainerWithException {
private:
    std::string* stringArray;
    size_t arraySize;
    int id;
    
public:
    StringArrayContainerWithException(int i, size_t size) : arraySize(size), id(i) {
        std::cout << "构造 StringArrayContainerWithException[" << id << "]" << std::endl;
        
        stringArray = new std::string[arraySize];
        for (size_t j = 0; j < arraySize; ++j) {
            stringArray[j] = "字符串数组元素 " + std::to_string(j);
        }
        
        std::cout << "StringArrayContainerWithException[" << id << "] 构造完成" << std::endl;
    }
    
    ~StringArrayContainerWithException() {
        std::cout << "析构 StringArrayContainerWithException[" << id << "]" << std::endl;
        
        // 模拟析构函数中的异常
        if (id == 1) {
            std::cout << "抛出异常，但 string 数组仍会析构" << std::endl;
            throw std::runtime_error("析构函数异常");
        }
        
        // delete[] 会自动调用每个 string 的析构函数
        delete[] stringArray;
        
        std::cout << "StringArrayContainerWithException[" << id << "] 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringArrayContainerWithException[" << id << "] 内容:" << std::endl;
        for (size_t j = 0; j < arraySize; ++j) {
            std::cout << "stringArray[" << j << "]: " << stringArray[j] << std::endl;
        }
    }
};

// 测试类 - 包含 string 向量，析构函数抛出异常
class StringVectorContainerWithException {
private:
    std::vector<std::string> stringVector;
    int id;
    
public:
    StringVectorContainerWithException(int i) : id(i) {
        std::cout << "构造 StringVectorContainerWithException[" << id << "]" << std::endl;
        
        stringVector.emplace_back("向量字符串1");
        stringVector.emplace_back("向量字符串2");
        stringVector.emplace_back("向量字符串3");
        
        std::cout << "StringVectorContainerWithException[" << id << "] 构造完成" << std::endl;
    }
    
    ~StringVectorContainerWithException() {
        std::cout << "析构 StringVectorContainerWithException[" << id << "]" << std::endl;
        
        // 模拟析构函数中的异常
        if (id == 1) {
            std::cout << "抛出异常，但 string 向量仍会析构" << std::endl;
            throw std::runtime_error("析构函数异常");
        }
        
        std::cout << "StringVectorContainerWithException[" << id << "] 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringVectorContainerWithException[" << id << "] 内容:" << std::endl;
        for (size_t j = 0; j < stringVector.size(); ++j) {
            std::cout << "stringVector[" << j << "]: " << stringVector[j] << std::endl;
        }
    }
};

// 测试类 - 包含 string 智能指针，析构函数抛出异常
class StringSmartPtrContainerWithException {
private:
    std::unique_ptr<std::string> stringSmartPtr1;
    std::unique_ptr<std::string> stringSmartPtr2;
    int id;
    
public:
    StringSmartPtrContainerWithException(int i) : id(i) {
        std::cout << "构造 StringSmartPtrContainerWithException[" << id << "]" << std::endl;
        
        stringSmartPtr1 = std::unique_ptr<std::string>(new std::string("智能指针字符串1"));
        stringSmartPtr2 = std::unique_ptr<std::string>(new std::string("智能指针字符串2"));
        
        std::cout << "StringSmartPtrContainerWithException[" << id << "] 构造完成" << std::endl;
    }
    
    ~StringSmartPtrContainerWithException() {
        std::cout << "析构 StringSmartPtrContainerWithException[" << id << "]" << std::endl;
        
        // 模拟析构函数中的异常
        if (id == 1) {
            std::cout << "抛出异常，但 string 智能指针仍会析构" << std::endl;
            throw std::runtime_error("析构函数异常");
        }
        
        std::cout << "StringSmartPtrContainerWithException[" << id << "] 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringSmartPtrContainerWithException[" << id << "] 内容:" << std::endl;
        std::cout << "stringSmartPtr1: " << *stringSmartPtr1 << std::endl;
        std::cout << "stringSmartPtr2: " << *stringSmartPtr2 << std::endl;
    }
};

// 测试类 - 演示异常传播
class ExceptionPropagationTest {
private:
    std::string stringMember;
    int id;
    
public:
    ExceptionPropagationTest(int i) : id(i) {
        std::cout << "构造 ExceptionPropagationTest[" << id << "]" << std::endl;
        stringMember = "异常传播测试字符串";
    }
    
    ~ExceptionPropagationTest() {
        std::cout << "析构 ExceptionPropagationTest[" << id << "]" << std::endl;
        
        // 模拟析构函数中的异常
        if (id == 1) {
            std::cout << "抛出异常，但 string 成员仍会析构" << std::endl;
            throw std::runtime_error("析构函数异常");
        }
        
        std::cout << "ExceptionPropagationTest[" << id << "] 析构完成" << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== string 类型在异常情况下的析构行为 ===" << std::endl;
    
    std::cout << "\n📋 核心问题：" << std::endl;
    std::cout << "string 类型在异常情况下是否仍会析构？" << std::endl;
    std::cout << "即使析构函数抛出异常，string 也会析构吗？" << std::endl;
    
    // 测试1：string 成员变量 + 析构函数异常
    std::cout << "\n🔹 测试1：string 成员变量 + 析构函数异常" << std::endl;
    std::cout << "即使析构函数抛出异常，string 成员仍会析构" << std::endl;
    
    try {
        StringContainerWithException container(1);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 成员已经析构了！" << std::endl;
    }
    
    // 测试2：string 数组 + 析构函数异常
    std::cout << "\n🔹 测试2：string 数组 + 析构函数异常" << std::endl;
    std::cout << "即使析构函数抛出异常，string 数组仍会析构" << std::endl;
    
    try {
        StringArrayContainerWithException container(1, 3);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 数组已经析构了！" << std::endl;
    }
    
    // 测试3：string 向量 + 析构函数异常
    std::cout << "\n🔹 测试3：string 向量 + 析构函数异常" << std::endl;
    std::cout << "即使析构函数抛出异常，string 向量仍会析构" << std::endl;
    
    try {
        StringVectorContainerWithException container(1);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 向量已经析构了！" << std::endl;
    }
    
    // 测试4：string 智能指针 + 析构函数异常
    std::cout << "\n🔹 测试4：string 智能指针 + 析构函数异常" << std::endl;
    std::cout << "即使析构函数抛出异常，string 智能指针仍会析构" << std::endl;
    
    try {
        StringSmartPtrContainerWithException container(1);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 智能指针已经析构了！" << std::endl;
    }
    
    // 测试5：正常情况（无异常）
    std::cout << "\n🔹 测试5：正常情况（无异常）" << std::endl;
    std::cout << "正常析构，string 成员正常析构" << std::endl;
    
    {
        StringContainerWithException container(2);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 正常析构
    
    // 测试6：异常传播测试
    std::cout << "\n🔹 测试6：异常传播测试" << std::endl;
    std::cout << "析构函数中的异常会传播到调用者" << std::endl;
    
    try {
        ExceptionPropagationTest test(1);
        std::cout << "test 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 成员已经析构了！" << std::endl;
    }
    
    // 测试7：嵌套异常测试
    std::cout << "\n🔹 测试7：嵌套异常测试" << std::endl;
    std::cout << "嵌套对象中的异常处理" << std::endl;
    
    try {
        class NestedTest {
        private:
            std::string stringMember;
            int id;
            
        public:
            NestedTest(int i) : id(i) {
                std::cout << "构造 NestedTest[" << id << "]" << std::endl;
                stringMember = "嵌套测试字符串";
            }
            
            ~NestedTest() {
                std::cout << "析构 NestedTest[" << id << "]" << std::endl;
                
                if (id == 1) {
                    std::cout << "抛出异常，但 string 成员仍会析构" << std::endl;
                    throw std::runtime_error("嵌套析构函数异常");
                }
            }
        };
        
        NestedTest test(1);
        std::cout << "test 即将析构..." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "注意：string 成员已经析构了！" << std::endl;
    }
    
    // 总结
    std::cout << "\n📊 string 类型异常析构总结：" << std::endl;
    
    std::cout << "\n1. string 成员变量：" << std::endl;
    std::cout << "   即使析构函数抛出异常，string 成员仍会析构" << std::endl;
    std::cout << "   析构顺序：先析构成员变量，再执行析构函数体" << std::endl;
    
    std::cout << "\n2. string 数组：" << std::endl;
    std::cout << "   即使析构函数抛出异常，string 数组仍会析构" << std::endl;
    std::cout << "   delete[] 会自动调用每个 string 的析构函数" << std::endl;
    
    std::cout << "\n3. string 向量：" << std::endl;
    std::cout << "   即使析构函数抛出异常，string 向量仍会析构" << std::endl;
    std::cout << "   vector 会自动析构每个 string 元素" << std::endl;
    
    std::cout << "\n4. string 智能指针：" << std::endl;
    std::cout << "   即使析构函数抛出异常，string 智能指针仍会析构" << std::endl;
    std::cout << "   智能指针会自动析构 string 对象" << std::endl;
    
    std::cout << "\n⚠️ 重要注意事项：" << std::endl;
    std::cout << "1. string 析构函数是 noexcept 的，不会抛出异常" << std::endl;
    std::cout << "2. 即使容器析构函数抛出异常，string 也会被正确析构" << std::endl;
    std::cout << "3. 析构顺序：先析构成员变量，再执行析构函数体" << std::endl;
    std::cout << "4. 析构函数中的异常会传播到调用者" << std::endl;
    std::cout << "5. 不建议在析构函数中抛出异常" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 string 类型在异常情况下的析构：" << std::endl;
    std::cout << "• 作为成员变量 → 即使析构函数异常，仍会析构" << std::endl;
    std::cout << "• 作为数组元素 → 即使析构函数异常，仍会析构" << std::endl;
    std::cout << "• 作为向量元素 → 即使析构函数异常，仍会析构" << std::endl;
    std::cout << "• 作为智能指针 → 即使析构函数异常，仍会析构" << std::endl;
    std::cout << "• 析构函数是 noexcept 的，不会抛出异常" << std::endl;
    std::cout << "• 析构顺序：先析构成员变量，再执行析构函数体" << std::endl;
    
    return 0;
}
