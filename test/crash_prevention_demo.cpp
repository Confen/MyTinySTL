#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>
#include <exception>
#include <signal.h>

// 方法1：使用异常处理跳过崩溃
class SafeBadExample {
private:
    std::string* data;
    size_t size;
    bool is_valid;  // 添加有效性标志
    
public:
    SafeBadExample(const std::string& str) : size(str.length()), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = str[i];
            }
            std::cout << "构造 SafeBadExample: " << str << std::endl;
        } catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "构造失败，设置为无效状态" << std::endl;
        }
    }
    
    SafeBadExample(const SafeBadExample& other) : size(other.size), is_valid(true) {
        try {
            data = new std::string[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            std::cout << "复制构造 SafeBadExample" << std::endl;
        } catch (...) {
            data = nullptr;
            is_valid = false;
            std::cout << "复制构造失败，设置为无效状态" << std::endl;
        }
    }
    
    // 安全的析构函数
    ~SafeBadExample() {
        if (is_valid && data != nullptr) {
            try {
                delete[] data;
                std::cout << "析构 SafeBadExample" << std::endl;
            } catch (...) {
                std::cout << "析构时发生异常，但已安全处理" << std::endl;
            }
        } else {
            std::cout << "跳过析构（无效状态）" << std::endl;
        }
        data = nullptr;  // 防止重复释放
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

// 方法2：使用智能指针避免手动内存管理（C++11兼容版本）
class SmartPointerExample {
private:
    std::unique_ptr<std::string[]> data;
    size_t size;
    
public:
    SmartPointerExample(const std::string& str) : size(str.length()) {
        data.reset(new std::string[size]);
        for (size_t i = 0; i < size; ++i) {
            data[i] = str[i];
        }
        std::cout << "构造 SmartPointerExample: " << str << std::endl;
    }
    
    SmartPointerExample(const SmartPointerExample& other) : size(other.size) {
        data.reset(new std::string[size]);
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        std::cout << "复制构造 SmartPointerExample" << std::endl;
    }
    
    SmartPointerExample& operator=(const SmartPointerExample& other) {
        if (this != &other) {
            size = other.size;
            data.reset(new std::string[size]);
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            std::cout << "赋值 SmartPointerExample" << std::endl;
        }
        return *this;
    }
    
    // 析构函数由智能指针自动处理，不会崩溃
    ~SmartPointerExample() {
        std::cout << "析构 SmartPointerExample（智能指针自动管理）" << std::endl;
    }
    
    void print() const {
        std::cout << "内容: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
    }
};

// 方法3：使用RAII和容器
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

// 方法4：信号处理函数
void signal_handler(int signal) {
    std::cout << "\n捕获到信号 " << signal << "，程序安全退出" << std::endl;
    exit(0);
}

// 方法5：异常处理包装
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
    
    // 设置信号处理
    signal(SIGSEGV, signal_handler);  // 段错误
    signal(SIGABRT, signal_handler);  // 中止信号
    
    std::cout << "=== 析构函数崩溃预防方法演示 ===" << std::endl;
    
    // 方法1：异常处理
    safe_execute([]() {
        SafeBadExample obj1("Hello");
        SafeBadExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;  // 使用默认的赋值运算符
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
    }, "方法1：异常处理");
    
    // 方法2：智能指针
    safe_execute([]() {
        SmartPointerExample obj1("Hello");
        SmartPointerExample obj2("World");
        
        std::cout << "赋值前：" << std::endl;
        obj1.print();
        obj2.print();
        
        std::cout << "\n执行赋值：obj1 = obj2" << std::endl;
        obj1 = obj2;
        
        std::cout << "赋值后：" << std::endl;
        obj1.print();
        obj2.print();
    }, "方法2：智能指针");
    
    // 方法3：RAII
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
    }, "方法3：RAII");
    
    std::cout << "\n=== 崩溃预防方法总结 ===" << std::endl;
    std::cout << "1. ✅ 异常处理：在析构函数中使用try-catch" << std::endl;
    std::cout << "2. ✅ 智能指针：使用unique_ptr/shared_ptr自动管理内存" << std::endl;
    std::cout << "3. ✅ RAII：使用容器类自动管理资源" << std::endl;
    std::cout << "4. ✅ 信号处理：捕获系统信号并安全退出" << std::endl;
    std::cout << "5. ✅ 有效性检查：添加状态标志避免重复操作" << std::endl;
    std::cout << "6. ✅ 空指针检查：在释放前检查指针是否为空" << std::endl;
    
    std::cout << "\n=== 最佳实践建议 ===" << std::endl;
    std::cout << "🎯 推荐使用智能指针或RAII，避免手动内存管理" << std::endl;
    std::cout << "🎯 如果必须手动管理，确保遵循三法则/五法则" << std::endl;
    std::cout << "🎯 在析构函数中添加异常处理" << std::endl;
    std::cout << "🎯 使用有效性标志避免重复操作" << std::endl;
    
    return 0;
}
