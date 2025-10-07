#include <iostream>
#include <chrono>
#include <vector>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

// 测试用的简单类
class TestObject {
private:
    int value_;
    static int count_;
    
public:
    TestObject(int v = 0) : value_(v) { ++count_; }
    TestObject(const TestObject& other) : value_(other.value_) { ++count_; }
    TestObject(TestObject&& other) noexcept : value_(other.value_) { 
        ++count_; 
        other.value_ = 0; 
    }
    ~TestObject() { --count_; }
    
    TestObject& operator=(const TestObject& other) {
        if (this != &other) {
            value_ = other.value_;
        }
        return *this;
    }
    
    TestObject& operator=(TestObject&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
        }
        return *this;
    }
    
    int getValue() const { return value_; }
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int TestObject::count_ = 0;

// 性能测试函数
void test_push_back_performance() {
    std::cout << "\n=== 测试push_back性能 ===" << std::endl;
    
    const size_t test_sizes[] = {1000, 10000, 100000, 1000000};
    
    for (size_t test_size : test_sizes) {
        std::cout << "\n测试大小: " << test_size << std::endl;
        
        // 测试MyTinySTL vector
        auto start = std::chrono::high_resolution_clock::now();
        
        vector<int> mystl_v;
        for (size_t i = 0; i < test_size; ++i) {
            mystl_v.push_back(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto mystl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 测试std::vector
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<int> std_v;
        for (size_t i = 0; i < test_size; ++i) {
            std_v.push_back(i);
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "  MyTinySTL vector: " << mystl_duration.count() << " 微秒" << std::endl;
        std::cout << "  std::vector:     " << std_duration.count() << " 微秒" << std::endl;
        std::cout << "  性能比: " << (double)mystl_duration.count() / std_duration.count() << std::endl;
        std::cout << "  MyTinySTL capacity: " << mystl_v.capacity() << std::endl;
        std::cout << "  std::vector capacity: " << std_v.capacity() << std::endl;
    }
}

void test_copy_performance() {
    std::cout << "\n=== 测试拷贝性能 ===" << std::endl;
    
    const size_t test_sizes[] = {1000, 10000, 100000, 1000000};
    
    for (size_t test_size : test_sizes) {
        std::cout << "\n测试大小: " << test_size << std::endl;
        
        // 准备测试数据
        vector<int> source_v;
        for (size_t i = 0; i < test_size; ++i) {
            source_v.push_back(i);
        }
        
        // 测试MyTinySTL vector拷贝
        auto start = std::chrono::high_resolution_clock::now();
        
        vector<int> mystl_v(source_v);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto mystl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 准备std::vector测试数据
        std::vector<int> std_source_v;
        for (size_t i = 0; i < test_size; ++i) {
            std_source_v.push_back(i);
        }
        
        // 测试std::vector拷贝
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<int> std_v(std_source_v);
        
        end = std::chrono::high_resolution_clock::now();
        auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "  MyTinySTL vector拷贝: " << mystl_duration.count() << " 微秒" << std::endl;
        std::cout << "  std::vector拷贝:     " << std_duration.count() << " 微秒" << std::endl;
        std::cout << "  性能比: " << (double)mystl_duration.count() / std_duration.count() << std::endl;
    }
}

void test_move_performance() {
    std::cout << "\n=== 测试移动性能 ===" << std::endl;
    
    const size_t test_sizes[] = {1000, 10000, 100000, 1000000};
    
    for (size_t test_size : test_sizes) {
        std::cout << "\n测试大小: " << test_size << std::endl;
        
        // 准备测试数据
        vector<int> source_v;
        for (size_t i = 0; i < test_size; ++i) {
            source_v.push_back(i);
        }
        
        // 测试MyTinySTL vector移动
        auto start = std::chrono::high_resolution_clock::now();
        
        vector<int> mystl_v(std::move(source_v));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto mystl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 准备std::vector测试数据
        std::vector<int> std_source_v;
        for (size_t i = 0; i < test_size; ++i) {
            std_source_v.push_back(i);
        }
        
        // 测试std::vector移动
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<int> std_v(std::move(std_source_v));
        
        end = std::chrono::high_resolution_clock::now();
        auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "  MyTinySTL vector移动: " << mystl_duration.count() << " 微秒" << std::endl;
        std::cout << "  std::vector移动:     " << std_duration.count() << " 微秒" << std::endl;
        std::cout << "  性能比: " << (double)mystl_duration.count() / std_duration.count() << std::endl;
    }
}

void test_complex_object_performance() {
    std::cout << "\n=== 测试复杂对象性能 ===" << std::endl;
    
    TestObject::resetCount();
    
    const size_t test_sizes[] = {1000, 10000, 100000};
    
    for (size_t test_size : test_sizes) {
        std::cout << "\n测试大小: " << test_size << std::endl;
        
        // 测试MyTinySTL vector with complex objects
        auto start = std::chrono::high_resolution_clock::now();
        
        vector<TestObject> mystl_v;
        for (size_t i = 0; i < test_size; ++i) {
            mystl_v.push_back(TestObject(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto mystl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 测试std::vector with complex objects
        start = std::chrono::high_resolution_clock::now();
        
        std::vector<TestObject> std_v;
        for (size_t i = 0; i < test_size; ++i) {
            std_v.push_back(TestObject(i));
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "  MyTinySTL vector (复杂对象): " << mystl_duration.count() << " 微秒" << std::endl;
        std::cout << "  std::vector (复杂对象):     " << std_duration.count() << " 微秒" << std::endl;
        std::cout << "  性能比: " << (double)mystl_duration.count() / std_duration.count() << std::endl;
        std::cout << "  对象计数: " << TestObject::getCount() << std::endl;
    }
}

void test_memory_allocation_patterns() {
    std::cout << "\n=== 测试内存分配模式 ===" << std::endl;
    
    std::cout << "\n测试扩容模式：" << std::endl;
    
    vector<int> v;
    size_t prev_capacity = 0;
    
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
        if (v.capacity() != prev_capacity) {
            std::cout << "  元素 " << i << ": size=" << v.size() 
                      << ", capacity=" << v.capacity() 
                      << ", 扩容倍数=" << (double)v.capacity() / (prev_capacity == 0 ? 1 : prev_capacity) << std::endl;
            prev_capacity = v.capacity();
        }
    }
    
    std::cout << "\n最终状态：" << std::endl;
    std::cout << "  size: " << v.size() << std::endl;
    std::cout << "  capacity: " << v.capacity() << std::endl;
    std::cout << "  内存利用率: " << (double)v.size() / v.capacity() * 100 << "%" << std::endl;
}

void test_reallocate_frequency() {
    std::cout << "\n=== 测试reallocate调用频率 ===" << std::endl;
    
    const size_t test_size = 100000;
    
    std::cout << "\n测试push_back过程中的reallocate调用：" << std::endl;
    
    vector<int> v;
    size_t reallocate_count = 0;
    size_t prev_capacity = 0;
    
    for (size_t i = 0; i < test_size; ++i) {
        v.push_back(i);
        if (v.capacity() != prev_capacity) {
            ++reallocate_count;
            prev_capacity = v.capacity();
        }
    }
    
    std::cout << "  总元素数: " << test_size << std::endl;
    std::cout << "  reallocate调用次数: " << reallocate_count << std::endl;
    std::cout << "  平均每次reallocate处理元素数: " << (double)test_size / reallocate_count << std::endl;
    std::cout << "  最终capacity: " << v.capacity() << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector 函数合并效率测试 ===" << std::endl;
    std::cout << "测试目标：验证reallocate和reallocate_and_insert函数合并是否提升效率" << std::endl;
    
    try {
        test_push_back_performance();
        test_copy_performance();
        test_move_performance();
        test_complex_object_performance();
        test_memory_allocation_patterns();
        test_reallocate_frequency();
        
        std::cout << "\n=== 性能测试完成 ===" << std::endl;
        std::cout << "\n分析结论：" << std::endl;
        std::cout << "1. 如果MyTinySTL性能接近std::vector，说明当前实现效率良好" << std::endl;
        std::cout << "2. 如果reallocate调用频率合理，说明扩容策略有效" << std::endl;
        std::cout << "3. 函数合并的收益需要根据具体使用场景评估" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}


