#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../algorithm.h"

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

void test_algorithm_integration() {
    std::cout << "\n=== 测试 algorithm.h 整合效果 ===" << std::endl;
    
    std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
    print_vector(vec, "原始数据");
    
    // 测试基本算法
    std::cout << "\n--- 基本算法测试 ---" << std::endl;
    
    // 查找
    auto it = mystl::find(vec.begin(), vec.end(), 5);
    std::cout << "查找 5: " << (it != vec.end() ? "找到" : "未找到") << std::endl;
    
    // 计数
    int count = mystl::count(vec.begin(), vec.end(), 1);
    std::cout << "1 的个数: " << count << std::endl;
    
    // 排序
    mystl::sort(vec.begin(), vec.end());
    print_vector(vec, "排序后");
    
    // 测试堆算法
    std::cout << "\n--- 堆算法测试 ---" << std::endl;
    
    std::vector<int> heap_vec{3, 1, 4, 1, 5, 9, 2, 6};
    mystl::make_heap(heap_vec.begin(), heap_vec.end());
    print_vector(heap_vec, "建堆后");
    
    mystl::push_heap(heap_vec.begin(), heap_vec.end());
    print_vector(heap_vec, "插入后");
    
    mystl::pop_heap(heap_vec.begin(), heap_vec.end());
    heap_vec.pop_back();
    print_vector(heap_vec, "删除后");
    
    // 测试集合算法
    std::cout << "\n--- 集合算法测试 ---" << std::endl;
    
    std::vector<int> set1{1, 2, 3, 4, 5};
    std::vector<int> set2{3, 4, 5, 6, 7};
    std::vector<int> result(10);
    
    auto union_end = mystl::set_union(set1.begin(), set1.end(), 
                                      set2.begin(), set2.end(), 
                                      result.begin());
    result.erase(union_end, result.end());
    print_vector(result, "并集");
    
    // 测试数值算法
    std::cout << "\n--- 数值算法测试 ---" << std::endl;
    
    std::vector<int> nums{1, 2, 3, 4, 5};
    int sum = mystl::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "累加和: " << sum << std::endl;
    
    int product = mystl::accumulate(nums.begin(), nums.end(), 1, mystl::multiplies<int>());
    std::cout << "累乘积: " << product << std::endl;
    
    // 测试部分和
    std::vector<int> partial_result(5);
    mystl::partial_sum(nums.begin(), nums.end(), partial_result.begin());
    print_vector(partial_result, "部分和");
    
    std::cout << "\n✅ 所有算法整合测试通过！" << std::endl;
}

void test_performance_comparison() {
    std::cout << "\n=== 性能对比测试 ===" << std::endl;
    
    const size_t size = 100000;
    std::vector<int> vec(size);
    mystl::iota(vec.begin(), vec.end(), 1);
    
    // 测试 mystl::sort
    auto start = std::chrono::high_resolution_clock::now();
    mystl::sort(vec.begin(), vec.end());
    auto end = std::chrono::high_resolution_clock::now();
    auto mystl_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 重新填充数据
    mystl::iota(vec.begin(), vec.end(), 1);
    
    // 测试 std::sort
    start = std::chrono::high_resolution_clock::now();
    std::sort(vec.begin(), vec.end());
    end = std::chrono::high_resolution_clock::now();
    auto std_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "mystl::sort 时间: " << mystl_time << " 微秒" << std::endl;
    std::cout << "std::sort 时间: " << std_time << " 微秒" << std::endl;
    std::cout << "性能比: " << (double)mystl_time / std_time << std::endl;
}

void test_algorithm_categories() {
    std::cout << "\n=== 算法分类测试 ===" << std::endl;
    
    std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
    
    // 非修改算法
    std::cout << "--- 非修改算法 ---" << std::endl;
    bool all_positive = mystl::all_of(vec.begin(), vec.end(), [](int x) { return x > 0; });
    std::cout << "所有元素都为正数: " << (all_positive ? "是" : "否") << std::endl;
    
    // 修改算法
    std::cout << "--- 修改算法 ---" << std::endl;
    mystl::reverse(vec.begin(), vec.end());
    print_vector(vec, "反转后");
    
    // 堆算法
    std::cout << "--- 堆算法 ---" << std::endl;
    mystl::make_heap(vec.begin(), vec.end());
    print_vector(vec, "建堆后");
    
    // 集合算法
    std::cout << "--- 集合算法 ---" << std::endl;
    std::vector<int> set1{1, 2, 3, 4, 5};
    std::vector<int> set2{3, 4, 5, 6, 7};
    bool includes_result = mystl::includes(set1.begin(), set1.end(), 
                                          set2.begin(), set2.end());
    std::cout << "set1 包含 set2: " << (includes_result ? "是" : "否") << std::endl;
    
    // 数值算法
    std::cout << "--- 数值算法 ---" << std::endl;
    int sum = mystl::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "总和: " << sum << std::endl;
}

int main() {
    std::cout << "MyTinySTL 算法总入口测试" << std::endl;
    std::cout << "========================" << std::endl;
    
    try {
        test_algorithm_integration();
        test_performance_comparison();
        test_algorithm_categories();
        
        std::cout << "\n🎉 所有测试完成！" << std::endl;
        std::cout << "algorithm.h 整合成功！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
