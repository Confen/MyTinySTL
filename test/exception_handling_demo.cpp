#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

// 模拟会抛出异常的类
class ExpensiveResource {
private:
    int id_;
    std::string name_;

public:
    ExpensiveResource(int id, const std::string& name) : id_(id), name_(name) {
        std::cout << "创建资源 " << id_ << ": " << name_ << std::endl;
        
        // 模拟创建失败
        if (id_ == 3) {
            throw std::runtime_error("资源 " + std::to_string(id_) + " 创建失败");
        }
    }
    
    ~ExpensiveResource() {
        std::cout << "销毁资源 " << id_ << ": " << name_ << std::endl;
    }
    
    int getId() const { return id_; }
    std::string getName() const { return name_; }
};

// 强异常安全的容器
class SafeContainer {
private:
    std::vector<ExpensiveResource> resources_;

public:
    // 强异常安全的插入操作
    void insertResource(int id, const std::string& name) {
        std::cout << "尝试插入资源 " << id << ": " << name << std::endl;
        
        // 先创建临时对象
        ExpensiveResource temp(id, name);
        
        // 如果创建成功，再添加到容器
        resources_.push_back(std::move(temp));
        
        std::cout << "资源插入成功！" << std::endl;
    }
    
    void printResources() const {
        std::cout << "容器中的资源: ";
        for (const auto& resource : resources_) {
            std::cout << resource.getId() << " ";
        }
        std::cout << std::endl;
    }
    
    size_t size() const { return resources_.size(); }
};

// 演示1：用户正确处理异常
void demonstrate_correct_usage() {
    std::cout << "=== 正确的异常处理方式 ===" << std::endl;
    
    SafeContainer container;
    
    // 用户必须使用 try-catch
    try {
        container.insertResource(1, "Resource1");
        std::cout << "操作1成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "操作1失败: " << e.what() << std::endl;
    }
    
    try {
        container.insertResource(2, "Resource2");
        std::cout << "操作2成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "操作2失败: " << e.what() << std::endl;
    }
    
    try {
        container.insertResource(3, "Resource3");  // 这个会失败
        std::cout << "操作3成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "操作3失败: " << e.what() << std::endl;
        std::cout << "用户知道操作失败了，可以采取相应措施" << std::endl;
    }
    
    try {
        container.insertResource(4, "Resource4");
        std::cout << "操作4成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "操作4失败: " << e.what() << std::endl;
    }
    
    container.printResources();
}

// 演示2：用户不处理异常（程序崩溃）
void demonstrate_no_exception_handling() {
    std::cout << "\n=== 用户不处理异常（程序崩溃）===" << std::endl;
    
    SafeContainer container;
    
    std::cout << "用户没有使用 try-catch，程序会崩溃：" << std::endl;
    
    try {
        container.insertResource(1, "Resource1");
        container.insertResource(2, "Resource2");
        container.insertResource(3, "Resource3");  // 这里会抛出异常
        container.insertResource(4, "Resource4");  // 这行不会执行
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "程序没有崩溃，但用户必须处理这个异常" << std::endl;
    }
    
    container.printResources();
}

// 演示3：用户的不同处理策略
void demonstrate_different_strategies() {
    std::cout << "\n=== 用户的不同处理策略 ===" << std::endl;
    
    SafeContainer container;
    
    // 策略1：重试
    std::cout << "策略1：重试机制" << std::endl;
    for (int attempt = 1; attempt <= 3; ++attempt) {
        try {
            container.insertResource(3, "Resource3");
            std::cout << "重试成功！" << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cout << "重试 " << attempt << " 失败: " << e.what() << std::endl;
            if (attempt == 3) {
                std::cout << "重试次数用完，放弃操作" << std::endl;
            }
        }
    }
    
    // 策略2：使用替代方案
    std::cout << "\n策略2：使用替代方案" << std::endl;
    try {
        container.insertResource(3, "Resource3");
    } catch (const std::exception& e) {
        std::cout << "原方案失败: " << e.what() << std::endl;
        std::cout << "使用替代方案：插入默认资源" << std::endl;
        try {
            container.insertResource(99, "DefaultResource");
        } catch (const std::exception& e2) {
            std::cout << "替代方案也失败: " << e2.what() << std::endl;
        }
    }
    
    // 策略3：记录错误并继续
    std::cout << "\n策略3：记录错误并继续" << std::endl;
    try {
        container.insertResource(3, "Resource3");
    } catch (const std::exception& e) {
        std::cout << "记录错误日志: " << e.what() << std::endl;
        std::cout << "继续执行其他操作" << std::endl;
    }
    
    container.printResources();
}

// 演示4：强异常安全的好处
void demonstrate_benefits() {
    std::cout << "\n=== 强异常安全的好处 ===" << std::endl;
    
    SafeContainer container;
    
    // 先添加一些资源
    try {
        container.insertResource(1, "Resource1");
        container.insertResource(2, "Resource2");
    } catch (const std::exception& e) {
        std::cout << "初始添加失败: " << e.what() << std::endl;
    }
    
    std::cout << "初始状态: ";
    container.printResources();
    
    // 尝试添加会失败的资源
    try {
        container.insertResource(3, "Resource3");  // 会失败
    } catch (const std::exception& e) {
        std::cout << "添加失败: " << e.what() << std::endl;
        std::cout << "强异常安全保证：容器状态没有改变" << std::endl;
    }
    
    std::cout << "失败后状态: ";
    container.printResources();
    
    std::cout << "容器大小: " << container.size() << " (没有变化)" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demonstrate_correct_usage();
    demonstrate_no_exception_handling();
    demonstrate_different_strategies();
    demonstrate_benefits();
    
    return 0;
}



















