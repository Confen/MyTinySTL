#include <iostream>
#include <string>
#include <windows.h>

// 测试类 - 用于观察析构函数调用
class TestClass {
public:
    size_t size;
    std::string data;
    bool is_valid;

    TestClass(const std::string& str) : data(str), size(str.length()), is_valid(true) {
        std::cout << "构造 TestClass: " << str << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass: " << data << std::endl;
    }
    
    void print() const {
        std::cout << "TestClass - size: " << size << ", data: " << data << ", is_valid: " << is_valid << std::endl;
    }
};

// 问题代码的修正版本
class SafeBadExample {
private:
    std::string* data;
    size_t size;
    bool is_valid;

public:
    // 构造函数
    SafeBadExample(const std::string& str) : size(str.length()), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = str[i];  // 这里有问题！
            }
            std::cout << "构造 SafeBadExample: " << str << std::endl;
        }
        catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "构造失败，设置为无效状态" << std::endl;
        }
    }

    // 复制构造函数 - 问题代码
    SafeBadExample(const TestClass& other) : size(other.size), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];  // 这里有问题！
            }
            std::cout << "复制构造 SafeBadExample" << std::endl;
        }
        catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "复制构造失败，设置为无效状态" << std::endl;
        }
    }

    ~SafeBadExample() {
        if (is_valid && data != nullptr) {
            try {
                delete[] data;
                std::cout << "析构 SafeBadExample" << std::endl;
            }
            catch (...) {
                std::cout << "析构时发生异常，但已安全处理" << std::endl;
            }
        }
        else {
            std::cout << "跳过析构（无效状态）" << std::endl;
        }
        data = nullptr;
    }

    void print() const {
        if (is_valid && data != nullptr) {
            std::cout << "SafeBadExample - size: " << size << ", data: [";
            for (size_t i = 0; i < size; ++i) {
                std::cout << data[i];
                if (i < size - 1) std::cout << ", ";
            }
            std::cout << "], is_valid: " << is_valid << std::endl;
        }
        else {
            std::cout << "SafeBadExample - 无效状态" << std::endl;
        }
    }
};

// 修正版本
class SafeGoodExample {
private:
    std::string* data;
    size_t size;
    bool is_valid;

public:
    // 构造函数
    SafeGoodExample(const std::string& str) : size(str.length()), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = std::string(1, str[i]);  // 修正：创建单个字符的字符串
            }
            std::cout << "构造 SafeGoodExample: " << str << std::endl;
        }
        catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "构造失败，设置为无效状态" << std::endl;
        }
    }

    // 复制构造函数 - 修正版本
    SafeGoodExample(const TestClass& other) : size(other.size), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = std::string(1, other.data[i]);  // 修正：创建单个字符的字符串
            }
            std::cout << "复制构造 SafeGoodExample" << std::endl;
        }
        catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "复制构造失败，设置为无效状态" << std::endl;
        }
    }

    ~SafeGoodExample() {
        if (is_valid && data != nullptr) {
            try {
                delete[] data;
                std::cout << "析构 SafeGoodExample" << std::endl;
            }
            catch (...) {
                std::cout << "析构时发生异常，但已安全处理" << std::endl;
            }
        }
        else {
            std::cout << "跳过析构（无效状态）" << std::endl;
        }
        data = nullptr;
    }

    void print() const {
        if (is_valid && data != nullptr) {
            std::cout << "SafeGoodExample - size: " << size << ", data: [";
            for (size_t i = 0; i < size; ++i) {
                std::cout << data[i];
                if (i < size - 1) std::cout << ", ";
            }
            std::cout << "], is_valid: " << is_valid << std::endl;
        }
        else {
            std::cout << "SafeGoodExample - 无效状态" << std::endl;
        }
    }
};

// 更好的设计版本
class BetterExample {
private:
    std::string data;  // 直接使用 std::string，不需要手动管理内存
    bool is_valid;

public:
    BetterExample(const std::string& str) : data(str), is_valid(true) {
        std::cout << "构造 BetterExample: " << str << std::endl;
    }

    BetterExample(const TestClass& other) : data(other.data), is_valid(true) {
        std::cout << "复制构造 BetterExample" << std::endl;
    }

    ~BetterExample() {
        std::cout << "析构 BetterExample: " << data << std::endl;
        // std::string 会自动析构，不需要手动管理
    }

