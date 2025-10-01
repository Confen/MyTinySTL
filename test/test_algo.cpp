#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "../algo.h"
#include "../algobase.h"
#include "../iterator.h"

using namespace mystl;

// 测试用的辅助函数
void print_vector(const std::vector<int>& vec, const std::string& name) {
    std::cout << name << ": ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// 测试 all_of, any_of, none_of
void test_predicate_algorithms() {
    std::cout << "=== 测试谓词算法 ===" << std::endl;
    
    std::vector<int> vec1 = {2, 4, 6, 8, 10};
    std::vector<int> vec2 = {1, 3, 5, 7, 9};
    std::vector<int> vec3 = {1, 2, 3, 4, 5};
    
    auto is_even = [](int x) { return x % 2 == 0; };
    auto is_odd = [](int x) { return x % 2 == 1; };
    
    // 测试 all_of
    assert(all_of(vec1.begin(), vec1.end(), is_even) == true);
    assert(all_of(vec2.begin(), vec2.end(), is_even) == false);
    assert(all_of(vec3.begin(), vec3.end(), is_even) == false);
    
    // 测试 any_of
    assert(any_of(vec1.begin(), vec1.end(), is_even) == true);
    assert(any_of(vec2.begin(), vec2.end(), is_even) == false);
    assert(any_of(vec3.begin(), vec3.end(), is_even) == true);
    
    // 测试 none_of
    assert(none_of(vec1.begin(), vec1.end(), is_odd) == true);
    assert(none_of(vec2.begin(), vec2.end(), is_odd) == false);
    assert(none_of(vec3.begin(), vec3.end(), is_odd) == false);
    
    std::cout << "谓词算法测试通过！" << std::endl;
}

// 测试 count 和 count_if
void test_count_algorithms() {
    std::cout << "=== 测试计数算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 2, 4, 2, 5};
    
    // 测试 count
    assert(count(vec.begin(), vec.end(), 2) == 3);
    assert(count(vec.begin(), vec.end(), 1) == 1);
    assert(count(vec.begin(), vec.end(), 6) == 0);
    
    // 测试 count_if
    auto is_even = [](int x) { return x % 2 == 0; };
    assert(count_if(vec.begin(), vec.end(), is_even) == 3);
    
    auto is_greater_than_3 = [](int x) { return x > 3; };
    assert(count_if(vec.begin(), vec.end(), is_greater_than_3) == 2);
    
    std::cout << "计数算法测试通过！" << std::endl;
}

// 测试 find 系列算法
void test_find_algorithms() {
    std::cout << "=== 测试查找算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 3, 5, 7, 9, 2, 4, 6, 8};
    
    // 测试 find
    auto it1 = find(vec.begin(), vec.end(), 5);
    assert(it1 != vec.end() && *it1 == 5);
    
    auto it2 = find(vec.begin(), vec.end(), 10);
    assert(it2 == vec.end());
    
    // 测试 find_if
    auto is_even = [](int x) { return x % 2 == 0; };
    auto it3 = find_if(vec.begin(), vec.end(), is_even);
    assert(it3 != vec.end() && *it3 == 2);
    
    // 测试 find_if_not
    auto it4 = find_if_not(vec.begin(), vec.end(), is_even);
    assert(it4 != vec.end() && *it4 == 1);
    
    std::cout << "查找算法测试通过！" << std::endl;
}

// 测试 search 系列算法
void test_search_algorithms() {
    std::cout << "=== 测试搜索算法 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> vec2 = {3, 4, 5};
    std::vector<int> vec3 = {5, 6, 7};
    std::vector<int> vec4 = {9, 8, 7};
    
    // 测试 search
    auto it1 = search(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    assert(it1 != vec1.end() && *it1 == 3);
    
    auto it2 = search(vec1.begin(), vec1.end(), vec3.begin(), vec3.end());
    assert(it2 != vec1.end() && *it2 == 5);
    
    auto it3 = search(vec1.begin(), vec1.end(), vec4.begin(), vec4.end());
    assert(it3 == vec1.end());
    
    // 测试 search_n
    std::vector<int> vec5 = {1, 2, 2, 2, 3, 4, 5};
    auto it4 = search_n(vec5.begin(), vec5.end(), 3, 2);
    assert(it4 != vec5.end() && *it4 == 2);
    
    auto it5 = search_n(vec5.begin(), vec5.end(), 4, 2);
    assert(it5 == vec5.end());
    
    std::cout << "搜索算法测试通过！" << std::endl;
}

// 测试 for_each
void test_for_each() {
    std::cout << "=== 测试 for_each ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> result;
    
    auto add_one = [&result](int x) { result.push_back(x + 1); };
    for_each(vec.begin(), vec.end(), add_one);
    
    assert(result.size() == 5);
    assert(result[0] == 2 && result[1] == 3 && result[2] == 4 && result[3] == 5 && result[4] == 6);
    
    std::cout << "for_each 测试通过！" << std::endl;
}

// 测试 adjacent_find
void test_adjacent_find() {
    std::cout << "=== 测试 adjacent_find ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 3, 4, 5};
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    
    auto it1 = adjacent_find(vec1.begin(), vec1.end());
    assert(it1 != vec1.end() && *it1 == 3);
    
    auto it2 = adjacent_find(vec2.begin(), vec2.end());
    assert(it2 == vec2.end());
    
    std::cout << "adjacent_find 测试通过！" << std::endl;
}

// 测试二分查找算法
void test_binary_search_algorithms() {
    std::cout << "=== 测试二分查找算法 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 测试 lower_bound
    auto it1 = lower_bound(vec.begin(), vec.end(), 5);
    assert(it1 != vec.end() && *it1 == 5);
    
    auto it2 = lower_bound(vec.begin(), vec.end(), 11);
    assert(it2 == vec.end());
    
    // 测试 upper_bound
    auto it3 = upper_bound(vec.begin(), vec.end(), 5);
    assert(it3 != vec.end() && *it3 == 6);
    
    auto it4 = upper_bound(vec.begin(), vec.end(), 10);
    assert(it4 == vec.end());
    
    // 测试 binary_search
    assert(binary_search(vec.begin(), vec.end(), 5) == true);
    assert(binary_search(vec.begin(), vec.end(), 11) == false);
    
    // 测试 equal_range
    auto range = equal_range(vec.begin(), vec.end(), 5);
    assert(range.first != vec.end() && *range.first == 5);
    assert(range.second != vec.end() && *range.second == 6);
    
    std::cout << "二分查找算法测试通过！" << std::endl;
}

// 测试 generate 系列算法
void test_generate_algorithms() {
    std::cout << "=== 测试生成算法 ===" << std::endl;
    
    std::vector<int> vec(5);
    int counter = 1;
    
    auto gen = [&counter]() { return counter++; };
    generate(vec.begin(), vec.end(), gen);
    
    assert(vec[0] == 1 && vec[1] == 2 && vec[2] == 3 && vec[3] == 4 && vec[4] == 5);
    
    std::vector<int> vec2(3);
    generate_n(vec2.begin(), 3, gen);
    
    assert(vec2[0] == 6 && vec2[1] == 7 && vec2[2] == 8);
    
    std::cout << "生成算法测试通过！" << std::endl;
}

// 测试 is_sorted
void test_is_sorted() {
    std::cout << "=== 测试 is_sorted ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {5, 4, 3, 2, 1};
    std::vector<int> vec3 = {1, 3, 2, 4, 5};
    
    assert(is_sorted(vec1.begin(), vec1.end()) == true);
    assert(is_sorted(vec2.begin(), vec2.end()) == false);
    assert(is_sorted(vec3.begin(), vec3.end()) == false);
    
    auto greater = [](int a, int b) { return a > b; };
    assert(is_sorted(vec2.begin(), vec2.end(), greater) == true);
    
    std::cout << "is_sorted 测试通过！" << std::endl;
}

// 测试 max_element 和 min_element
void test_minmax_element() {
    std::cout << "=== 测试 minmax_element ===" << std::endl;
    
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    
    auto max_it = max_element(vec.begin(), vec.end());
    assert(max_it != vec.end() && *max_it == 9);
    
    auto min_it = min_element(vec.begin(), vec.end());
    assert(min_it != vec.end() && *min_it == 1);
    
    auto greater = [](int a, int b) { return a > b; };
    auto max_it2 = max_element(vec.begin(), vec.end(), greater);
    assert(max_it2 != vec.end() && *max_it2 == 1);
    
    std::cout << "minmax_element 测试通过！" << std::endl;
}

// 测试 transform
void test_transform() {
    std::cout << "=== 测试 transform ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {2, 3, 4, 5, 6};
    std::vector<int> result(5);
    
    // 测试一元 transform
    auto square = [](int x) { return x * x; };
    transform(vec1.begin(), vec1.end(), result.begin(), square);
    
    assert(result[0] == 1 && result[1] == 4 && result[2] == 9 && result[3] == 16 && result[4] == 25);
    
    // 测试二元 transform
    std::vector<int> result2(5);
    auto add = [](int a, int b) { return a + b; };
    transform(vec1.begin(), vec1.end(), vec2.begin(), result2.begin(), add);
    
    assert(result2[0] == 3 && result2[1] == 5 && result2[2] == 7 && result2[3] == 9 && result2[4] == 11);
    
    std::cout << "transform 测试通过！" << std::endl;
}

// 测试 reverse
void test_reverse() {
    std::cout << "=== 测试 reverse ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    reverse(vec.begin(), vec.end());
    
    assert(vec[0] == 5 && vec[1] == 4 && vec[2] == 3 && vec[3] == 2 && vec[4] == 1);
    
    std::cout << "reverse 测试通过！" << std::endl;
}

// 测试 merge
void test_merge() {
    std::cout << "=== 测试 merge ===" << std::endl;
    
    std::vector<int> vec1 = {1, 3, 5, 7, 9};
    std::vector<int> vec2 = {2, 4, 6, 8, 10};
    std::vector<int> result(10);
    
    merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
    
    assert(result[0] == 1 && result[1] == 2 && result[2] == 3 && result[3] == 4 && result[4] == 5);
    assert(result[5] == 6 && result[6] == 7 && result[7] == 8 && result[8] == 9 && result[9] == 10);
    
    std::cout << "merge 测试通过！" << std::endl;
}

// 测试 equal
void test_equal() {
    std::cout << "=== 测试 equal ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    std::vector<int> vec3 = {1, 2, 3, 4, 6};
    
    assert(equal(vec1.begin(), vec1.end(), vec2.begin()) == true);
    assert(equal(vec1.begin(), vec1.end(), vec3.begin()) == false);
    
    std::cout << "equal 测试通过！" << std::endl;
}

// 测试 lexicographical_compare
void test_lexicographical_compare() {
    std::cout << "=== 测试 lexicographical_compare ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = {1, 2, 4};
    std::vector<int> vec3 = {1, 2, 3, 4};
    
    assert(lexicographical_compare(vec1.begin(), vec1.end(), vec2.begin(), vec2.end()) == true);
    assert(lexicographical_compare(vec2.begin(), vec2.end(), vec1.begin(), vec1.end()) == false);
    assert(lexicographical_compare(vec1.begin(), vec1.end(), vec3.begin(), vec3.end()) == true);
    
    std::cout << "lexicographical_compare 测试通过！" << std::endl;
}

int main() {
    std::cout << "开始测试 MyTinySTL 基本算法库..." << std::endl;
    
    try {
        test_predicate_algorithms();
        test_count_algorithms();
        test_find_algorithms();
        test_search_algorithms();
        test_for_each();
        test_adjacent_find();
        test_binary_search_algorithms();
        test_generate_algorithms();
        test_is_sorted();
        test_minmax_element();
        test_transform();
        test_reverse();
        test_merge();
        test_equal();
        test_lexicographical_compare();
        
        std::cout << "\n所有基本算法测试通过！" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

