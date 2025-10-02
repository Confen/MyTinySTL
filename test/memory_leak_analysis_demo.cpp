#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include "../memory.h"

#define MYTINYSTL_DEBUG

using namespace mystl;

// ============================================================================
// 内存泄露检测原理详解
// ============================================================================

void explain_detection_principle() {
    std::cout << "=== 内存泄露检测原理详解 ===" << std::endl;
    
    std::cout << "\n1. 检测机制组成：" << std::endl;
    std::cout << "   - memory_manager: 统计总体内存使用情况" << std::endl;
    std::cout << "   - memory_debug: 记录分配/释放操作次数" << std::endl;
    std::cout << "   - memory_leak_detector: 维护详细的分配记录链表" << std::endl;
    
    std::cout << "\n2. 工作原理：" << std::endl;
    std::cout << "   - 分配时：记录指针、大小、文件名、行号到链表" << std::endl;
    std::cout << "   - 释放时：从链表中移除对应记录" << std::endl;
    std::cout << "   - 检测时：链表中剩余的记录就是泄露的内存" << std::endl;
    
    std::cout << "\n3. 优势：" << std::endl;
    std::cout << "   - 精确定位：提供泄露发生的文件和行号" << std::endl;
    std::cout << "   - 实时监控：可以随时检查当前内存状态" << std::endl;
    std::cout << "   - 零开销：发布版本完全无性能影响" << std::endl;
    
    std::cout << "\n4. 数据结构：" << std::endl;
    std::cout << "   struct allocation_info {" << std::endl;
    std::cout << "       void* ptr;          // 分配的指针地址" << std::endl;
    std::cout << "       size_t size;        // 分配的内存大小" << std::endl;
    std::cout << "       const char* file;   // 分配发生的文件" << std::endl;
    std::cout << "       int line;           // 分配发生的行号" << std::endl;
    std::cout << "       allocation_info* next; // 链表指针" << std::endl;
    std::cout << "   };" << std::endl;
}

// ============================================================================
// 测试用例类
// ============================================================================

class LeakTestObject {
private:
    std::string name_;
    int* data_;
    size_t size_;
    
public:
    LeakTestObject(const std::string& name, size_t size) 
        : name_(name), size_(size) {
        data_ = static_cast<int*>(MYTINYSTL_ALLOC(size * sizeof(int)));
        std::cout << "创建 " << name_ << "，分配 " << size * sizeof(int) << " 字节" << std::endl;
        
        // 初始化数据
        for (size_t i = 0; i < size; ++i) {
            data_[i] = static_cast<int>(i);
        }
    }
    
    ~LeakTestObject() {
        if (data_) {
            MYTINYSTL_FREE(data_, size_ * sizeof(int));
            std::cout << "销毁 " << name_ << "，释放 " << size_ * sizeof(int) << " 字节" << std::endl;
        }
    }
    
    void print_info() const {
        std::cout << "对象 " << name_ << "：地址=" << data_ 
                  << "，大小=" << size_ * sizeof(int) << " 字节" << std::endl;
    }
    
    // 禁用拷贝
    LeakTestObject(const LeakTestObject&) = delete;
    LeakTestObject& operator=(const LeakTestObject&) = delete;
};

// ============================================================================
// 测试场景
// ============================================================================

void test_scenario_1_simple_leak() {
    std::cout << "\n=== 场景1：简单内存泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "分配内存但不释放：" << std::endl;
    void* ptr1 = MYTINYSTL_ALLOC(1024);
    void* ptr2 = MYTINYSTL_ALLOC(2048);
    
    std::cout << "分配了两个内存块：" << ptr1 << " 和 " << ptr2 << std::endl;
    
    std::cout << "\n检测结果：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
    
    // 清理（实际应用中这些内存已经泄露了）
    MYTINYSTL_FREE(ptr1, 1024);
    MYTINYSTL_FREE(ptr2, 2048);
}

void test_scenario_2_object_leak() {
    std::cout << "\n=== 场景2：对象内存泄露 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "创建对象但不删除：" << std::endl;
    LeakTestObject* obj1 = new LeakTestObject("对象1", 100);
    LeakTestObject* obj2 = new LeakTestObject("对象2", 200);
    
    obj1->print_info();
    obj2->print_info();
    
    std::cout << "\n检测结果：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
    
    // 清理
    delete obj1;
    delete obj2;
}

void test_scenario_3_partial_cleanup() {
    std::cout << "\n=== 场景3：部分清理 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "创建多个对象：" << std::endl;
    LeakTestObject* obj1 = new LeakTestObject("对象A", 50);
    LeakTestObject* obj2 = new LeakTestObject("对象B", 75);
    LeakTestObject* obj3 = new LeakTestObject("对象C", 100);
    LeakTestObject* obj4 = new LeakTestObject("对象D", 125);
    
    std::cout << "\n删除部分对象：" << std::endl;
    delete obj1;
    delete obj3;
    
    std::cout << "\n检测结果（应该显示obj2和obj4泄露）：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
    
    // 清理剩余对象
    delete obj2;
    delete obj4;
}

