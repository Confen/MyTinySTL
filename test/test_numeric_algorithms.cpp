// 数值算法测试：accumulate / inner_product / partial_sum / adjacent_difference / iota
#include <iostream>
#include <vector>
#include "numeric.h"

int main() {
    // accumulate
    {
        std::vector<int> v{1,2,3,4,5};
        int s1 = mystl::accumulate(v.begin(), v.end(), 0);
        if (s1 != 15) return 1;
        int s2 = mystl::accumulate(v.begin(), v.end(), 1, [](int a,int b){ return a*b; });
        if (s2 != 120) return 2;
    }

    // inner_product
    {
        std::vector<int> a{1,2,3};
        std::vector<int> b{4,5,6};
        int ip = mystl::inner_product(a.begin(), a.end(), b.begin(), 0);
        if (ip != 32) return 3;
    }

    // partial_sum
    {
        std::vector<int> v{1,2,3,4};
        std::vector<int> out(4);
        mystl::partial_sum(v.begin(), v.end(), out.begin());
        std::vector<int> expect{1,3,6,10};
        if (out != expect) return 4;
    }

    // adjacent_difference
    {
        std::vector<int> v{1,3,6,10};
        std::vector<int> out(4);
        mystl::adjacent_difference(v.begin(), v.end(), out.begin());
        std::vector<int> expect{1,2,3,4};
        if (out != expect) return 5;
    }

    // iota
    {
        std::vector<int> v(5);
        mystl::iota(v.begin(), v.end(), 10);
        std::vector<int> expect{10,11,12,13,14};
        if (v != expect) return 6;
    }

    std::cout << "test_numeric_algorithms: OK" << std::endl;
    return 0;
}


