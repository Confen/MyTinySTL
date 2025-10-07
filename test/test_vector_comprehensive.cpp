#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

// 测试用的类
class TestClass {
private:
    int value_;
    std::string name_;
    static int count_;
    
public:
    TestClass(int v, const std::string& n) : value_(v), name_(n) {
        ++count_;
        std::cout << "构造 TestClass(" << value_ << ", " << name_ << "), 总数: " << count_ << std::endl;
    }
    
    TestClass(const TestClass& other) : value_(other.value_), name_(other.name_) {
        ++count_;
        std::cout << "拷贝构造 TestClass(" << value_ << ", " << name_ << "), 总数: " << count_ << std::endl;
    }
    
    TestClass(TestClass&& other) noexcept : value_(other.value_), name_(std::move(other.name_)) {
        ++count_;
        other.value_ = 0;
        other.name_ = "";
        std::cout << "移动构造 TestClass(" << value_ << ", " << name_ << "), 总数: " << count_ << std::endl;
    }
    
    ~TestClass() {
        --count_;
        std::cout << "析构 TestClass(" << value_ << ", " << name_ << "), 剩余: " << count_ << std::endl;
    }
    
    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value_ = other.value_;
            name_ = other.name_;
            std::cout << "拷贝赋值 TestClass(" << value_ << ", " << name_ << ")" << std::endl;
        }
        return *this;
    }
    
    TestClass& operator=(TestClass&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            name_ = std::move(other.name_);
            other.value_ = 0;
            other.name_ = "";
            std::cout << "移动赋值 TestClass(" << value_ << ", " << name_ << ")" << std::endl;
        }
        return *this;
    }
    
    int getValue() const { return value_; }
    const std::string& getName() const { return name_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int TestClass::count_ = 0;

// 测试函数
void test_basic_operations() {
    std::cout << "\n=== 测试基础操作 ===" << std::endl;
    
    // 测试默认构造
    std::cout << "\n1. 测试默认构造：" << std::endl;
    vector<int> v1;
    std::cout << "   空vector - size: " << v1.size() 
              << ", capacity: " << v1.capacity() 
              << ", empty: " << (v1.empty() ? "true" : "false") << std::endl;
    
    // 测试指定大小构造
    std::cout << "\n2. 测试指定大小构造：" << std::endl;
    vector<int> v2(5);
    std::cout << "   vector<int> v2(5) - size: " << v2.size() 
              << ", capacity: " << v2.capacity() << std::endl;
    
    // 测试指定大小和值构造
    std::cout << "\n3. 测试指定大小和值构造：" << std::endl;
    vector<int> v3(3, 42);
    std::cout << "   vector<int> v3(3, 42) - size: " << v3.size() 
              << ", capacity: " << v3.capacity() << std::endl;
    
    // 测试初始化列表构造
    std::cout << "\n4. 测试初始化列表构造：" << std::endl;
    vector<int> v4{1, 2, 3, 4, 5};
    std::cout << "   vector<int> v4{1,2,3,4,5} - size: " << v4.size() 
              << ", capacity: " << v4.capacity() << std::endl;
}

void test_push_pop_operations() {
    std::cout << "\n=== 测试push/pop操作 ===" << std::endl;
    
    vector<int> v;
    std::cout << "\n1. 测试push_back操作：" << std::endl;
    
    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
        std::cout << "   添加元素 " << i << " - size: " << v.size() 
                  << ", capacity: " << v.capacity() << std::endl;
    }
    
    std::cout << "\n2. 测试pop_back操作：" << std::endl;
    for (int i = 0; i < 5; ++i) {
        v.pop_back();
        std::cout << "   删除元素后 - size: " << v.size() 
                  << ", capacity: " << v.capacity() << std::endl;
    }
}

void test_copy_move_operations() {
    std::cout << "\n=== 测试拷贝/移动操作 ===" << std::endl;
    
    // 测试拷贝构造
    std::cout << "\n1. 测试拷贝构造：" << std::endl;
    vector<int> v1{1, 2, 3, 4, 5};
    vector<int> v2(v1);
    std::cout << "   原vector - size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
    std::cout << "   拷贝vector - size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    
    // 测试移动构造
    std::cout << "\n2. 测试移动构造：" << std::endl;
    vector<int> v3{6, 7, 8, 9, 10};
    vector<int> v4(std::move(v3));
    std::cout << "   原vector - size: " << v3.size() << ", capacity: " << v3.capacity() << std::endl;
    std::cout << "   移动vector - size: " << v4.size() << ", capacity: " << v4.capacity() << std::endl;
    
    // 测试拷贝赋值
    std::cout << "\n3. 测试拷贝赋值：" << std::endl;
    vector<int> v5{11, 12, 13};
    vector<int> v6{14, 15, 16, 17, 18};
    std::cout << "   赋值前 v5 - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
    std::cout << "   赋值前 v6 - size: " << v6.size() << ", capacity: " << v6.capacity() << std::endl;
    
    v5 = v6;
    std::cout << "   赋值后 v5 - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
    std::cout << "   赋值后 v6 - size: " << v6.size() << ", capacity: " << v6.capacity() << std::endl;
    
    // 测试移动赋值
    std::cout << "\n4. 测试移动赋值：" << std::endl;
    vector<int> v7{19, 20, 21, 22};
    vector<int> v8{23, 24, 25, 26, 27, 28};
    std::cout << "   移动前 v7 - size: " << v7.size() << ", capacity: " << v7.capacity() << std::endl;
    std::cout << "   移动前 v8 - size: " << v8.size() << ", capacity: " << v8.capacity() << std::endl;
    
    v7 = std::move(v8);
    std::cout << "   移动后 v7 - size: " << v7.size() << ", capacity: " << v7.capacity() << std::endl;
    std::cout << "   移动后 v8 - size: " << v8.size() << ", capacity: " << v8.capacity() << std::endl;
}

