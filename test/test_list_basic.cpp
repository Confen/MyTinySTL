#include <iostream>
#include <windows.h>
#include "../list.h"

using namespace mystl;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL List 基础测试 ===" << std::endl;
    
    try {
        // 测试1：默认构造
        std::cout << "\n1. 测试默认构造：" << std::endl;
        list<int> l1;
        std::cout << "   空list - size: " << l1.size() << ", empty: " << (l1.empty() ? "true" : "false") << std::endl;
        
        // 测试2：指定大小构造
        std::cout << "\n2. 测试指定大小构造：" << std::endl;
        list<int> l2(5);
        std::cout << "   list<int> l2(5) - size: " << l2.size() << std::endl;
        
        // 测试3：指定大小和值构造
        std::cout << "\n3. 测试指定大小和值构造：" << std::endl;
        list<int> l3(3, 42);
        std::cout << "   list<int> l3(3, 42) - size: " << l3.size() << std::endl;
        
        // 测试4：初始化列表构造
        std::cout << "\n4. 测试初始化列表构造：" << std::endl;
        list<int> l4{1, 2, 3, 4, 5};
        std::cout << "   list<int> l4{1,2,3,4,5} - size: " << l4.size() << std::endl;
        
        // 测试5：push_back操作
        std::cout << "\n5. 测试push_back操作：" << std::endl;
        list<int> l5;
        for (int i = 1; i <= 5; ++i) {
            l5.push_back(i);
            std::cout << "   添加元素 " << i << " - size: " << l5.size() << std::endl;
        }
        
        // 测试6：push_front操作
        std::cout << "\n6. 测试push_front操作：" << std::endl;
        list<int> l6;
        for (int i = 1; i <= 3; ++i) {
            l6.push_front(i);
            std::cout << "   在前面添加元素 " << i << " - size: " << l6.size() << std::endl;
        }
        
        // 测试7：front和back访问
        std::cout << "\n7. 测试front和back访问：" << std::endl;
        list<int> l7{10, 20, 30, 40, 50};
        std::cout << "   front(): " << l7.front() << ", back(): " << l7.back() << std::endl;
        
        // 测试8：pop_back操作
        std::cout << "\n8. 测试pop_back操作：" << std::endl;
        list<int> l8{1, 2, 3, 4, 5};
        std::cout << "   删除前 - size: " << l8.size() << ", back(): " << l8.back() << std::endl;
        l8.pop_back();
        std::cout << "   删除后 - size: " << l8.size() << ", back(): " << l8.back() << std::endl;
        
        // 测试9：pop_front操作
        std::cout << "\n9. 测试pop_front操作：" << std::endl;
        list<int> l9{1, 2, 3, 4, 5};
        std::cout << "   删除前 - size: " << l9.size() << ", front(): " << l9.front() << std::endl;
        l9.pop_front();
        std::cout << "   删除后 - size: " << l9.size() << ", front(): " << l9.front() << std::endl;
        
        // 测试10：resize操作
        std::cout << "\n10. 测试resize操作：" << std::endl;
        list<int> l10{1, 2, 3};
        std::cout << "   原始 - size: " << l10.size() << std::endl;
        
        l10.resize(5, 99);
        std::cout << "   resize(5, 99)后 - size: " << l10.size() << std::endl;
        
        l10.resize(2);
        std::cout << "   resize(2)后 - size: " << l10.size() << std::endl;
        
        // 测试11：clear操作
        std::cout << "\n11. 测试clear操作：" << std::endl;
        list<int> l11{1, 2, 3, 4, 5};
        std::cout << "   清空前 - size: " << l11.size() << std::endl;
        l11.clear();
        std::cout << "   清空后 - size: " << l11.size() << ", empty: " << (l11.empty() ? "true" : "false") << std::endl;
        
        // 测试12：拷贝构造
        std::cout << "\n12. 测试拷贝构造：" << std::endl;
        list<int> l12{1, 2, 3, 4, 5};
        list<int> l12_copy(l12);
        std::cout << "   原list - size: " << l12.size() << std::endl;
        std::cout << "   拷贝list - size: " << l12_copy.size() << std::endl;
        
        // 测试13：移动构造
        std::cout << "\n13. 测试移动构造：" << std::endl;
        list<int> l13{1, 2, 3, 4, 5};
        list<int> l13_move(std::move(l13));
        std::cout << "   原list - size: " << l13.size() << std::endl;
        std::cout << "   移动list - size: " << l13_move.size() << std::endl;
        
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



