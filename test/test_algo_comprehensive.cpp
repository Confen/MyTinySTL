#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

#include "../algo.h"
#include "../algobase.h"
#include "../functional.h"
#include "../util.h"

using namespace mystl;

// 测试辅助函数
template<typename Container>
void print_container(const Container& c, const std::string& name) {
    std::cout << name << ": ";
    for (const auto& elem : c) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// 测试非修改性算法
void test_non_modifying_algorithms() {
    std::cout << "=== 测试非修改性算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_container(vec, "原始容器");
    
    // 测试 all_of
    bool all_even = all_of(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    std::cout << "所有元素都是偶数: " << (all_even ? "是" : "否") << std::endl;
    
    // 测试 any_of
    bool any_even = any_of(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    std::cout << "存在偶数元素: " << (any_even ? "是" : "否") << std::endl;
    
    // 测试 none_of
    bool none_negative = none_of(vec.begin(), vec.end(), [](int x) { return x < 0; });
    std::cout << "没有负数元素: " << (none_negative ? "是" : "否") << std::endl;
    
    // 测试 count
    size_t count_even = count(vec.begin(), vec.end(), 2);
    std::cout << "数字2的个数: " << count_even << std::endl;
    
    // 测试 count_if
    size_t count_even_numbers = count_if(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    std::cout << "偶数个数: " << count_even_numbers << std::endl;
    
    // 测试 find
    auto it = find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "找到数字5，位置: " << (it - vec.begin()) << std::endl;
    }
    
    // 测试 find_if
    auto it_even = find_if(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    if (it_even != vec.end()) {
        std::cout << "找到第一个偶数: " << *it_even << std::endl;
    }
    
    // 测试 find_if_not
    auto it_odd = find_if_not(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    if (it_odd != vec.end()) {
        std::cout << "找到第一个奇数: " << *it_odd << std::endl;
    }
    
    std::cout << std::endl;
}

// 测试二分查找算法
void test_binary_search_algorithms() {
    std::cout << "=== 测试二分查找算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    print_container(vec, "已排序容器");
    
    // 测试 lower_bound
    auto it_lower = lower_bound(vec.begin(), vec.end(), 8);
    if (it_lower != vec.end()) {
        std::cout << "lower_bound(8) 找到: " << *it_lower << std::endl;
    }
    
    // 测试 upper_bound
    auto it_upper = upper_bound(vec.begin(), vec.end(), 8);
    if (it_upper != vec.end()) {
        std::cout << "upper_bound(8) 找到: " << *it_upper << std::endl;
    }
    
    // 测试 binary_search
    bool found = binary_search(vec.begin(), vec.end(), 7);
    std::cout << "binary_search(7): " << (found ? "找到" : "未找到") << std::endl;
    
    found = binary_search(vec.begin(), vec.end(), 8);
    std::cout << "binary_search(8): " << (found ? "找到" : "未找到") << std::endl;
    
    std::cout << std::endl;
}

// 测试生成算法
void test_generating_algorithms() {
    std::cout << "=== 测试生成算法 ===" << std::endl;
    
    std::vector<int> vec(10);
    print_container(vec, "生成前");
    
    // 测试 generate
    int counter = 1;
    generate(vec.begin(), vec.end(), [&counter]() { return counter++; });
    print_container(vec, "generate后");
    
    // 测试 generate_n
    std::vector<int> vec2(5);
    generate_n(vec2.begin(), 5, [&counter]() { return counter++; });
    print_container(vec2, "generate_n后");
    
    std::cout << std::endl;
}

// 测试排序相关算法
void test_sorting_algorithms() {
    std::cout << "=== 测试排序相关算法 ===" << std::endl;
    
    std::vector<int> vec = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    print_container(vec, "排序前");
    
    // 测试 is_sorted
    bool sorted = is_sorted(vec.begin(), vec.end());
    std::cout << "是否已排序: " << (sorted ? "是" : "否") << std::endl;
    
    // 测试 max_element
    auto max_it = max_element(vec.begin(), vec.end());
    std::cout << "最大元素: " << *max_it << std::endl;
    
    // 测试 min_element
    auto min_it = min_element(vec.begin(), vec.end());
    std::cout << "最小元素: " << *min_it << std::endl;
    
    // 测试 sort
    sort(vec.begin(), vec.end());
    print_container(vec, "sort后");
    
    // 测试 is_sorted
    sorted = is_sorted(vec.begin(), vec.end());
    std::cout << "排序后是否已排序: " << (sorted ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

// 测试变换算法
void test_transforming_algorithms() {
    std::cout << "=== 测试变换算法 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {2, 3, 4, 5, 6};
    std::vector<int> result(5);
    
    print_container(vec1, "输入1");
    print_container(vec2, "输入2");
    
    // 测试 transform (一元操作)
    transform(vec1.begin(), vec1.end(), result.begin(), [](int x) { return x * 2; });
    print_container(result, "transform(一元)后");
    
    // 测试 transform (二元操作)
    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(), 
              [](int x, int y) { return x + y; });
    print_container(result, "transform(二元)后");
    
    std::cout << std::endl;
}

// 测试反转算法
void test_reversing_algorithms() {
    std::cout << "=== 测试反转算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> result(5);
    
    print_container(vec, "反转前");
    
    // 测试 reverse
    reverse(vec.begin(), vec.end());
    print_container(vec, "reverse后");
    
    // 测试 reverse_copy
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    reverse_copy(vec2.begin(), vec2.end(), result.begin());
    print_container(vec2, "reverse_copy输入");
    print_container(result, "reverse_copy输出");
    
    std::cout << std::endl;
}

// 测试随机算法
void test_random_algorithms() {
    std::cout << "=== 测试随机算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_container(vec, "随机前");
    
    // 测试 random_shuffle
    random_shuffle(vec.begin(), vec.end());
    print_container(vec, "random_shuffle后");
    
    std::cout << std::endl;
}

// 测试修改性算法
void test_modifying_algorithms() {
    std::cout << "=== 测试修改性算法 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2(5);
    std::vector<int> vec3(5);
    
    print_container(vec1, "输入");
    
    // 测试 copy
    copy(vec1.begin(), vec1.end(), vec2.begin());
    print_container(vec2, "copy后");
    
    // 测试 move
    move(vec1.begin(), vec1.end(), vec3.begin());
    print_container(vec3, "move后");
    
    // 测试 fill
    fill(vec1.begin(), vec1.end(), 0);
    print_container(vec1, "fill(0)后");
    
    // 测试 fill_n
    fill_n(vec1.begin(), 3, 1);
    print_container(vec1, "fill_n(3, 1)后");
    
    std::cout << std::endl;
}

// 测试分区算法
void test_partitioning_algorithms() {
    std::cout << "=== 测试分区算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_container(vec, "分区前");
    
    // 测试 partition
    auto it = partition(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    print_container(vec, "partition(偶数)后");
    std::cout << "分区点位置: " << (it - vec.begin()) << std::endl;
    
    // 测试 is_partitioned
    bool partitioned = is_partitioned(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; });
    std::cout << "是否已分区: " << (partitioned ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

// 测试堆算法
void test_heap_algorithms() {
    std::cout << "=== 测试堆算法 ===" << std::endl;
    
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    print_container(vec, "堆操作前");
    
    // 测试 make_heap
    make_heap(vec.begin(), vec.end());
    print_container(vec, "make_heap后");
    
    // 测试 push_heap
    vec.push_back(8);
    push_heap(vec.begin(), vec.end());
    print_container(vec, "push_heap(8)后");
    
    // 测试 pop_heap
    pop_heap(vec.begin(), vec.end());
    vec.pop_back();
    print_container(vec, "pop_heap后");
    
    // 测试 sort_heap
    sort_heap(vec.begin(), vec.end());
    print_container(vec, "sort_heap后");
    
    std::cout << std::endl;
}

// 测试合并算法
void test_merging_algorithms() {
    std::cout << "=== 测试合并算法 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 3, 5, 7, 9};
    std::vector<int> vec2 = {2, 4, 6, 8, 10};
    std::vector<int> result(10);
    
    print_container(vec1, "序列1");
    print_container(vec2, "序列2");
    
    // 测试 merge
    merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
    print_container(result, "merge后");
    
    // 测试 inplace_merge
    std::vector<int> vec3 = {1, 3, 5, 2, 4, 6};
    print_container(vec3, "inplace_merge前");
    inplace_merge(vec3.begin(), vec3.begin() + 3, vec3.end());
    print_container(vec3, "inplace_merge后");
    
    std::cout << std::endl;
}

// 测试旋转算法
void test_rotating_algorithms() {
    std::cout << "=== 测试旋转算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_container(vec, "旋转前");
    
    // 测试 rotate
    rotate(vec.begin(), vec.begin() + 3, vec.end());
    print_container(vec, "rotate(左移3位)后");
    
    std::cout << std::endl;
}

// 性能测试
void test_performance() {
    std::cout << "=== 性能测试 ===" << std::endl;
    
    const size_t size = 100000;
    std::vector<int> vec(size);
    
    // 生成随机数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    for (size_t i = 0; i < size; ++i) {
        vec[i] = dis(gen);
    }
    
    // 测试排序性能
    auto start = std::chrono::high_resolution_clock::now();
    sort(vec.begin(), vec.end());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "排序 " << size << " 个元素耗时: " << duration.count() << " 毫秒" << std::endl;
    
    // 验证排序结果
    bool is_sorted_result = is_sorted(vec.begin(), vec.end());
    std::cout << "排序结果验证: " << (is_sorted_result ? "正确" : "错误") << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "MyTinySTL 算法库综合测试" << std::endl;
    std::cout << "=========================" << std::endl;
    
    try {
        test_non_modifying_algorithms();
        test_binary_search_algorithms();
        test_generating_algorithms();
        test_sorting_algorithms();
        test_transforming_algorithms();
        test_reversing_algorithms();
        test_random_algorithms();
        test_modifying_algorithms();
        test_partitioning_algorithms();
        test_heap_algorithms();
        test_merging_algorithms();
        test_rotating_algorithms();
        test_performance();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}












