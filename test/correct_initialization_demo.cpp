#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 用户提供的正确代码示例
class MyResource {
    int* data_;
public:
    MyResource() { data_ = new int[10]; }  // 获取资源
    ~MyResource() { delete[] data_; }        // 释放资源
};

// 演示1：分析用户的正确代码
void demo_user_correct_code() {
    std::cout << "=== 演示1：分析用户的正确代码 ===" << std::endl;
    
    std::cout << "用户代码分析：" << std::endl;
    std::cout << "class MyResource {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    MyResource() { data_ = new int[10]; }  // 获取资源" << std::endl;
    std::cout << "    ~MyResource() { delete[] data_; }        // 释放资源" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n这属于正确的初始化！" << std::endl;
    std::cout << "原因分析：" << std::endl;
    std::cout << "1. 成员变量 data_ 被正确初始化" << std::endl;
    std::cout << "   构造函数中：data_ = new int[10];" << std::endl;
    std::cout << "   这给成员变量 data_ 分配了内存并赋值" << std::endl;
    
    std::cout << "\n2. 符合RAII原则" << std::endl;
    std::cout << "   资源获取即初始化：在构造函数中获取资源" << std::endl;
    std::cout << "   资源释放即析构：在析构函数中释放资源" << std::endl;
    
    std::cout << "\n3. 对象状态有效" << std::endl;
    std::cout << "   构造完成后，data_ 指向有效的内存" << std::endl;
    std::cout << "   对象可以安全使用" << std::endl;
    
    std::cout << "\n4. 异常安全" << std::endl;
    std::cout << "   如果构造失败，析构函数会正确释放资源" << std::endl;
}

// 演示2：实际测试用户的代码
void demo_test_user_code() {
    std::cout << "\n=== 演示2：实际测试用户的代码 ===" << std::endl;
    
    std::cout << "测试用户的MyResource类：" << std::endl;
    
    {
        MyResource obj;
        std::cout << "对象创建成功" << std::endl;
        std::cout << "data_ 地址: " << obj.getData() << std::endl;
        std::cout << "对象状态: 有效，可以使用" << std::endl;
    } // 对象离开作用域，自动调用析构函数
    
    std::cout << "对象已自动析构，资源已释放" << std::endl;
}

// 演示3：与错误代码的对比
void demo_comparison_with_wrong_code() {
    std::cout << "\n=== 演示3：与错误代码的对比 ===" << std::endl;
    
    std::cout << "正确代码 vs 错误代码：" << std::endl;
    
    std::cout << "\n正确代码（用户的MyResource）：" << std::endl;
    std::cout << "class MyResource {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    MyResource() { data_ = new int[10]; }  // 初始化成员变量" << std::endl;
    std::cout << "    ~MyResource() { delete[] data_; }        // 释放成员变量" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n错误代码（之前的A类）：" << std::endl;
    std::cout << "struct A {" << std::endl;
    std::cout << "    A() { int* data = new int[4]; }  // 局部变量，不是成员变量" << std::endl;
    std::cout << "    ~A() {" << std::endl;
    std::cout << "        delete[] data;  // 访问未初始化的成员变量" << std::endl;
    std::cout << "        data = nullptr;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    int* data;  // 成员变量未初始化" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n关键区别：" << std::endl;
    std::cout << "1. 正确代码：data_ = new int[10];  // 给成员变量赋值" << std::endl;
    std::cout << "2. 错误代码：int* data = new int[4];  // 创建局部变量" << std::endl;
    std::cout << "3. 正确代码：成员变量被初始化" << std::endl;
    std::cout << "4. 错误代码：成员变量未被初始化" << std::endl;
}

// 演示4：初始化的不同方式
void demo_different_initialization_methods() {
    std::cout << "\n=== 演示4：初始化的不同方式 ===" << std::endl;
    
    std::cout << "初始化的不同方式：" << std::endl;
    
    std::cout << "\n1. 构造函数体中初始化（用户的方式）：" << std::endl;
    std::cout << "MyResource() { data_ = new int[10]; }" << std::endl;
    std::cout << "优点：简单直接" << std::endl;
    std::cout << "缺点：效率稍低（先默认构造，再赋值）" << std::endl;
    
    std::cout << "\n2. 初始化列表（推荐）：" << std::endl;
    std::cout << "MyResource() : data_(new int[10]) {}" << std::endl;
    std::cout << "优点：更高效，直接初始化" << std::endl;
    std::cout << "缺点：语法稍复杂" << std::endl;
    
    std::cout << "\n3. 智能指针（最佳）：" << std::endl;
    std::cout << "class MyResource {" << std::endl;
    std::cout << "    std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    MyResource() : data_(std::make_unique<int[]>(10)) {}" << std::endl;
    std::cout << "    // 无需析构函数，自动管理" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << "优点：自动内存管理，异常安全" << std::endl;
    std::cout << "缺点：需要C++11支持" << std::endl;
}