    void print() const {
        std::cout << "BetterExample - data: " << data << ", is_valid: " << is_valid << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 复制构造失败原因分析 ===" << std::endl;
    
    std::cout << "\n📋 问题分析：" << std::endl;
    std::cout << "1. 类型不匹配问题" << std::endl;
    std::cout << "2. 内存管理问题" << std::endl;
    std::cout << "3. 设计问题" << std::endl;
    
    // 测试1：问题代码
    std::cout << "\n🔹 测试1：问题代码" << std::endl;
    std::cout << "问题：data[i] = other.data[i];" << std::endl;
    std::cout << "other.data[i] 是 char 类型，data[i] 是 std::string 类型" << std::endl;
    std::cout << "char 不能直接赋值给 std::string" << std::endl;
    
    {
        TestClass testObj("Hello");
        testObj.print();
        
        // 这里会编译失败
        // SafeBadExample badObj(testObj);  // 编译错误
        std::cout << "SafeBadExample 复制构造会编译失败" << std::endl;
    }
    
    // 测试2：修正版本
    std::cout << "\n🔹 测试2：修正版本" << std::endl;
    std::cout << "修正：data[i] = std::string(1, other.data[i]);" << std::endl;
    std::cout << "将 char 转换为 std::string" << std::endl;
    
    {
        TestClass testObj("Hello");
        testObj.print();
        
        SafeGoodExample goodObj(testObj);
        goodObj.print();
        std::cout << "goodObj 即将析构..." << std::endl;
    }
    
    // 测试3：更好的设计
    std::cout << "\n🔹 测试3：更好的设计" << std::endl;
    std::cout << "直接使用 std::string，不需要手动管理内存" << std::endl;
    
    {
        TestClass testObj("World");
        testObj.print();
        
        BetterExample betterObj(testObj);
        betterObj.print();
        std::cout << "betterObj 即将析构..." << std::endl;
    }
    
    // 详细分析
    std::cout << "\n📊 复制构造失败的具体原因：" << std::endl;
    
    std::cout << "\n1. 类型不匹配：" << std::endl;
    std::cout << "   other.data[i] 是 char 类型" << std::endl;
    std::cout << "   data[i] 是 std::string 类型" << std::endl;
    std::cout << "   char 不能直接赋值给 std::string" << std::endl;
    
    std::cout << "\n2. 内存管理问题：" << std::endl;
    std::cout << "   data = new std::string[size];  // 分配了 size 个 std::string 对象" << std::endl;
    std::cout << "   data[i] = other.data[i];      // 试图将 char 赋值给 std::string" << std::endl;
    std::cout << "   这会导致编译错误" << std::endl;
    
    std::cout << "\n3. 设计问题：" << std::endl;
    std::cout << "   为什么要将字符串拆分成字符数组？" << std::endl;
    std::cout << "   直接使用 std::string 更简单、更安全" << std::endl;
    
    std::cout << "\n🔧 修正方法：" << std::endl;
    
    std::cout << "\n方法1：类型转换" << std::endl;
    std::cout << "   data[i] = std::string(1, other.data[i]);" << std::endl;
    std::cout << "   将 char 转换为 std::string" << std::endl;
    
    std::cout << "\n方法2：重新设计" << std::endl;
    std::cout << "   class BetterExample {" << std::endl;
    std::cout << "   private:" << std::endl;
    std::cout << "       std::string data;  // 直接使用 std::string" << std::endl;
    std::cout << "   };" << std::endl;
    std::cout << "   不需要手动管理内存" << std::endl;
    
    std::cout << "\n方法3：使用标准容器" << std::endl;
    std::cout << "   std::vector<std::string> data;" << std::endl;
    std::cout << "   自动管理内存" << std::endl;
    
    std::cout << "\n⚠️ 重要注意事项：" << std::endl;
    std::cout << "1. 类型匹配是 C++ 的基本要求" << std::endl;
    std::cout << "2. 手动内存管理容易出错" << std::endl;
    std::cout << "3. 优先使用 RAII 和智能指针" << std::endl;
    std::cout << "4. 避免不必要的复杂性" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 复制构造失败的原因：" << std::endl;
    std::cout << "• 类型不匹配：char 不能赋值给 std::string" << std::endl;
    std::cout << "• 内存管理：手动管理容易出错" << std::endl;
    std::cout << "• 设计问题：过度复杂化" << std::endl;
    std::cout << "• 解决方案：使用类型转换或重新设计" << std::endl;
    
    return 0;
}




