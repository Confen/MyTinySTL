#include <iostream>
#include <vector>
#include "../heap_algo.h"

using namespace mystl;

int main() {
    
    std::cout << "=== MyTinySTL 堆算法测试 ===" << std::endl;
    
    try {
        // 测试1：make_heap
        std::cout << "\n1. 测试make_heap：" << std::endl;
        std::vector<int> v1{3, 1, 4, 1, 5, 9, 2, 6};
        std::cout << "   原始数组: ";
        for (int x : v1) std::cout << x << " ";
        std::cout << std::endl;
        
        mystl::make_heap(v1.begin(), v1.end());
        std::cout << "   建堆后: ";
        for (int x : v1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "   是否为堆: " << (mystl::is_heap(v1.begin(), v1.end()) ? "是" : "否") << std::endl;
        
        // 测试2：push_heap
        std::cout << "\n2. 测试push_heap：" << std::endl;
        v1.push_back(8);
        mystl::push_heap(v1.begin(), v1.end());
        std::cout << "   添加8后: ";
        for (int x : v1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "   是否为堆: " << (mystl::is_heap(v1.begin(), v1.end()) ? "是" : "否") << std::endl;
        
        // 测试3：pop_heap
        std::cout << "\n3. 测试pop_heap：" << std::endl;
        mystl::pop_heap(v1.begin(), v1.end());
        int max_val = v1.back();
        v1.pop_back();
        std::cout << "   弹出最大值: " << max_val << std::endl;
        std::cout << "   剩余元素: ";
        for (int x : v1) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "   是否为堆: " << (mystl::is_heap(v1.begin(), v1.end()) ? "是" : "否") << std::endl;
        
        // 测试4：sort_heap
        std::cout << "\n4. 测试sort_heap：" << std::endl;
        mystl::sort_heap(v1.begin(), v1.end());
        std::cout << "   堆排序后: ";
        for (int x : v1) std::cout << x << " ";
        std::cout << std::endl;
        
        // 测试5：is_heap_until
        std::cout << "\n5. 测试is_heap_until：" << std::endl;
        std::vector<int> v2{9, 5, 4, 1, 1, 3, 2, 6};
        auto it = mystl::is_heap_until(v2.begin(), v2.end());
        std::cout << "   数组: ";
        for (int x : v2) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "   第一个不满足堆性质的位置: " << (it - v2.begin()) << std::endl;
        
        std::cout << "\n=== 所有测试完成 ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}