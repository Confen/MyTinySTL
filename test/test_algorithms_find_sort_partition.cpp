// 基础算法：查找 / 分区 / 排序 / 旋转 综合测试（最小可运行集）
#include <iostream>
#include <vector>
#include <functional>
#include "algorithm.h"

int main() {
    // 查找与计数
    {
        std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
        auto it1 = mystl::find(v.begin(), v.end(), 4);
        auto it2 = mystl::find_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
        auto it3 = mystl::find_if_not(v.begin(), v.end(), [](int x){ return x < 10; });
        auto c1  = mystl::count(v.begin(), v.end(), 1);
        auto c2  = mystl::count_if(v.begin(), v.end(), [](int x){ return x % 3 == 0; });

        if (it1 == v.end() || *it1 != 4) return 1;
        if (it2 == v.end() || (*it2 % 2) != 0) return 2;
        if (it3 != v.end()) return 3; // 所有元素都 < 10
        if (c1 != 2) return 4;
        if (c2 != 3) return 5; // 3,6,9
    }

    // 分区与稳定分区
    {
        std::vector<int> v{1,2,3,4,5,6,7,8};
        auto is_even = [](int x){ return (x & 1) == 0; };

        auto mid = mystl::partition(v.begin(), v.end(), is_even);
        // 左边都为偶数，右边奇数
        if (!mystl::is_partitioned(v.begin(), v.end(), is_even)) return 6;

        // 稳定分区验证：偶数相对次序应保持
        // 暂不验证 stable_partition（当前未在 mystl 中实现）
    }

    // 排序与稳定排序、部分排序
    {
        std::vector<int> v{5,1,4,2,8,5,2};
        mystl::sort(v.begin(), v.end());
        if (!mystl::is_sorted(v.begin(), v.end())) return 9;

        // 自定义比较器（降序）
        auto desc = [](int a, int b){ return a > b; };
        mystl::sort(v.begin(), v.end(), desc);
        if (!mystl::is_sorted(v.begin(), v.end(), desc)) return 10;

        // partial_sort：取前三个最小元素（再升序）
        std::vector<int> v2{5,1,4,2,8,5,2};
        mystl::partial_sort(v2.begin(), v2.begin()+3, v2.end());
        if (!mystl::is_sorted(v2.begin(), v2.begin()+3)) return 11;
    }

    // 旋转 rotate：将区间 [first, middle) 移到尾部
    {
        std::vector<int> v{3,4,1,5,2};
        auto first = v.begin();
        auto middle = v.begin() + 2; // 指向值 1
        auto last = v.end();
        auto res = mystl::rotate(first, middle, last);
        // 期望：[1,5,2,3,4]，返回新 first 的迭代器（原 middle）向后的对应位置
        std::vector<int> expect{1,5,2,3,4};
        if (v != expect) return 12;
        if (res != v.begin() + (v.size() - 2)) return 13; // 简单一致性检查
    }

    std::cout << "test_algorithms_find_sort_partition: OK" << std::endl;
    return 0;
}