// 演示5：RAII原则的体现
void demo_raii_principle_embodiment() {
    std::cout << "\n=== 演示5：RAII原则的体现 ===" << std::endl;
    
    std::cout << "用户的MyResource类如何体现RAII原则：" << std::endl;
    
    std::cout << "\n1. 资源获取即初始化：" << std::endl;
    std::cout << "   MyResource() { data_ = new int[10]; }" << std::endl;
    std::cout << "   - 在构造函数中获取资源（内存）" << std::endl;
    std::cout << "   - 确保对象构造时资源已准备好" << std::endl;
    std::cout << "   - 对象创建后立即可用" << std::endl;
    
    std::cout << "\n2. 资源释放即析构：" << std::endl;
    std::cout << "   ~MyResource() { delete[] data_; }" << std::endl;
    std::cout << "   - 在析构函数中释放资源" << std::endl;
    std::cout << "   - 确保对象销毁时资源被正确释放" << std::endl;
    std::cout << "   - 防止内存泄漏" << std::endl;
    
    std::cout << "\n3. 自动管理：" << std::endl;
    std::cout << "   - 利用对象生命周期自动管理资源" << std::endl;
    std::cout << "   - 无需手动管理内存" << std::endl;
    std::cout << "   - 异常安全" << std::endl;
}

// 演示6：异常安全性
void demo_exception_safety() {
    std::cout << "\n=== 演示6：异常安全性 ===" << std::endl;
    
    std::cout << "用户的MyResource类的异常安全性：" << std::endl;
    
    std::cout << "\n1. 构造异常：" << std::endl;
    std::cout << "   try {" << std::endl;
    std::cout << "       MyResource obj;" << std::endl;
    std::cout << "       // 如果这里抛出异常..." << std::endl;
    std::cout << "       throw std::runtime_error(\"错误\");" << std::endl;
    std::cout << "   } catch (...) {" << std::endl;
    std::cout << "       // obj的析构函数会自动调用！" << std::endl;
    std::cout << "       // 资源会被正确释放" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n2. 析构异常：" << std::endl;
    std::cout << "   ~MyResource() { delete[] data_; }" << std::endl;
    std::cout << "   - 析构函数不应该抛出异常" << std::endl;
    std::cout << "   - delete[] 通常不会抛出异常" << std::endl;
    std::cout << "   - 这是安全的" << std::endl;
}

// 演示7：最佳实践建议
void demo_best_practices_suggestions() {
    std::cout << "\n=== 演示7：最佳实践建议 ===" << std::endl;
    
    std::cout << "对用户代码的改进建议：" << std::endl;
    
    std::cout << "\n1. 当前代码（正确但可改进）：" << std::endl;
    std::cout << "   MyResource() { data_ = new int[10]; }" << std::endl;
    std::cout << "   ~MyResource() { delete[] data_; }" << std::endl;
    
    std::cout << "\n2. 改进建议1：使用初始化列表" << std::endl;
    std::cout << "   MyResource() : data_(new int[10]) {}" << std::endl;
    std::cout << "   ~MyResource() { delete[] data_; }" << std::endl;
    
    std::cout << "\n3. 改进建议2：添加空指针检查" << std::endl;
    std::cout << "   ~MyResource() {" << std::endl;
    std::cout << "       if (data_) {" << std::endl;
    std::cout << "           delete[] data_;" << std::endl;
    std::cout << "           data_ = nullptr;" << std::endl;
    std::cout << "       }" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n4. 改进建议3：使用智能指针（最佳）" << std::endl;
    std::cout << "   class MyResource {" << std::endl;
    std::cout << "       std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "   public:" << std::endl;
    std::cout << "       MyResource() : data_(std::make_unique<int[]>(10)) {}" << std::endl;
    std::cout << "       // 无需析构函数" << std::endl;
    std::cout << "   };" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demo_user_correct_code();
    demo_test_user_code();
    demo_comparison_with_wrong_code();
    demo_different_initialization_methods();
    demo_raii_principle_embodiment();
    demo_exception_safety();
    demo_best_practices_suggestions();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "用户的MyResource类分析结果：" << std::endl;
    std::cout << "1. 这属于正确的初始化！" << std::endl;
    std::cout << "2. 成员变量 data_ 被正确初始化" << std::endl;
    std::cout << "3. 符合RAII原则" << std::endl;
    std::cout << "4. 对象状态有效，可以安全使用" << std::endl;
    std::cout << "5. 异常安全" << std::endl;
    std::cout << "6. 虽然可以进一步改进，但基本实现是正确的" << std::endl;
    
    return 0;
}















