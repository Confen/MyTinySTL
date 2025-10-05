#include <iostream>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector 第一阶段测试 ===" << std::endl;
    
    // 测试1：默认构造函数
    std::cout << "\n1. 测试默认构造函数：" << std::endl;
    vector<int> v1;
    std::cout << "   空vector - size: " << v1.size() 
              << ", capacity: " << v1.capacity() 
              << ", empty: " << (v1.empty() ? "true" : "false") << std::endl;
    
    // 测试2：指定大小的构造函数
    std::cout << "\n2. 测试指定大小的构造函数：" << std::endl;
    vector<int> v2(5);
    std::cout << "   vector<int> v2(5) - size: " << v2.size() 
              << ", capacity: " << v2.capacity() << std::endl;
    
    // 测试3：指定大小和值的构造函数
    std::cout << "\n3. 测试指定大小和值的构造函数：" << std::endl;
    vector<int> v3(3, 42);
    std::cout << "   vector<int> v3(3, 42) - size: " << v3.size() 
              << ", capacity: " << v3.capacity() << std::endl;
    
    // 测试4：拷贝构造函数
    std::cout << "\n4. 测试拷贝构造函数：" << std::endl;
    vector<int> v4(v3);
    std::cout << "   vector<int> v4(v3) - size: " << v4.size() 
              << ", capacity: " << v4.capacity() << std::endl;
    
    // 测试5：移动构造函数
    std::cout << "\n5. 测试移动构造函数：" << std::endl;
    vector<int> v5(std::move(v4));
    std::cout << "   vector<int> v5(std::move(v4)) - size: " << v5.size() 
              << ", capacity: " << v5.capacity() << std::endl;
    std::cout << "   原vector v4 - size: " << v4.size() 
              << ", capacity: " << v4.capacity() << std::endl;
    
    // 测试6：初始化列表构造函数
    std::cout << "\n6. 测试初始化列表构造函数：" << std::endl;
    vector<int> v6{1, 2, 3, 4, 5};
    std::cout << "   vector<int> v6{1,2,3,4,5} - size: " << v6.size() 
              << ", capacity: " << v6.capacity() << std::endl;
    
    // 测试7：push_back操作
    std::cout << "\n7. 测试push_back操作：" << std::endl;
    vector<int> v7;
    std::cout << "   初始状态 - size: " << v7.size() 
              << ", capacity: " << v7.capacity() << std::endl;
    
    v7.push_back(10);
    std::cout << "   添加1个元素后 - size: " << v7.size() 
              << ", capacity: " << v7.capacity() << std::endl;
    
    v7.push_back(20);
    std::cout << "   添加2个元素后 - size: " << v7.size() 
              << ", capacity: " << v7.capacity() << std::endl;
    
    v7.push_back(30);
    std::cout << "   添加3个元素后 - size: " << v7.size() 
              << ", capacity: " << v7.capacity() << std::endl;
    
    // 测试8：pop_back操作
    std::cout << "\n8. 测试pop_back操作：" << std::endl;
    std::cout << "   pop_back前 - size: " << v7.size() << std::endl;
    v7.pop_back();
    std::cout << "   pop_back后 - size: " << v7.size() << std::endl;
    
    // 测试9：容量管理
    std::cout << "\n9. 测试容量管理：" << std::endl;
    vector<int> v9;
    for (int i = 0; i < 10; ++i) {
        v9.push_back(i);
        std::cout << "   添加元素 " << i << " - size: " << v9.size() 
                  << ", capacity: " << v9.capacity() << std::endl;
    }
    
    // 测试10：异常安全
    std::cout << "\n10. 测试异常安全：" << std::endl;
    try {
        vector<int> v10;
        for (int i = 0; i < 1000; ++i) {
            v10.push_back(i);
        }
        std::cout << "   成功添加1000个元素 - size: " << v10.size() 
                  << ", capacity: " << v10.capacity() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== 第一阶段测试完成 ===" << std::endl;
    
    return 0;
}

