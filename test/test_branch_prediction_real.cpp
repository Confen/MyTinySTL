#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

// 生成随机数据，避免编译器优化
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 1);

void test_branch_prediction_with_random_data() {
    std::cout << "\n=== 真实分支预测测试（随机数据）===" << std::endl;
    
    const size_t test_size = 10000000; // 增大测试规模
    std::vector<int> data(test_size);
    
    // 生成随机数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = dis(gen);
    }
    
    // 测试有分支的情况
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (data[i] == 0) {  // 随机分支，难以预测
            sum += i;
        } else {
            sum -= i;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto branch_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试无分支的情况（使用位运算避免分支）
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        // 使用位运算避免分支
        int mask = (data[i] - 1) & 0xFFFFFFFF;  // 如果data[i]==0，mask=0xFFFFFFFF；否则mask=0
        sum += (i & mask) + (-i & ~mask);  // 等价于条件分支，但无分支
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto no_branch_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  随机分支测试: " << branch_duration.count() << " 微秒" << std::endl;
    std::cout << "  无分支测试: " << no_branch_duration.count() << " 微秒" << std::endl;
    std::cout << "  分支开销: " << (double)branch_duration.count() / no_branch_duration.count() << "x" << std::endl;
}

void test_vector_branch_prediction_impact() {
    std::cout << "\n=== Vector实现中的分支预测影响 ===" << std::endl;
    
    // 测试push_back中的分支预测
    const size_t test_size = 1000000;
    
    // 测试1：顺序push_back（分支预测友好）
    auto start = std::chrono::high_resolution_clock::now();
    
    vector<int> v1;
    for (size_t i = 0; i < test_size; ++i) {
        v1.push_back(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试2：随机大小vector的push_back（分支预测不友好）
    start = std::chrono::high_resolution_clock::now();
    
    vector<int> v2;
    std::vector<size_t> sizes;
    for (size_t i = 0; i < 1000; ++i) {
        sizes.push_back(dis(gen) * 1000 + 100);  // 随机大小：100或1100
    }
    
    for (size_t size : sizes) {
        vector<int> temp;
        for (size_t j = 0; j < size; ++j) {
            temp.push_back(j);
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序push_back: " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机大小push_back: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  分支预测影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_compiler_optimization_levels() {
    std::cout << "\n=== 编译器优化级别对分支预测的影响 ===" << std::endl;
    
    const size_t test_size = 1000000;
    std::vector<int> data(test_size);
    
    // 生成半随机数据（50%概率为0）
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = (i % 2 == 0) ? 0 : 1;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (data[i] == 0) { //编译器对规律性分支进行了优化
            sum += i;
        } else {
            sum -= i;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  规律性分支测试: " << duration.count() << " 微秒" << std::endl;
    std::cout << "  编译器可能优化了规律性分支" << std::endl;
}

void test_memory_access_patterns() {
    std::cout << "\n=== 内存访问模式对分支预测的影响 ===" << std::endl;
    
    const size_t test_size = 1000000;
    std::vector<int> data(test_size);
    
    // 生成数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = i % 100;  // 0-99的循环
    }
    
    // 测试1：顺序访问
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (data[i] < 50) {
            sum += data[i];
        } else {
            sum -= data[i];
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试2：随机访问
    std::vector<size_t> indices(test_size);
    for (size_t i = 0; i < test_size; ++i) {
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), gen);
    
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        size_t idx = indices[i];
        if (data[idx] < 50) {
            sum += data[idx];
        } else {
            sum -= data[idx];
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序访问: " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机访问: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_vector_reallocate_branch_prediction() {
    std::cout << "\n=== Vector reallocate中的分支预测分析 ===" << std::endl;
    
    // 模拟reallocate中的分支判断
    const size_t test_size = 1000000;
    
    // 测试1：频繁扩容（分支预测友好）
    auto start = std::chrono::high_resolution_clock::now();
    
    vector<int> v1;
    for (size_t i = 0; i < test_size; ++i) {
        v1.push_back(i);
        // 每次push_back都可能触发扩容检查
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto frequent_realloc_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试2：预分配容量（减少分支判断）
    start = std::chrono::high_resolution_clock::now();
    
    vector<int> v2;
    v2.reserve(test_size);  // 预分配，减少扩容检查
    for (size_t i = 0; i < test_size; ++i) {
        v2.push_back(i);
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto preallocated_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  频繁扩容检查: " << frequent_realloc_duration.count() << " 微秒" << std::endl;
    std::cout << "  预分配容量: " << preallocated_duration.count() << " 微秒" << std::endl;
    std::cout << "  分支预测收益: " << (double)frequent_realloc_duration.count() / preallocated_duration.count() << "x" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 真实分支预测影响测试 ===" << std::endl;
    std::cout << "分析为什么之前的测试没有显示分支预测影响" << std::endl;
    
    try {
        test_branch_prediction_with_random_data();
        test_vector_branch_prediction_impact();
        test_compiler_optimization_levels();
        test_memory_access_patterns();
        test_vector_reallocate_branch_prediction();
        
        std::cout << "\n=== 分析结论 ===" << std::endl;
        std::cout << "1. 编译器优化：现代编译器会优化规律性分支" << std::endl;
        std::cout << "2. 分支预测器：现代CPU的分支预测器非常高效" << std::endl;
        std::cout << "3. 测试规模：需要足够大的随机数据才能看到差异" << std::endl;
        std::cout << "4. Vector实现：分支预测主要影响扩容检查，但影响较小" << std::endl;
        std::cout << "5. 实际建议：在vector实现中，分支预测不是主要性能瓶颈" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}




