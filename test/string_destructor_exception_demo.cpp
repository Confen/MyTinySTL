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

// 测试类 - 包含 string 成员变量
class StringContainer {
private:
    std::string stringMember1;
    std::string stringMember2;
    std::string stringMember3;
    
public:
    StringContainer() {
        std::cout << "构造 StringContainer" << std::endl;
        
        stringMember1 = "第一个字符串成员";
        stringMember2 = "第二个字符串成员";
        stringMember3 = "第三个字符串成员";
        
        std::cout << "StringContainer 构造完成" << std::endl;
    }
    
    ~StringContainer() {
        std::cout << "析构 StringContainer" << std::endl;
        
        // string 成员变量会自动析构，不需要手动操作
        std::cout << "string 成员变量自动析构..." << std::endl;
        
        std::cout << "StringContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringContainer 内容:" << std::endl;
        std::cout << "stringMember1: " << stringMember1 << std::endl;
        std::cout << "stringMember2: " << stringMember2 << std::endl;
        std::cout << "stringMember3: " << stringMember3 << std::endl;
    }
};

// 测试类 - 包含 string 数组
class StringArrayContainer {
private:
    std::string* stringArray;
    size_t arraySize;
    
public:
    StringArrayContainer(size_t size) : arraySize(size) {
        std::cout << "构造 StringArrayContainer" << std::endl;
        
        stringArray = new std::string[arraySize];
        for (size_t i = 0; i < arraySize; ++i) {
            stringArray[i] = "字符串数组元素 " + std::to_string(i);
        }
        
        std::cout << "StringArrayContainer 构造完成" << std::endl;
    }
    
    ~StringArrayContainer() {
        std::cout << "析构 StringArrayContainer" << std::endl;
        
        // delete[] 会自动调用每个 string 的析构函数
        delete[] stringArray;
        
        std::cout << "StringArrayContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringArrayContainer 内容:" << std::endl;
        for (size_t i = 0; i < arraySize; ++i) {
            std::cout << "stringArray[" << i << "]: " << stringArray[i] << std::endl;
        }
    }
};

// 测试类 - 包含 string 向量
class StringVectorContainer {
private:
    std::vector<std::string> stringVector;
    
public:
    StringVectorContainer() {
        std::cout << "构造 StringVectorContainer" << std::endl;
        
        stringVector.emplace_back("向量字符串1");
        stringVector.emplace_back("向量字符串2");
        stringVector.emplace_back("向量字符串3");
        
        std::cout << "StringVectorContainer 构造完成" << std::endl;
    }
    
    ~StringVectorContainer() {
        std::cout << "析构 StringVectorContainer" << std::endl;
        
        // vector 会自动析构，包括其中的 string 元素
        std::cout << "string 向量自动析构..." << std::endl;
        
        std::cout << "StringVectorContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringVectorContainer 内容:" << std::endl;
        for (size_t i = 0; i < stringVector.size(); ++i) {
            std::cout << "stringVector[" << i << "]: " << stringVector[i] << std::endl;
        }
    }
};

// 测试类 - 包含 string 智能指针
class StringSmartPtrContainer {
private:
    std::unique_ptr<std::string> stringSmartPtr1;
    std::unique_ptr<std::string> stringSmartPtr2;
    
public:
    StringSmartPtrContainer() {
        std::cout << "构造 StringSmartPtrContainer" << std::endl;
        
        stringSmartPtr1 = std::unique_ptr<std::string>(new std::string("智能指针字符串1"));
        stringSmartPtr2 = std::unique_ptr<std::string>(new std::string("智能指针字符串2"));
        
        std::cout << "StringSmartPtrContainer 构造完成" << std::endl;
    }
    
    ~StringSmartPtrContainer() {
        std::cout << "析构 StringSmartPtrContainer" << std::endl;
        
        // 智能指针会自动析构，包括其中的 string
        std::cout << "string 智能指针自动析构..." << std::endl;
        
        std::cout << "StringSmartPtrContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringSmartPtrContainer 内容:" << std::endl;
        std::cout << "stringSmartPtr1: " << *stringSmartPtr1 << std::endl;
        std::cout << "stringSmartPtr2: " << *stringSmartPtr2 << std::endl;
    }
};

// 测试类 - 包含 string 引用
class StringReferenceContainer {
private:
    std::string& stringRef;
    
public:
    StringReferenceContainer(std::string& ref) : stringRef(ref) {
        std::cout << "构造 StringReferenceContainer" << std::endl;
        std::cout << "StringReferenceContainer 构造完成" << std::endl;
    }
    
    ~StringReferenceContainer() {
        std::cout << "析构 StringReferenceContainer" << std::endl;
        
        // 引用不会析构，因为它不拥有对象
        std::cout << "string 引用不会析构（不拥有对象）..." << std::endl;
        
        std::cout << "StringReferenceContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringReferenceContainer 内容:" << std::endl;
        std::cout << "stringRef: " << stringRef << std::endl;
    }
};

// 测试类 - 包含 string 指针
class StringPointerContainer {
private:
    std::string* stringPtr;
    
public:
    StringPointerContainer() {
        std::cout << "构造 StringPointerContainer" << std::endl;
        
        stringPtr = new std::string("指针字符串");
        
        std::cout << "StringPointerContainer 构造完成" << std::endl;
    }
    
