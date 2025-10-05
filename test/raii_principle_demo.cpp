#include <iostream>
#include <memory>
#include <fstream>
#include <mutex>
#include <windows.h>

// 演示1：基本 RAII 原则
class BasicRAII {
private:
    int* data_;
    std::string name_;
    
public:
    // 构造函数：获取资源
    BasicRAII(int size, const std::string& n) : name_(n) {
        data_ = new int[size];
        for (int i = 0; i < size; ++i) {
            data_[i] = i;
        }
        std::cout << "RAII: 获取资源 - " << name_ << std::endl;
        std::cout << "  分配内存: " << data_ << std::endl;
    }
    
    // 析构函数：释放资源
    ~BasicRAII() {
        if (data_) {
            std::cout << "RAII: 释放资源 - " << name_ << std::endl;
            std::cout << "  释放内存: " << data_ << std::endl;
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
};

// 演示2：文件资源管理
class FileRAII {
private:
    std::ofstream file_;
    std::string filename_;
    
public:
    // 构造函数：打开文件
    FileRAII(const std::string& filename) : filename_(filename) {
        file_.open(filename);
        if (file_.is_open()) {
            std::cout << "RAII: 打开文件 - " << filename_ << std::endl;
        } else {
            std::cout << "RAII: 无法打开文件 - " << filename_ << std::endl;
        }
    }
    
    // 析构函数：关闭文件
    ~FileRAII() {
        if (file_.is_open()) {
            std::cout << "RAII: 关闭文件 - " << filename_ << std::endl;
            file_.close();
        }
    }
    
    void write(const std::string& content) {
        if (file_.is_open()) {
            file_ << content << std::endl;
        }
    }
    
    bool isOpen() const { return file_.is_open(); }
};

// 演示3：互斥锁资源管理
class MutexRAII {
private:
    std::mutex* mutex_;
    std::string name_;
    
public:
    // 构造函数：锁定互斥锁
    MutexRAII(std::mutex& mtx, const std::string& n) : mutex_(&mtx), name_(n) {
        mutex_->lock();
        std::cout << "RAII: 锁定互斥锁 - " << name_ << std::endl;
    }
    
    // 析构函数：解锁互斥锁
    ~MutexRAII() {
        if (mutex_) {
            std::cout << "RAII: 解锁互斥锁 - " << name_ << std::endl;
            mutex_->unlock();
        }
    }
    
    // 禁止复制
    MutexRAII(const MutexRAII&) = delete;
    MutexRAII& operator=(const MutexRAII&) = delete;
};

// 演示4：智能指针 RAII
class SmartPointerRAII {
private:
    std::unique_ptr<int[]> data_;
    std::string name_;
    
public:
    // 构造函数：使用智能指针管理资源
    SmartPointerRAII(int size, const std::string& n) : name_(n) {
        data_ = std::make_unique<int[]>(size);
        for (int i = 0; i < size; ++i) {
            data_[i] = i;
        }
        std::cout << "RAII: 智能指针获取资源 - " << name_ << std::endl;
        std::cout << "  智能指针管理内存: " << data_.get() << std::endl;
    }
    
    // 析构函数：智能指针自动释放资源
    ~SmartPointerRAII() {
        std::cout << "RAII: 智能指针释放资源 - " << name_ << std::endl;
        // 智能指针自动释放，无需手动delete
    }
    
    int* getData() const { return data_.get(); }
    const std::string& getName() const { return name_; }
};

// 演示5：异常安全的 RAII
class ExceptionSafeRAII {
private:
    int* data_;
    std::string name_;
    bool constructed_;
    
public:
    // 构造函数：可能抛出异常
    ExceptionSafeRAII(int size, const std::string& n) : name_(n), constructed_(false) {
        try {
            data_ = new int[size];
            for (int i = 0; i < size; ++i) {
                data_[i] = i;
            }
            constructed_ = true;
            std::cout << "RAII: 异常安全获取资源 - " << name_ << std::endl;
        } catch (...) {
            std::cout << "RAII: 获取资源失败 - " << name_ << std::endl;
            throw; // 重新抛出异常
        }
    }
    
    // 析构函数：只释放已构造的资源
    ~ExceptionSafeRAII() {
        if (constructed_ && data_) {
            std::cout << "RAII: 异常安全释放资源 - " << name_ << std::endl;
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    int* getData() const { return data_; }
    const std::string& getName() const { return name_; }
};

// 演示6：RAII 的优势
void demo_raii_advantages() {
    std::cout << "=== 演示6：RAII 的优势 ===" << std::endl;
    
    std::cout << "RAII 的优势：" << std::endl;
    std::cout << "1. 自动资源管理：无需手动释放资源" << std::endl;
    std::cout << "2. 异常安全：即使发生异常也能正确释放资源" << std::endl;
    std::cout << "3. 防止资源泄漏：自动释放资源，防止泄漏" << std::endl;
    std::cout << "4. 简化代码：减少手动资源管理代码" << std::endl;
    std::cout << "5. 线程安全：自动管理锁等资源" << std::endl;
}

// 演示7：非 RAII 的问题
void demo_non_raii_problems() {
    std::cout << "\n=== 演示7：非 RAII 的问题 ===" << std::endl;
    
    std::cout << "非 RAII 的问题：" << std::endl;
    std::cout << "1. 手动资源管理：容易忘记释放资源" << std::endl;
    std::cout << "2. 异常不安全：异常时资源可能泄漏" << std::endl;
    std::cout << "3. 代码复杂：需要手动管理资源生命周期" << std::endl;
    std::cout << "4. 容易出错：资源管理错误导致程序崩溃" << std::endl;
    
    std::cout << "\n错误示例：" << std::endl;
    std::cout << "void badFunction() {" << std::endl;
    std::cout << "    int* data = new int[10];" << std::endl;
    std::cout << "    // 如果这里抛出异常..." << std::endl;
    std::cout << "    throw std::runtime_error(\"错误\");" << std::endl;
    std::cout << "    delete[] data; // 永远不会执行！" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n正确示例：" << std::endl;
    std::cout << "void goodFunction() {" << std::endl;
    std::cout << "    BasicRAII obj(10, \"test\");" << std::endl;
    std::cout << "    // 如果这里抛出异常..." << std::endl;
    std::cout << "    throw std::runtime_error(\"错误\");" << std::endl;
    std::cout << "    // obj 的析构函数会自动调用！" << std::endl;
    std::cout << "}" << std::endl;
}

// 演示8：RAII 的实际应用
void demo_raii_real_world() {
    std::cout << "\n=== 演示8：RAII 的实际应用 ===" << std::endl;
    
    std::cout << "RAII 的实际应用：" << std::endl;
    std::cout << "1. 智能指针：std::unique_ptr, std::shared_ptr" << std::endl;
    std::cout << "2. 文件管理：std::ifstream, std::ofstream" << std::endl;
    std::cout << "3. 锁管理：std::lock_guard, std::unique_lock" << std::endl;
    std::cout << "4. 内存管理：自定义资源管理类" << std::endl;
    std::cout << "5. 网络连接：自动关闭连接" << std::endl;
    
    std::cout << "\n标准库中的 RAII 示例：" << std::endl;
    std::cout << "std::unique_ptr<int> ptr = std::make_unique<int>(42);" << std::endl;
    std::cout << "std::ofstream file(\"test.txt\");" << std::endl;
    std::cout << "std::lock_guard<std::mutex> lock(mtx);" << std::endl;
}

// 演示9：RAII 的最佳实践
void demo_raii_best_practices() {
    std::cout << "\n=== 演示9：RAII 的最佳实践 ===" << std::endl;
    
    std::cout << "RAII 的最佳实践：" << std::endl;
    std::cout << "1. 优先使用智能指针：避免手动内存管理" << std::endl;
    std::cout << "2. 使用标准库：利用已有的 RAII 类" << std::endl;
    std::cout << "3. 异常安全：确保异常时也能正确释放资源" << std::endl;
    std::cout << "4. 单一职责：每个类只管理一种资源" << std::endl;
    std::cout << "5. 禁止复制：对于独占资源，禁止复制构造" << std::endl;
    
    std::cout << "\n推荐做法：" << std::endl;
    std::cout << "class MyResource {" << std::endl;
    std::cout << "    std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    MyResource(int size) : data_(std::make_unique<int[]>(size)) {}" << std::endl;
    std::cout << "    // 析构函数自动释放资源" << std::endl;
    std::cout << "    // 禁止复制" << std::endl;
    std::cout << "    MyResource(const MyResource&) = delete;" << std::endl;
    std::cout << "    MyResource& operator=(const MyResource&) = delete;" << std::endl;
    std::cout << "};" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== RAII 原则详解 ===" << std::endl;
    
    // 演示1：基本 RAII
    std::cout << "\n=== 演示1：基本 RAII 原则 ===" << std::endl;
    {
        BasicRAII obj(5, "基本RAII");
        std::cout << "对象创建后状态：" << std::endl;
        std::cout << "  名称: " << obj.getName() << std::endl;
        std::cout << "  数据地址: " << obj.getData() << std::endl;
    } // 对象离开作用域，自动调用析构函数
    
    // 演示2：文件资源管理
    std::cout << "\n=== 演示2：文件资源管理 ===" << std::endl;
    {
        FileRAII file("test.txt");
        if (file.isOpen()) {
            file.write("这是RAII管理的文件");
            file.write("自动关闭文件");
        }
    } // 文件自动关闭
    
    // 演示3：互斥锁资源管理
    std::cout << "\n=== 演示3：互斥锁资源管理 ===" << std::endl;
    std::mutex mtx;
    {
        MutexRAII lock(mtx, "互斥锁");
        std::cout << "在锁保护下执行操作" << std::endl;
    } // 锁自动释放
    
    // 演示4：智能指针 RAII
    std::cout << "\n=== 演示4：智能指针 RAII ===" << std::endl;
    {
        SmartPointerRAII obj(5, "智能指针RAII");
        std::cout << "对象创建后状态：" << std::endl;
        std::cout << "  名称: " << obj.getName() << std::endl;
        std::cout << "  数据地址: " << obj.getData() << std::endl;
    } // 智能指针自动释放内存
    
    // 演示5：异常安全的 RAII
    std::cout << "\n=== 演示5：异常安全的 RAII ===" << std::endl;
    try {
        ExceptionSafeRAII obj(5, "异常安全RAII");
        std::cout << "对象创建成功" << std::endl;
        throw std::runtime_error("模拟异常");
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "资源已自动释放" << std::endl;
    }
    
    // 其他演示
    demo_raii_advantages();
    demo_non_raii_problems();
    demo_raii_real_world();
    demo_raii_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "RAII 原则的核心：" << std::endl;
    std::cout << "1. 资源获取即初始化：在构造函数中获取资源" << std::endl;
    std::cout << "2. 资源释放即析构：在析构函数中释放资源" << std::endl;
    std::cout << "3. 自动管理：利用对象生命周期自动管理资源" << std::endl;
    std::cout << "4. 异常安全：即使发生异常也能正确释放资源" << std::endl;
    std::cout << "5. 简化代码：减少手动资源管理代码" << std::endl;
    
    return 0;
}















