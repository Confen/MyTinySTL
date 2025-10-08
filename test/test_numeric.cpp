#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include "numeric.h"
#include "functional.h"

// 测试辅助函数
template<typename T>
void print_vector(const std::vector<T>& vec, const std::string& name) {
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void test_accumulate() {
    std::cout << "\n=== 测试 accumulate 算法 ===" << std::endl;
    
    std::vector<int> vec{1, 2, 3, 4, 5};
    print_vector(vec, "输入向量");
    
    // 测试基本累加
    int sum1 = mystl::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "mystl::accumulate (sum): " << sum1 << std::endl;
    
    // 测试自定义操作（乘法）
    int product = mystl::accumulate(vec.begin(), vec.end(), 1, mystl::multiplies<int>());
    std::cout << "mystl::accumulate (product): " << product << std::endl;
    
    // 验证结果
    int expected_sum = 15;
    int expected_product = 120;
    std::cout << "✅ 累加测试: " << (sum1 == expected_sum ? "通过" : "失败") << std::endl;
    std::cout << "✅ 乘积测试: " << (product == expected_product ? "通过" : "失败") << std::endl;
}

void test_inner_product() {
    std::cout << "\n=== 测试 inner_product 算法 ===" << std::endl;
    
    std::vector<int> vec1{1, 2, 3, 4};
    std::vector<int> vec2{2, 3, 4, 5};
    print_vector(vec1, "向量1");
    print_vector(vec2, "向量2");
    
    // 测试基本内积
    int dot_product = mystl::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0);
    std::cout << "mystl::inner_product: " << dot_product << std::endl;
    
    // 测试自定义操作
    int custom_result = mystl::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0,
                                            mystl::plus<int>(), mystl::multiplies<int>());
    std::cout << "mystl::inner_product (custom): " << custom_result << std::endl;
    
    // 验证结果
    int expected = 1*2 + 2*3 + 3*4 + 4*5; // 2 + 6 + 12 + 20 = 40
    std::cout << "✅ 内积测试: " << (dot_product == expected ? "通过" : "失败") << std::endl;
}

void test_partial_sum() {
    std::cout << "\n=== 测试 partial_sum 算法 ===" << std::endl;
    
    std::vector<int> input{1, 2, 3, 4, 5};
    std::vector<int> output(5);
    print_vector(input, "输入向量");
    
    // 测试基本部分和
    mystl::partial_sum(input.begin(), input.end(), output.begin());
    print_vector(output, "部分和结果");
    
    // 验证结果
    std::vector<int> expected{1, 3, 6, 10, 15};
    bool passed = (output == expected);
    std::cout << "✅ 部分和测试: " << (passed ? "通过" : "失败") << std::endl;
    
    // 测试自定义操作（乘法）
    std::vector<int> output2(5);
    mystl::partial_sum(input.begin(), input.end(), output2.begin(), mystl::multiplies<int>());
    print_vector(output2, "部分积结果");
    
    std::vector<int> expected2{1, 2, 6, 24, 120};
    bool passed2 = (output2 == expected2);
    std::cout << "✅ 部分积测试: " << (passed2 ? "通过" : "失败") << std::endl;
}

void test_adjacent_difference() {
    std::cout << "\n=== 测试 adjacent_difference 算法 ===" << std::endl;
    
    std::vector<int> input{1, 3, 6, 10, 15};
    std::vector<int> output(5);
    print_vector(input, "输入向量");
    
    // 测试基本相邻差分
    mystl::adjacent_difference(input.begin(), input.end(), output.begin());
    print_vector(output, "相邻差分结果");
    
    // 验证结果
    std::vector<int> expected{1, 2, 3, 4, 5}; // 第一个元素保持不变，后续元素是相邻差值
    bool passed = (output == expected);
    std::cout << "✅ 相邻差分测试: " << (passed ? "通过" : "失败") << std::endl;
    
    // 测试自定义操作（除法）
    std::vector<int> input2{2, 4, 8, 16, 32};
    std::vector<int> output2(5);
    mystl::adjacent_difference(input2.begin(), input2.end(), output2.begin(), mystl::divides<int>());
    print_vector(output2, "相邻比值结果");
    
    std::vector<int> expected2{2, 2, 2, 2, 2}; // 2, 4/2=2, 8/4=2, 16/8=2, 32/16=2
    bool passed2 = (output2 == expected2);
    std::cout << "✅ 相邻比值测试: " << (passed2 ? "通过" : "失败") << std::endl;
}

void test_iota() {
    std::cout << "\n=== 测试 iota 算法 ===" << std::endl;
    
    std::vector<int> vec(5);
    print_vector(vec, "初始向量");
    
    // 测试序列填充
    mystl::iota(vec.begin(), vec.end(), 10);
    print_vector(vec, "iota结果");
    
    // 验证结果
    std::vector<int> expected{10, 11, 12, 13, 14};
    bool passed = (vec == expected);
    std::cout << "✅ iota测试: " << (passed ? "通过" : "失败") << std::endl;
}

void test_performance_comparison() {
    std::cout << "\n=== 性能对比测试 ===" << std::endl;
    
    const size_t size = 1000000;
    std::vector<int> vec(size);
    mystl::iota(vec.begin(), vec.end(), 1);
    
    // 测试 mystl::accumulate
    auto start = std::chrono::high_resolution_clock::now();
    long long sum1 = mystl::accumulate(vec.begin(), vec.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    auto mystl_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试 std::accumulate
    start = std::chrono::high_resolution_clock::now();
    long long sum2 = std::accumulate(vec.begin(), vec.end(), 0LL);
    end = std::chrono::high_resolution_clock::now();
    auto std_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "mystl::accumulate 时间: " << mystl_time << " 微秒" << std::endl;
    std::cout << "std::accumulate 时间: " << std_time << " 微秒" << std::endl;
    std::cout << "结果一致性: " << (sum1 == sum2 ? "✅ 一致" : "❌ 不一致") << std::endl;
}

int main() {
    std::cout << "MyTinySTL 数值算法测试" << std::endl;
    std::cout << "========================" << std::endl;
    
    try {
        test_accumulate();
        test_inner_product();
        test_partial_sum();
        test_adjacent_difference();
        test_iota();
        test_performance_comparison();
        
        std::cout << "\n🎉 所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}


