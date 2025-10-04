#include <iostream>
#include <vector>
#include "../memory.h"

// 定义调试模式
#define MYTINYSTL_DEBUG

using namespace mystl;

class TestObject {
public:
    int* data;
    size_t size;
    
    TestObject(size_t n) : size(n) {
        data = static_cast<int*>(MYTINYSTL_ALLOC(n * sizeof(int)));
        std::cout << "TestObject 构造，分配 " << n * sizeof(int) << " 字节" << std::endl;
    }
    
    ~TestObject() {
        if (data) {
            MYTINYSTL_FREE(data, size * sizeof(int));
            std::cout << "TestObject 析构，释放 " << size * sizeof(int) << " 字节" << std::endl;
        }
    }
};

void test_no_leak() {
    std::cout << "\n=== 测试：无内存泄露 ===" << std::endl;
    
    memory_debug::enable_debug();
    memory_debug::reset();
    memory_leak_detector::reset();
    
    {
        TestObject obj(100);
        // 对象在作用域结束时自动析构
    }
    
    std::cout << "\n--- 内存状态检查 ---" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_with_leak() {
    std::cout << "\n=== 测试：有内存泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    {
        TestObject* obj = new TestObject(200);
        // 故意不删除对象，造成内存泄露
        (void)obj; // 避免未使用变量警告
    }
    
    std::cout << "\n--- 内存状态检查 ---" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_partial_leak() {
    std::cout << "\n=== 测试：部分内存泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    {
        TestObject* obj1 = new TestObject(50);
        TestObject* obj2 = new TestObject(75);
        TestObject* obj3 = new TestObject(100);
        
        // 只删除部分对象
        delete obj1;  // 正常释放
        delete obj2;  // 正常释放
        // obj3 未删除，造成泄露
        (void)obj3;
    }
    
    std::cout << "\n--- 内存状态检查 ---" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_smart_pointer_no_leak() {
    std::cout << "\n=== 测试：智能指针无泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    {
        auto ptr1 = make_unique<TestObject>(150);
        auto ptr2 = make_unique<TestObject>(250);
        // 智能指针自动管理内存
    }
    
    std::cout << "\n--- 内存状态检查 ---" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_smart_pointer_with_leak() {
    std::cout << "\n=== 测试：智能指针 release() 造成泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    {
        auto ptr = make_unique<TestObject>(300);
        TestObject* raw_ptr = ptr.release();  // 释放所有权
        // 故意不删除 raw_ptr，造成泄露
        (void)raw_ptr;
    }
    
    std::cout << "\n--- 内存状态检查 ---" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_memory_manager_statistics() {
    std::cout << "\n=== 测试：内存管理器统计 ===" << std::endl;
    
    memory_manager::reset();
    
    // 模拟一系列内存操作
    memory_manager::record_allocation(1024);
    memory_manager::record_allocation(2048);
    memory_manager::record_allocation(512);
    
    std::cout << "分配后状态：" << std::endl;
    memory_manager::print_status();
    
    memory_manager::record_deallocation(1024);
    memory_manager::record_deallocation(512);
    
    std::cout << "\n部分释放后状态：" << std::endl;
    memory_manager::print_status();
    
    memory_manager::record_deallocation(2048);
    
    std::cout << "\n全部释放后状态：" << std::endl;
    memory_manager::print_status();
}

void test_leak_detection_accuracy() {
    std::cout << "\n=== 测试：泄露检测准确性 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    // 分配多个不同大小的内存块
    void* ptr1 = MYTINYSTL_ALLOC(100);
    void* ptr2 = MYTINYSTL_ALLOC(200);
    void* ptr3 = MYTINYSTL_ALLOC(300);
    void* ptr4 = MYTINYSTL_ALLOC(400);
    
    std::cout << "分配了4个内存块" << std::endl;
    memory_debug::print_summary();
    
    // 释放部分内存
    MYTINYSTL_FREE(ptr2, 200);
    MYTINYSTL_FREE(ptr4, 400);
    
    std::cout << "\n释放了2个内存块" << std::endl;
    memory_debug::print_summary();
    
    std::cout << "\n检查剩余泄露：" << std::endl;
    memory_leak_detector::check_leaks();
    
    // 清理剩余内存
    MYTINYSTL_FREE(ptr1, 100);
    MYTINYSTL_FREE(ptr3, 300);
    
    std::cout << "\n全部清理后：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void demonstrate_debugging_workflow() {
    std::cout << "\n=== 演示：完整调试工作流程 ===" << std::endl;
    
    // 1. 初始化调试环境
    std::cout << "1. 初始化调试环境" << std::endl;
    memory_debug::enable_debug();
    memory_debug::reset();
    memory_leak_detector::reset();
    memory_manager::reset();
    
    // 2. 执行可能有泄露的代码
    std::cout << "\n2. 执行测试代码" << std::endl;
    {
        TestObject* obj1 = new TestObject(100);
        TestObject* obj2 = new TestObject(200);
        
        delete obj1;  // 正常释放
        // obj2 故意不释放
        (void)obj2;
    }
    
    // 3. 检查内存状态
    std::cout << "\n3. 检查内存状态" << std::endl;
    memory_debug::print_summary();
    memory_manager::print_status();
    
    // 4. 检测泄露
    std::cout << "\n4. 检测内存泄露" << std::endl;
    memory_leak_detector::check_leaks();
    
    // 5. 分析结果
    std::cout << "\n5. 分析结果" << std::endl;
    size_t alloc_count = memory_debug::get_allocation_count();
    size_t dealloc_count = memory_debug::get_deallocation_count();
    
    if (alloc_count > dealloc_count) {
        std::cout << "检测到 " << (alloc_count - dealloc_count) 
                  << " 次未匹配的分配操作" << std::endl;
    } else {
        std::cout << "所有分配操作都已正确释放" << std::endl;
    }
}

int main() {
    std::cout << "MyTinySTL 内存泄露检测系统演示" << std::endl;
    std::cout << "=================================" << std::endl;
    
    try {
        test_no_leak();
        test_with_leak();
        test_partial_leak();
        test_smart_pointer_no_leak();
        test_smart_pointer_with_leak();
        test_memory_manager_statistics();
        test_leak_detection_accuracy();
        demonstrate_debugging_workflow();
        
        std::cout << "\n=== 所有测试完成 ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}






