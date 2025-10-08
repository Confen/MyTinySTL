#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

// 方法1：使用异常处理跳过崩溃
class SafeExample {
private:
    std::string* data;
    size_t size;
    bool is_valid;
    
public:
    SafeExample(const std::string& str) : size(str.length()), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = str[i];
            }
            std::cout << "构造 SafeExample: " << str << std::endl;
        } catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "构造失败，设置为无效状态" << std::endl;
        }
    }
    
    SafeExample(const SafeExample& other) : size(other.size), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            std::cout << "复制构造 SafeExample" << std::endl;
        } catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "复制构造失败，设置为无效状态" << std::endl;
        }
    }
    
    // 安全的析构函数
    ~SafeExample() {
        if (is_valid && data != nullptr) {
            try {
                delete[] data;
                std::cout << "析构 SafeExample" << std::endl;
            } catch (...) {
                std::cout << "析构时发生异常，但已安全处理" << std::endl;
            }
        } else {
            std::cout << "跳过析构（无效状态）" << std::endl;
        }
        data = nullptr;
    }
    
    void print() const {
        if (!is_valid || data == nullptr) {
            std::cout << "对象处于无效状态" << std::endl;
            return;
        }
        std::cout << "内容: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
    }
};

// 方法2：使用RAII和容器（推荐）
class RAIIExample {
private:
    std::vector<std::string> data;
    
public:
    RAIIExample(const std::string& str) {
        for (char c : str) {
            data.push_back(std::string(1, c));
        }
        std::cout << "构造 RAIIExample: " << str << std::endl;
    }
    
    RAIIExample(const RAIIExample& other) = default;
    RAIIExample& operator=(const RAIIExample& other) = default;
    
    // 析构函数由vector自动处理，不会崩溃
    ~RAIIExample() {
        std::cout << "析构 RAIIExample（vector自动管理）" << std::endl;
    }
    
    void print() const {
        std::cout << "内容: ";
        for (const auto& str : data) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
};

// 方法3：异常处理包装
template<typename Func>
void safe_execute(Func func, const std::string& description) {
    try {
        std::cout << "\n=== " << description << " ===" << std::endl;
        func();
        std::cout << "✅ " << description << " 执行成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "❌ " << description << " 发生异常: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "❌ " << description << " 发生未知异常" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 析构函数崩溃预防方法演示 ===" << std::endl;
    
    // 方法1：异常处理
    safe_execute([]() {
        SafeExample obj1("Hello");
        SafeExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;  // 使用默认的赋值运算符
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
    }, "方法1：异常处理");
    
    // 方法2：RAII
    safe_execute([]() {
        RAIIExample obj1("Hello");
        RAIIExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
    }, "方法2：RAII");
    
    std::cout << "\n=== 崩溃预防方法总结 ===" << std::endl;
    std::cout << "1. ✅ 异常处理：在析构函数中使用try-catch" << std::endl;
    std::cout << "2. ✅ RAII：使用容器类自动管理资源" << std::endl;
    std::cout << "3. ✅ 有效性检查：添加状态标志避免重复操作" << std::endl;
    std::cout << "4. ✅ 空指针检查：在释放前检查指针是否为空" << std::endl;
    
    std::cout << "\n=== 最佳实践建议 ===" << std::endl;
    std::cout << "🎯 推荐使用RAII，避免手动内存管理" << std::endl;
    std::cout << "🎯 如果必须手动管理，确保遵循三法则/五法则" << std::endl;
    std::cout << "🎯 在析构函数中添加异常处理" << std::endl;
    std::cout << "🎯 使用有效性标志避免重复操作" << std::endl;
    
    return 0;
}
