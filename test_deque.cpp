#include "deque.h"
#include <iostream>
int main() {
    // 基本 push/pop 检查
    mystl::deque<int> dq;
    dq.push_back(1);
    dq.push_back(2);
    dq.push_front(0);   // [0,1,2]
    dq.pop_back();      // [0,1]
    dq.pop_front();     // [1]
    mystl::deque<int> d2;
    std::cout << "size=" << dq.size() << ", front=" << dq.front() << ", back=" << dq.back() << std::endl;
    mystl::deque<int> dq2;
    for (int i = 0; i < 200; ++i) dq2.push_back(i);   // 触发多次尾部跨块/扩容
    for (int i = 0; i < 150; ++i) dq2.push_front(-i); // 触发多次头部跨块/扩容
    std::cout << dq2.front() << " " << dq2.back() << " " << dq2.size() << std::endl;
    
        mystl::deque<int> dq3;
    {
        for (int i = 0; i < 100; ++i) dq3.push_back(i);  
        dq3.clear();
        std::cout << "after clear: size=" << dq3.size() << ", empty=" << dq3.empty() << std::endl;
        dq3.push_back(7);
        std::cout << "after reuse: size=" << dq3.size() << ", front=" << dq3.front() << ", back=" << dq3.back() << std::endl;
    }
        {
            mystl::deque<int> dq4;
            dq4.resize(3, 9);   // [9,9,9]
            dq4.resize(5);      // [9,9,9,0,0]
            dq4.resize(2);      // [9,9]
            std::cout << "resize: size=" << dq4.size()
                      << ", front=" << dq4.front()
                      << ", back="  << dq4.back() << std::endl; // 2, 9, 9
        }
        {
            mystl::deque<int> dq5;
            dq5.assign(3, 5);  // [5,5,5]
            std::cout << "assign1: size=" << dq5.size() << ", front=" << dq5.front() << ", back=" << dq5.back() << std::endl;
        
            int a[] = {1,2,3,4};
            dq5.assign(a+1, a+4); // [2,3,4]
            std::cout << "assign2: size=" << dq5.size() << ", front=" << dq5.front() << ", back=" << dq5.back() << std::endl;
        }

        {
            mystl::deque<int> dq6;
            for (int i = 0; i < 8; ++i) dq6.push_back(i);   // [0..7]
            auto it = dq6.begin() + 3;
            dq6.insert(it, 99);                             // [0,1,2,99,3,4,5,6,7]
            std::cout << dq6[3] << " " << dq6[4] << " " << dq6.size() << std::endl;
        
            it = dq6.begin() + 4;
            dq6.erase(it);                                  // 删除 3 -> [0,1,2,99,4,5,6,7]
            std::cout << dq6[3] << " " << dq6[4] << " " << dq6.size() << std::endl;
            // auto it1 = dq6.begin() + 2;
            // auto it2 = dq6.begin() + 5;                     // 删 [2,5): 删除 2,99,4
            // dq6.erase(it1, it2);                            // -> [0,1,5,6,7]
            // std::cout << dq6.front() << " " << dq6.back() << " " << dq6.size() << std::endl;
        }
        
        {
            //pair相当于一个元素，first和second是pair的成员变量
            mystl::deque<std::pair<int,int>> dq;
            dq.emplace_back(1,2);
            dq.emplace_front(0,1);
            auto it = dq.begin() + 1;
            dq.emplace(it, 7,8);
            std::cout << dq.size() << " "
                      << dq.front().first << "," << dq.front().second << " "
                      << dq.back().first  << "," << dq.back().second  << std::endl; // 3 0,1 1,2
        }
// // 触发跨块：大量 push 再 pop
// {
//     mystl::deque<int> dq;
//     for (int i = 0; i < 100; ++i) dq.push_back(i); // 触发多块与 map 扩容
//     for (int i = 0; i < 50; ++i) dq.pop_front();   // 削掉前半
//     for (int i = 0; i < 49; ++i) dq.pop_back();    // 再削到只剩一个
//     std::cout << "size=" << dq.size() << ", front=" << dq.front() << ", back=" << dq.back() << std::endl; // 应为 size=1 且 front==back==50
 }