#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include <windows.h>

// 自定义异常类
class MyCustomException : public std::exception {
private:
    std::string message_;
public:
    explicit MyCustomException(const std::string& msg) : message_(msg) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

void demonstrate_std_exception() {
    std::cout << "=== std::exception 演示 ===" << std::endl;
    
    // 1. 基本异常对象
    std::cout << "1. 基本异常对象:" << std::endl;
    std::exception ex1;
    std::cout << "   默认异常消息: '" << ex1.what() << "'" << std::endl;
    
    // 2. 标准异常类
    std::cout << "\n2. 标准异常类:" << std::endl;
    
    try {
        throw std::runtime_error("运行时错误");
    } catch (const std::exception& e) {
        std::cout << "   捕获 std::runtime_error: " << e.what() << std::endl;
    }
    
    try {
        throw std::logic_error("逻辑错误");
    } catch (const std::exception& e) {
        std::cout << "   捕获 std::logic_error: " << e.what() << std::endl;
    }
    
    try {
        throw std::invalid_argument("无效参数");
    } catch (const std::exception& e) {
        std::cout << "   捕获 std::invalid_argument: " << e.what() << std::endl;
    }
    
    try {
        throw std::out_of_range("越界访问");
    } catch (const std::exception& e) {
        std::cout << "   捕获 std::out_of_range: " << e.what() << std::endl;
    }
}

void demonstrate_exception_hierarchy() {
    std::cout << "\n=== 异常层次结构演示 ===" << std::endl;
    
    // 1. 多态捕获
    std::cout << "1. 多态捕获:" << std::endl;
    
    auto test_exception = [](const std::exception& e) {
        std::cout << "   异常类型: " << typeid(e).name() << std::endl;
        std::cout << "   异常消息: " << e.what() << std::endl;
        
        // 尝试动态转换
        if (const std::runtime_error* re = dynamic_cast<const std::runtime_error*>(&e)) {
            std::cout << "   这是 runtime_error 类型" << std::endl;
        } else if (const std::logic_error* le = dynamic_cast<const std::logic_error*>(&e)) {
            std::cout << "   这是 logic_error 类型" << std::endl;
        }
    };
    
    try {
        throw std::runtime_error("测试运行时错误");
    } catch (const std::exception& e) {
        test_exception(e);
    }
    
    try {
        throw std::logic_error("测试逻辑错误");
    } catch (const std::exception& e) {
        test_exception(e);
    }
}

void demonstrate_custom_exception() {
    std::cout << "\n=== 自定义异常类演示 ===" << std::endl;
    
    // 1. 自定义异常
    std::cout << "1. 自定义异常:" << std::endl;
    try {
        throw MyCustomException("这是我的自定义异常");
    } catch (const std::exception& e) {
        std::cout << "   捕获自定义异常: " << e.what() << std::endl;
        std::cout << "   异常类型: " << typeid(e).name() << std::endl;
    }
    
    // 2. 异常链
    std::cout << "\n2. 异常链:" << std::endl;
    try {
        try {
            throw std::runtime_error("原始异常");
        } catch (const std::exception& e) {
            std::cout << "   捕获原始异常: " << e.what() << std::endl;
            throw MyCustomException("包装后的异常: " + std::string(e.what()));
        }
    } catch (const std::exception& e) {
        std::cout << "   捕获包装异常: " << e.what() << std::endl;
    }
}

void demonstrate_exception_safety() {
    std::cout << "\n=== 异常安全性演示 ===" << std::endl;
    
    class ResourceManager {
        int* data;
        size_t size;
    public:
        ResourceManager(size_t n) : size(n) {
            data = new int[n];
            if (n > 1000) {
                delete[] data;
                throw std::runtime_error("资源大小超过限制");
            }
        }
        
        ~ResourceManager() {
            delete[] data;
        }
        
        size_t getSize() const { return size; }
    };
    
    std::cout << "1. 异常安全测试:" << std::endl;
    try {
        ResourceManager rm(100);
        std::cout << "   资源创建成功，大小: " << rm.getSize() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
    
    try {
        ResourceManager rm(2000);  // 会抛出异常
        std::cout << "   资源创建成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   异常安全：资源已正确清理" << std::endl;
    }
}

void demonstrate_exception_handling() {
    std::cout << "\n=== 异常处理演示 ===" << std::endl;
    
    // 1. 基本异常处理
    std::cout << "1. 基本异常处理:" << std::endl;
    try {
        throw std::runtime_error("测试异常");
    } catch (const std::runtime_error& e) {
        std::cout << "   捕获 runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获 exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "   捕获未知异常" << std::endl;
    }
    
    // 2. 异常重新抛出
    std::cout << "\n2. 异常重新抛出:" << std::endl;
    try {
        try {
            throw std::runtime_error("原始异常");
        } catch (const std::exception& e) {
            std::cout << "   捕获异常: " << e.what() << std::endl;
            throw;  // 重新抛出
        }
    } catch (const std::exception& e) {
        std::cout << "   重新捕获异常: " << e.what() << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== std::exception 详细演示 ===" << std::endl;
    
    demonstrate_std_exception();
    demonstrate_exception_hierarchy();
    demonstrate_custom_exception();
    demonstrate_exception_safety();
    demonstrate_exception_handling();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}




















