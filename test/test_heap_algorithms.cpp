// 堆算法测试：make_heap / push_heap / pop_heap / sort_heap / is_heap / is_heap_until
#include <iostream>
#include <vector>
#include "heap_algo.h"
#include "algo.h"

int main() {
    // make_heap & is_heap
    {
        std::vector<int> v{3,1,4,1,5,9,2,6};
        mystl::make_heap(v.begin(), v.end());
        if (!mystl::is_heap(v.begin(), v.end())) return 1;
    }

    // push_heap & pop_heap
    {
        std::vector<int> v{3,1,4,1,5};
        mystl::make_heap(v.begin(), v.end());
        v.push_back(9);
        mystl::push_heap(v.begin(), v.end());
        if (!mystl::is_heap(v.begin(), v.end())) return 2;

        mystl::pop_heap(v.begin(), v.end()); // 最大值到末尾
        if (v.back() != 9) return 3;
        v.pop_back();
        if (!mystl::is_heap(v.begin(), v.end())) return 4;
    }

    // sort_heap：输出为升序
    {
        std::vector<int> v{5,3,8,1,2,7};
        mystl::make_heap(v.begin(), v.end());
        mystl::sort_heap(v.begin(), v.end());
        if (!mystl::is_sorted(v.begin(), v.end())) return 5;
    }

    // 自定义比较器（最小堆）
    {
        std::vector<int> v{5,3,8,1,2,7};
        auto comp = [](int a, int b){ return a > b; };
        mystl::make_heap(v.begin(), v.end(), comp);
        if (!mystl::is_heap(v.begin(), v.end(), comp)) return 6;
    }

    std::cout << "test_heap_algorithms: OK" << std::endl;
    return 0;
}