    ~StringPointerContainer() {
        std::cout << "析构 StringPointerContainer" << std::endl;
        
        // 必须手动删除指针指向的 string
        delete stringPtr;
        
        std::cout << "StringPointerContainer 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "StringPointerContainer 内容:" << std::endl;
        std::cout << "stringPtr: " << *stringPtr << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== string 类型的析构和异常安全性 ===" << std::endl;
    
    std::cout << "\n📋 核心问题：" << std::endl;
    std::cout << "string 类型是否一定自动析构？" << std::endl;
    std::cout << "string 类型是否受异常检测影响？" << std::endl;
    
    // 测试1：string 成员变量
    std::cout << "\n🔹 测试1：string 成员变量" << std::endl;
    std::cout << "string 作为成员变量会自动析构" << std::endl;
    
    {
        StringContainer container;
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringContainer 的析构函数
    
    // 测试2：string 数组
    std::cout << "\n🔹 测试2：string 数组" << std::endl;
    std::cout << "string 数组会自动析构每个元素" << std::endl;
    
    {
        StringArrayContainer container(3);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringArrayContainer 的析构函数
    
    // 测试3：string 向量
    std::cout << "\n🔹 测试3：string 向量" << std::endl;
    std::cout << "vector<string> 会自动析构每个元素" << std::endl;
    
    {
        StringVectorContainer container;
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringVectorContainer 的析构函数
    
    // 测试4：string 智能指针
    std::cout << "\n🔹 测试4：string 智能指针" << std::endl;
    std::cout << "unique_ptr<string> 会自动析构" << std::endl;
    
    {
        StringSmartPtrContainer container;
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringSmartPtrContainer 的析构函数
    
    // 测试5：string 引用
    std::cout << "\n🔹 测试5：string 引用" << std::endl;
    std::cout << "string& 不会析构（不拥有对象）" << std::endl;
    
    {
        std::string externalString = "外部字符串";
        StringReferenceContainer container(externalString);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringReferenceContainer 的析构函数
    
    // 测试6：string 指针
    std::cout << "\n🔹 测试6：string 指针" << std::endl;
    std::cout << "string* 必须手动删除" << std::endl;
    
    {
        StringPointerContainer container;
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 StringPointerContainer 的析构函数
    
    // 测试7：直接 string 对象
    std::cout << "\n🔹 测试7：直接 string 对象" << std::endl;
    std::cout << "直接 string 对象会自动析构" << std::endl;
    
    {
        std::string directString = "直接字符串对象";
        std::cout << "directString: " << directString << std::endl;
        std::cout << "directString 即将析构..." << std::endl;
    }  // 这里会调用 string 的析构函数
    
    // 测试8：string 异常安全性
    std::cout << "\n🔹 测试8：string 异常安全性" << std::endl;
    std::cout << "string 析构函数是 noexcept 的" << std::endl;
    
    {
        class ExceptionSafeStringContainer {
        private:
            std::string stringMember;
            
        public:
            ExceptionSafeStringContainer() {
                std::cout << "构造 ExceptionSafeStringContainer" << std::endl;
                stringMember = "异常安全字符串";
            }
            
            ~ExceptionSafeStringContainer() {
                std::cout << "析构 ExceptionSafeStringContainer" << std::endl;
                
                // string 析构函数不会抛出异常
                // 即使容器析构时抛出异常，string 也会被正确析构
                std::cout << "string 析构函数是 noexcept 的" << std::endl;
            }
        };
        
        ExceptionSafeStringContainer container;
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 ExceptionSafeStringContainer 的析构函数
    
    // 总结
    std::cout << "\n📊 string 类型析构总结：" << std::endl;
    
    std::cout << "\n1. string 成员变量：" << std::endl;
    std::cout << "   std::string stringMember;" << std::endl;
    std::cout << "   // 自动析构，不需要手动操作" << std::endl;
    
    std::cout << "\n2. string 数组：" << std::endl;
    std::cout << "   std::string* stringArray = new std::string[5];" << std::endl;
    std::cout << "   delete[] stringArray;  // 自动调用每个 string 的析构函数" << std::endl;
    
    std::cout << "\n3. string 向量：" << std::endl;
    std::cout << "   std::vector<std::string> stringVector;" << std::endl;
    std::cout << "   // 自动析构，包括其中的 string 元素" << std::endl;
    
    std::cout << "\n4. string 智能指针：" << std::endl;
    std::cout << "   std::unique_ptr<std::string> stringPtr;" << std::endl;
    std::cout << "   // 自动析构，包括其中的 string" << std::endl;
    
    std::cout << "\n5. string 引用：" << std::endl;
    std::cout << "   std::string& stringRef;" << std::endl;
    std::cout << "   // 不会析构（不拥有对象）" << std::endl;
    
    std::cout << "\n6. string 指针：" << std::endl;
    std::cout << "   std::string* stringPtr = new std::string;" << std::endl;
    std::cout << "   delete stringPtr;  // 必须手动删除" << std::endl;
    
    std::cout << "\n⚠️ 重要注意事项：" << std::endl;
    std::cout << "1. string 析构函数是 noexcept 的，不会抛出异常" << std::endl;
    std::cout << "2. string 作为成员变量会自动析构" << std::endl;
    std::cout << "3. string 数组会自动析构每个元素" << std::endl;
    std::cout << "4. string 向量会自动析构每个元素" << std::endl;
    std::cout << "5. string 智能指针会自动析构" << std::endl;
    std::cout << "6. string 引用不会析构（不拥有对象）" << std::endl;
    std::cout << "7. string 指针必须手动删除" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 string 类型的析构：" << std::endl;
    std::cout << "• 作为成员变量 → 自动析构" << std::endl;
    std::cout << "• 作为数组元素 → 自动析构" << std::endl;
    std::cout << "• 作为向量元素 → 自动析构" << std::endl;
    std::cout << "• 作为智能指针 → 自动析构" << std::endl;
    std::cout << "• 作为引用 → 不会析构" << std::endl;
    std::cout << "• 作为指针 → 必须手动删除" << std::endl;
    std::cout << "• 析构函数是 noexcept 的，不会抛出异常" << std::endl;
    
    return 0;
}