void test_complex_objects() {
    std::cout << "\n=== 测试复杂对象 ===" << std::endl;
    
    TestClass::resetCount();
    
    std::cout << "\n1. 创建TestClass对象：" << std::endl;
    vector<TestClass> v;
    
    std::cout << "\n2. 添加TestClass对象：" << std::endl;
    v.push_back(TestClass(1, "First"));
    v.push_back(TestClass(2, "Second"));
    v.push_back(TestClass(3, "Third"));
    
    std::cout << "\n3. 当前vector状态：" << std::endl;
    std::cout << "   size: " << v.size() << ", capacity: " << v.capacity() << std::endl;
    std::cout << "   TestClass总数: " << TestClass::getCount() << std::endl;
    
    std::cout << "\n4. 测试拷贝构造：" << std::endl;
    vector<TestClass> v2(v);
    std::cout << "   拷贝后 TestClass总数: " << TestClass::getCount() << std::endl;
    
    std::cout << "\n5. 测试移动构造：" << std::endl;
    vector<TestClass> v3(std::move(v2));
    std::cout << "   移动后 TestClass总数: " << TestClass::getCount() << std::endl;
    
    std::cout << "\n6. 测试pop_back：" << std::endl;
    v3.pop_back();
    std::cout << "   删除后 TestClass总数: " << TestClass::getCount() << std::endl;
    
    std::cout << "\n7. 作用域结束，对象自动析构：" << std::endl;
}

void test_exception_safety() {
    std::cout << "\n=== 测试异常安全 ===" << std::endl;
    
    std::cout << "\n1. 测试正常情况：" << std::endl;
    try {
        vector<int> v;
        for (int i = 0; i < 1000; ++i) {
            v.push_back(i);
        }
        std::cout << "   成功添加1000个元素 - size: " << v.size() 
                  << ", capacity: " << v.capacity() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n2. 测试内存分配失败：" << std::endl;
    try {
        vector<int> v;
        // 尝试分配非常大的内存
        v.reserve(SIZE_MAX);
        std::cout << "   意外成功分配内存" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   异常安全：vector状态保持有效" << std::endl;
    }
}

void test_performance() {
    std::cout << "\n=== 测试性能 ===" << std::endl;
    
    const size_t test_size = 100000;
    
    std::cout << "\n1. 测试push_back性能：" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    vector<int> v;
    for (size_t i = 0; i < test_size; ++i) {
        v.push_back(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "   添加 " << test_size << " 个元素耗时: " << duration.count() << " 微秒" << std::endl;
    std::cout << "   最终size: " << v.size() << ", capacity: " << v.capacity() << std::endl;
    
    std::cout << "\n2. 测试拷贝性能：" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    
    vector<int> v2(v);
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "   拷贝 " << v.size() << " 个元素耗时: " << duration.count() << " 微秒" << std::endl;
    
    std::cout << "\n3. 测试移动性能：" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    
    vector<int> v3(std::move(v2));
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "   移动 " << v3.size() << " 个元素耗时: " << duration.count() << " 微秒" << std::endl;
}

void test_std_compatibility() {
    std::cout << "\n=== 测试与std::vector的兼容性 ===" << std::endl;
    
    std::cout << "\n1. 测试接口兼容性：" << std::endl;
    vector<int> mystl_v{1, 2, 3, 4, 5};
    std::vector<int> std_v{1, 2, 3, 4, 5};
    
    std::cout << "   MyTinySTL vector - size: " << mystl_v.size() 
              << ", capacity: " << mystl_v.capacity() << std::endl;
    std::cout << "   std::vector - size: " << std_v.size() 
              << ", capacity: " << std_v.capacity() << std::endl;
    
    std::cout << "\n2. 测试操作兼容性：" << std::endl;
    mystl_v.push_back(6);
    std_v.push_back(6);
    
    mystl_v.pop_back();
    std_v.pop_back();
    
    std::cout << "   操作后 MyTinySTL vector - size: " << mystl_v.size() << std::endl;
    std::cout << "   操作后 std::vector - size: " << std_v.size() << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector 综合测试 ===" << std::endl;
    
    try {
        test_basic_operations();
        test_push_pop_operations();
        test_copy_move_operations();
        test_complex_objects();
        test_exception_safety();
        test_performance();
        test_std_compatibility();
        
        std::cout << "\n=== 所有测试完成 ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}


