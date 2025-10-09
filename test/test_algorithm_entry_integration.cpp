// 算法总入口集成测试：仅包含 algorithm.h，交叉验证所有组件可同时使用
#include <iostream>
#include <vector>
#include "algorithm.h"

int main() {
    // 集成：排序+旋转+计数
    std::vector<int> v{5,1,4,2,3};
    mystl::sort(v.begin(), v.end());
    if (!mystl::is_sorted(v.begin(), v.end())) return 1;

    mystl::rotate(v.begin(), v.begin()+2, v.end()); // [3,4,5,1,2]
    std::vector<int> expect{3,4,5,1,2};
    if (v != expect) return 2;

    auto c = mystl::count(v.begin(), v.end(), 4);
    if (c != 1) return 3;

    // 集成：堆 + 数值
    mystl::make_heap(v.begin(), v.end());
    if (!mystl::is_heap(v.begin(), v.end())) return 4;

    mystl::sort_heap(v.begin(), v.end());
    if (!mystl::is_sorted(v.begin(), v.end())) return 5;

    int sum = mystl::accumulate(v.begin(), v.end(), 0);
    if (sum != 15) return 6;

    std::cout << "test_algorithm_entry_integration: OK" << std::endl;
    return 0;
}


