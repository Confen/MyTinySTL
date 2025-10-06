#include <iostream>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector clear() 测试 ===" << std::endl;
    
    try {
        // 测试1：清空非空vector
        std::cout << "\n1. 测试清空非空vector：" << std::endl;
        vector<int> v1{1, 2, 3, 4, 5};
        std::cout << "   清空前 - size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
        
        v1.clear();
        std::cout << "   清空后 - size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
        std::cout << "   empty(): " << (v1.empty() ? "true" : "false") << std::endl;
        
        // 测试2：清空空vector
        std::cout << "\n2. 测试清空空vector：" << std::endl;
        vector<int> v2;
        std::cout << "   清空前 - size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
        
        v2.clear();
        std::cout << "   清空后 - size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
        std::cout << "   empty(): " << (v2.empty() ? "true" : "false") << std::endl;
        
        // 测试3：清空后重新添加元素
        std::cout << "\n3. 测试清空后重新添加元素：" << std::endl;
        vector<int> v3{10, 20, 30, 40, 50};
        std::cout << "   原始 - size: " << v3.size() << ", capacity: " << v3.capacity() << std::endl;
        
        v3.clear();
        std::cout << "   清空后 - size: " << v3.size() << ", capacity: " << v3.capacity() << std::endl;
        
        v3.push_back(100);
        v3.push_back(200);
        std::cout << "   重新添加后 - size: " << v3.size() << ", capacity: " << v3.capacity() << std::endl;
        std::cout << "   元素: ";
        for (size_t i = 0; i < v3.size(); ++i) {
            std::cout << v3[i] << " ";
        }
        std::cout << std::endl;
        
        // 测试4：测试resize方法
        std::cout << "\n4. 测试resize方法：" << std::endl;
        vector<int> v4{1, 2, 3};
        std::cout << "   原始 - size: " << v4.size() << ", capacity: " << v4.capacity() << std::endl;
        
        // 测试缩小
        v4.resize(2, 99);
        std::cout << "   resize(2, 99)后 - size: " << v4.size() << ", capacity: " << v4.capacity() << std::endl;
        std::cout << "   元素: ";
        for (size_t i = 0; i < v4.size(); ++i) {
            std::cout << v4[i] << " ";
        }
        std::cout << std::endl;
        
        // 测试扩大
        v4.resize(5, 88);
        std::cout << "   resize(5, 88)后 - size: " << v4.size() << ", capacity: " << v4.capacity() << std::endl;
        std::cout << "   元素: ";
        for (size_t i = 0; i < v4.size(); ++i) {
            std::cout << v4[i] << " ";
        }
        std::cout << std::endl;
        
        // 测试5：测试shrink_to_fit
        std::cout << "\n5. 测试shrink_to_fit：" << std::endl;
        vector<int> v5;
        for (int i = 0; i < 100; ++i) {
            v5.push_back(i);
        }
        std::cout << "   扩容后 - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
        
        v5.shrink_to_fit();
        std::cout << "   shrink_to_fit后 - size: " << v5.size() << ", capacity: " << v5.capacity() << std::endl;
        
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
