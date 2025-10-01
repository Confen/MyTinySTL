#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <windows.h>

void safe_function() noexcept {
    std::cout << "安全函数执行" << std::endl;
}

void risky_function() {
    throw std::runtime_error("risky_function");
}

class BasicGuaranteeClass {
    public:
    // BasicGuaranteeClass() {}
    BasicGuaranteeClass()  {throw std::runtime_error("BasicGuaranteeClass");}
    ~BasicGuaranteeClass() {}
};
    class StrongGuaranteeClass {
        std::vector<int> data;
    public:
        // 默认构造函数
        StrongGuaranteeClass() = default;
        
        // 拷贝构造函数
        StrongGuaranteeClass(const StrongGuaranteeClass&) = default;
        
        // 拷贝赋值操作符 - 强异常保证
        StrongGuaranteeClass& operator=(const StrongGuaranteeClass& other) {
            if (this != &other) {
                std::vector<int> temp = other.data;  // 创建副本
                // 如果这里抛出异常，原对象状态不变
                data = std::move(temp);  // 原子性操作
            }
            return *this;
        }
        
        // 添加元素 - 强异常保证
        void addElement(int value) {
            std::vector<int> temp = data;  // 创建副本（temp）
            temp.push_back(value);
            
            // 模拟可能失败的条件：如果值太大就抛出异常
            if (value > 100) {
                throw std::runtime_error("值太大，操作失败");
            }
            
            // 如果上面抛出异常，原对象状态不变,没有抛出异常则更新data
            data = std::move(temp);  // 原子性操作
        }
        
        // 添加多个元素 - 强异常保证
        void addElements(const std::vector<int>& values) {
            std::vector<int> temp = data;  // 创建副本
            
            // 先检查所有值
            for (int value : values) {
                if (value > 50) {
                    throw std::runtime_error("包含过大的值，操作失败");
                }
            }
            
            // 如果上面抛出异常，原对象状态不变
            temp.insert(temp.end(), values.begin(), values.end());
            data = std::move(temp);  // 原子性操作
        }
        
        size_t size() const { return data.size(); }
        const std::vector<int>& getData() const { return data; }
    };
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 测试开始 ===" << std::endl;
    
    // 测试1：安全函数
    std::cout << "1. 测试安全函数..." << std::endl;
    MYSTL_NOEXCEPT_ASSERT(safe_function());
    std::cout << "   安全函数测试通过" << std::endl;
    
    // 测试2：基本异常保证//宏设计的目的就是捕获异常来测试异常安全性，而不是阻止异常
    std::cout << "2. 测试基本异常保证..." << std::endl;
    MYSTL_BASIC_GUARANTEE_ASSERT(BasicGuaranteeClass());
    std::cout << "   基本异常保证测试通过（异常被捕获）" << std::endl;
    
    // 测试3：直接创建对象（不通过宏）
    std::cout << "3. 直接创建对象（会抛出异常）..." << std::endl;
    try {
        BasicGuaranteeClass obj;
        std::cout << "   对象创建成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获到异常: " << e.what() << std::endl;
    }

    // 测试2：强异常保证 - 操作成功的情况
    std::cout << "2. 测试强异常保证 - 操作成功..." << std::endl;
    StrongGuaranteeClass obj1;
    obj1.addElement(1);
    obj1.addElement(2);
    std::cout << "   初始对象大小: " << obj1.size() << "，数据: [";
    for (size_t i = 0; i < obj1.size(); ++i) {
        std::cout << obj1.getData()[i];
        if (i < obj1.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    // 测试成功操作
    try {
        obj1.addElement(3);
        std::cout << "   添加元素3成功，新大小: " << obj1.size() << "，数据: [";
        for (size_t i = 0; i < obj1.size(); ++i) {
            std::cout << obj1.getData()[i];
            if (i < obj1.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
    }
    
    // 测试3：强异常保证 - 操作失败的情况
    std::cout << "\n3. 测试强异常保证 - 操作失败..." << std::endl;
    std::cout << "   操作前对象大小: " << obj1.size() << "，数据: [";
    for (size_t i = 0; i < obj1.size(); ++i) {
        std::cout << obj1.getData()[i];
        if (i < obj1.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    try {
        obj1.addElement(150);  // 这个会失败
        std::cout << "   添加元素150成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   强异常保证：对象状态未改变，大小仍为: " << obj1.size() << "，数据: [";
        for (size_t i = 0; i < obj1.size(); ++i) {
            std::cout << obj1.getData()[i];
            if (i < obj1.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    // 测试4：批量操作失败
    std::cout << "\n4. 测试批量操作失败..." << std::endl;
    try {
        obj1.addElements({10, 20, 60, 30});  // 60 > 50，会失败
        std::cout << "   批量添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   强异常保证：对象状态未改变，大小仍为: " << obj1.size() << std::endl;
    }
    
    // 测试3：使用强异常保证宏
    std::cout << "3. 使用强异常保证宏..." << std::endl;
    MYSTL_STRONG_GUARANTEE_ASSERT(obj1.addElement(60));
    std::cout << "   强异常保证宏测试通过" << std::endl;
    std::cout << obj1.size() << std::endl;  //60 < 100添加成功
    std::cout << "=== 测试完成 ===" << std::endl;
    return 0;
}
