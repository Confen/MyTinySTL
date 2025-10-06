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
std::uniform_int_distribution<> dis(0, 99);

// 使用volatile防止编译器优化
volatile int global_sum = 0;

void test_memory_access_with_random_data() {
    std::cout << "\n=== 真实内存访问测试（随机数据）===" << std::endl;
    
    const size_t test_size = 10000000; // 增大测试规模
    std::vector<int> data(test_size);
    
    // 生成随机数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = dis(gen);
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
    global_sum = sum; // 防止编译器优化掉sum
    
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
    global_sum = sum; // 防止编译器优化掉sum
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序访问: " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机访问: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_memory_access_with_volatile() {
    std::cout << "\n=== 使用volatile防止优化的内存访问测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    std::vector<int> data(test_size);
    volatile int* volatile_data = data.data();
    
    // 生成随机数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = dis(gen);
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
    global_sum = sum;
    
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
    global_sum = sum;
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序访问(volatile): " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机访问(volatile): " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_memory_access_with_function_call() {
    std::cout << "\n=== 使用函数调用防止优化的内存访问测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    std::vector<int> data(test_size);
    
    // 生成随机数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = dis(gen);
    }
    
    // 定义函数，防止编译器优化
    auto process_element = [](int value) -> int {
        if (value < 50) {
            return value;
        } else {
            return -value;
        }
    };
    
    // 测试1：顺序访问
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        sum += process_element(data[i]);
    }
    global_sum = sum;
    
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
        sum += process_element(data[idx]);
    }
    global_sum = sum;
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序访问(函数调用): " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机访问(函数调用): " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_vector_memory_access_patterns() {
    std::cout << "\n=== Vector内存访问模式测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    
    // 测试1：顺序访问vector元素
    auto start = std::chrono::high_resolution_clock::now();
    
    vector<int> v1;
    for (size_t i = 0; i < test_size; ++i) {
        v1.push_back(i);
    }
    
    int sum = 0;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] < test_size / 2) {
            sum += v1[i];
        } else {
            sum -= v1[i];
        }
    }
    global_sum = sum;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试2：随机访问vector元素
    start = std::chrono::high_resolution_clock::now();
    
    vector<int> v2;
    for (size_t i = 0; i < test_size; ++i) {
        v2.push_back(i);
    }
    
    std::vector<size_t> indices(test_size);
    for (size_t i = 0; i < test_size; ++i) {
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), gen);
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        size_t idx = indices[i];
        if (v2[idx] < test_size / 2) {
            sum += v2[idx];
        } else {
            sum -= v2[idx];
        }
    }
    global_sum = sum;
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Vector顺序访问: " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  Vector随机访问: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_compiler_optimization_impact() {
    std::cout << "\n=== 编译器优化对内存访问的影响 ===" << std::endl;
    
    const size_t test_size = 1000000;
    std::vector<int> data(test_size);
    
    // 生成数据
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = i % 100;  // 规律性数据
    }
    
    // 测试1：规律性数据访问
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (data[i] < 50) {
            sum += data[i];
        } else {
            sum -= data[i];
        }
    }
    global_sum = sum;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto pattern_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试2：随机数据访问
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = dis(gen);  // 随机数据
    }
    
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (data[i] < 50) {
            sum += data[i];
        } else {
            sum -= data[i];
        }
    }
    global_sum = sum;
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  规律性数据访问: " << pattern_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机数据访问: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  优化影响: " << (double)pattern_duration.count() / random_duration.count() << "x" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 真实内存访问模式测试 ===" << std::endl;
    std::cout << "分析为什么之前的内存访问测试没有显示差异" << std::endl;
    
    try {
        test_memory_access_with_random_data();
        test_memory_access_with_volatile();
        test_memory_access_with_function_call();
        test_vector_memory_access_patterns();
        test_compiler_optimization_impact();
        
        std::cout << "\n=== 分析结论 ===" << std::endl;
        std::cout << "1. 编译器优化：现代编译器会优化规律性内存访问" << std::endl;
        std::cout << "2. 数据模式识别：编译器识别规律性数据并直接计算结果" << std::endl;
        std::cout << "3. 循环优化：编译器可能将整个循环优化为数学计算" << std::endl;
        std::cout << "4. 内存访问：顺序访问比随机访问快，但编译器优化掩盖了差异" << std::endl;
        std::cout << "5. 实际建议：在真实应用中，内存访问模式确实影响性能" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}
