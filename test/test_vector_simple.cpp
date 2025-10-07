#include <iostream>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector 简单测试 ===" << std::endl;
    
    try {
        // 测试1：默认构造
        std::cout << "\n1. 测试默认构造：" << std::endl;
        vector<int> v1;
        std::cout << "   空vector - size: " << v1.size() 
                  << ", capacity: " << v1.capacity() 
                  << ", empty: " << (v1.empty() ? "true" : "false") << std::endl;
        
        // 测试2：指定大小构造
        std::cout << "\n2. 测试指定大小构造：" << std::endl;
        vector<int> v2(5);
        std::cout << "   vector<int> v2(5) - size: " << v2.size() 
                  << ", capacity: " << v2.capacity() << std::endl;
        
        // 测试3：指定大小和值构造
        std::cout << "\n3. 测试指定大小和值构造：" << std::endl;
        vector<int> v3(3, 42);
        std::cout << "   vector<int> v3(3, 42) - size: " << v3.size() 
                  << ", capacity: " << v3.capacity() << std::endl;
        
        // 测试4：初始化列表构造
        std::cout << "\n4. 测试初始化列表构造：" << std::endl;
        vector<int> v4{1, 2, 3, 4, 5};
        std::cout << "   vector<int> v4{1,2,3,4,5} - size: " << v4.size() 
                  << ", capacity: " << v4.capacity() << std::endl;
        
        // 测试5：push_back操作
        std::cout << "\n5. 测试push_back操作：" << std::endl;
        vector<int> v5;
        for (int i = 1; i <= 10; ++i) {
            v5.push_back(i);
            std::cout << "   添加元素 " << i << " - size: " << v5.size() 
                      << ", capacity: " << v5.capacity() << std::endl;
        }
        
        // 测试6：pop_back操作
        std::cout << "\n6. 测试pop_back操作：" << std::endl;
        for (int i = 0; i < 5; ++i) {
            v5.pop_back();
            std::cout << "   删除元素后 - size: " << v5.size() 
                      << ", capacity: " << v5.capacity() << std::endl;
        }
        
        // 测试7：拷贝构造
        std::cout << "\n7. 测试拷贝构造：" << std::endl;
        vector<int> v6(v5);
        std::cout << "   原vector - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
        std::cout << "   拷贝vector - size: " << v6.size() << ", capacity: " << v6.capacity() << std::endl;
        
        // 测试8：移动构造
        std::cout << "\n8. 测试移动构造：" << std::endl;
        vector<int> v7{6, 7, 8, 9, 10};
        vector<int> v8(std::move(v7));
        std::cout << "   原vector - size: " << v7.size() << ", capacity: " << v7.capacity() << std::endl;
        std::cout << "   移动vector - size: " << v8.size() << ", capacity: " << v8.capacity() << std::endl;
        
        // 测试9：拷贝赋值
        std::cout << "\n9. 测试拷贝赋值：" << std::endl;
        vector<int> v9{11, 12, 13};
        vector<int> v10{14, 15, 16, 17, 18};
        std::cout << "   赋值前 v9 - size: " << v9.size() << ", capacity: " << v9.capacity() << std::endl;
        std::cout << "   赋值前 v10 - size: " << v10.size() << ", capacity: " << v10.capacity() << std::endl;
        
        v9 = v10;
        std::cout << "   赋值后 v9 - size: " << v9.size() << ", capacity: " << v9.capacity() << std::endl;
        std::cout << "   赋值后 v10 - size: " << v10.size() << ", capacity: " << v10.capacity() << std::endl;
        
        // 测试10：移动赋值
        std::cout << "\n10. 测试移动赋值：" << std::endl;
        vector<int> v11{19, 20, 21, 22};
        vector<int> v12{23, 24, 25, 26, 27, 28};
        std::cout << "   移动前 v11 - size: " << v11.size() << ", capacity: " << v11.capacity() << std::endl;
        std::cout << "   移动前 v12 - size: " << v12.size() << ", capacity: " << v12.capacity() << std::endl;
        
        v11 = std::move(v12);
        std::cout << "   移动后 v11 - size: " << v11.size() << ", capacity: " << v11.capacity() << std::endl;
        std::cout << "   移动后 v12 - size: " << v12.size() << ", capacity: " << v12.capacity() << std::endl;
        
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