void test_scenario_4_smart_pointer_comparison() {
    std::cout << "\n=== 场景4：智能指针对比 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "使用智能指针（无泄露）：" << std::endl;
    {
        auto smart_obj1 = make_unique<LeakTestObject>("智能对象1", 150);
        auto smart_obj2 = make_unique<LeakTestObject>("智能对象2", 200);
        
        smart_obj1->print_info();
        smart_obj2->print_info();
    } // 智能指针自动清理
    
    std::cout << "\n智能指针检测结果：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "\n使用原始指针（有泄露）：" << std::endl;
    {
        LeakTestObject* raw_obj1 = new LeakTestObject("原始对象1", 150);
        LeakTestObject* raw_obj2 = new LeakTestObject("原始对象2", 200);
        
        raw_obj1->print_info();
        raw_obj2->print_info();
        
        // 故意不删除
        (void)raw_obj1;
        (void)raw_obj2;
    }
    
    std::cout << "\n原始指针检测结果：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

void test_scenario_5_exception_safety() {
    std::cout << "\n=== 场景5：异常安全性测试 ===" << std::endl;
    
    memory_debug::reset();
    memory_leak_detector::reset();
    
    std::cout << "测试异常情况下的内存管理：" << std::endl;
    
    try {
        LeakTestObject* obj1 = new LeakTestObject("异常前对象", 100);
        obj1->print_info();
        
        // 模拟异常
        throw std::runtime_error("模拟异常");
        
        // 这行代码不会执行
        delete obj1;
        
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "异常导致内存未正确释放" << std::endl;
    }
    
    std::cout << "\n异常后检测结果：" << std::endl;
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

// ============================================================================
// 性能测试
// ============================================================================

void test_performance_impact() {
    std::cout << "\n=== 性能影响测试 ===" << std::endl;
    
    const size_t test_count = 10000;
    
    // 测试调试模式性能
    std::cout << "测试调试模式下的性能影响：" << std::endl;
    memory_debug::enable_debug();
    memory_debug::reset();
    memory_leak_detector::reset();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<void*> ptrs;
    for (size_t i = 0; i < test_count; ++i) {
        void* ptr = MYTINYSTL_ALLOC(64);
        ptrs.push_back(ptr);
    }
    
    for (void* ptr : ptrs) {
        MYTINYSTL_FREE(ptr, 64);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "调试模式：" << test_count << " 次分配/释放耗时 " 
              << duration.count() << " 微秒" << std::endl;
    
    memory_debug::print_summary();
    memory_leak_detector::check_leaks();
}

// ============================================================================
// 最佳实践演示
// ============================================================================

void demonstrate_best_practices() {
    std::cout << "\n=== 最佳实践演示 ===" << std::endl;
    
    std::cout << "\n1. 使用RAII原则：" << std::endl;
    {
        memory_debug::reset();
        memory_leak_detector::reset();
        
        // 好的做法：使用智能指针
        auto good_obj = make_unique<LeakTestObject>("RAII对象", 100);
        
        // 作用域结束时自动清理
    }
    
    std::cout << "RAII检测结果：" << std::endl;
    memory_leak_detector::check_leaks();
    
    std::cout << "\n2. 避免裸指针：" << std::endl;
    {
        memory_debug::reset();
        memory_leak_detector::reset();
        
        // 不好的做法：使用裸指针
        LeakTestObject* bad_obj = new LeakTestObject("裸指针对象", 100);
        // 忘记删除
        (void)bad_obj;
    }
    
    std::cout << "裸指针检测结果：" << std::endl;
    memory_leak_detector::check_leaks();
    
    std::cout << "\n3. 异常安全的资源管理：" << std::endl;
    {
        memory_debug::reset();
        memory_leak_detector::reset();
        
        try {
            auto safe_obj = make_unique<LeakTestObject>("异常安全对象", 100);
            
            // 即使这里抛出异常，智能指针也会自动清理
            if (true) {  // 模拟条件
                throw std::runtime_error("测试异常");
            }
            
        } catch (const std::exception& e) {
            std::cout << "异常被安全处理: " << e.what() << std::endl;
        }
    }
    
    std::cout << "异常安全检测结果：" << std::endl;
    memory_leak_detector::check_leaks();
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "MyTinySTL 内存泄露检测原理与测试方法详解" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    try {
        // 启用调试模式
        memory_debug::enable_debug();
        
        // 原理讲解
        explain_detection_principle();
        
        // 测试场景
        test_scenario_1_simple_leak();
        test_scenario_2_object_leak();
        test_scenario_3_partial_cleanup();
        test_scenario_4_smart_pointer_comparison();
        test_scenario_5_exception_safety();
        
        // 性能测试
        test_performance_impact();
        
        // 最佳实践
        demonstrate_best_practices();
        
        std::cout << "\n=== 总结 ===" << std::endl;
        std::cout << "内存泄露检测系统的核心优势：" << std::endl;
        std::cout << "1. 精确定位泄露位置（文件名+行号）" << std::endl;
        std::cout << "2. 实时监控内存使用情况" << std::endl;
        std::cout << "3. 发布版本零性能开销" << std::endl;
        std::cout << "4. 支持复杂的内存管理场景" << std::endl;
        std::cout << "5. 与智能指针完美配合" << std::endl;
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
