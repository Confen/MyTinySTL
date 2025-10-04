#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 空的 vector 是否有效？ ===" << std::endl;
    
    // 创建空的 vector
    std::vector<int> empty_vec;
    
    std::cout << "\n1. 空 vector 的基本信息：" << std::endl;
    std::cout << "size(): " << empty_vec.size() << std::endl;
    std::cout << "capacity(): " << empty_vec.capacity() << std::endl;
    std::cout << "empty(): " << (empty_vec.empty() ? "true" : "false") << std::endl;
    std::cout << "max_size(): " << empty_vec.max_size() << std::endl;
    
    std::cout << "\n2. 空 vector 可以执行的有效操作：" << std::endl;
    
    // 可以安全地调用这些方法
    std::cout << "✅ 可以调用 size()：" << empty_vec.size() << std::endl;
    std::cout << "✅ 可以调用 empty()：" << empty_vec.empty() << std::endl;
    std::cout << "✅ 可以调用 capacity()：" << empty_vec.capacity() << std::endl;
    
    // 可以安全地使用迭代器
    std::cout << "✅ 可以使用迭代器：" << std::endl;
    std::cout << "   begin() == end(): " << (empty_vec.begin() == empty_vec.end() ? "true" : "false") << std::endl;
    
    // 可以安全地使用范围 for 循环
    std::cout << "✅ 可以使用范围 for 循环：" << std::endl;
    int count = 0;
    for (const auto& item : empty_vec) {
        count++;
        std::cout << item << " ";  // 这行不会执行，因为没有元素
    }
    std::cout << "   循环执行了 " << count << " 次" << std::endl;
    
    // 可以安全地添加元素
    std::cout << "✅ 可以添加元素：" << std::endl;
    empty_vec.push_back(42);
    std::cout << "   添加后 size(): " << empty_vec.size() << std::endl;
    std::cout << "   添加后 empty(): " << (empty_vec.empty() ? "true" : "false") << std::endl;
    
    // 可以安全地清空
    empty_vec.clear();
    std::cout << "✅ 可以清空：" << std::endl;
    std::cout << "   清空后 size(): " << empty_vec.size() << std::endl;
    
    std::cout << "\n3. 空 vector 不能执行的操作（会导致未定义行为）：" << std::endl;
    
    std::vector<int> another_empty_vec;
    
    std::cout << "❌ 不能访问 front()：" << std::endl;
    std::cout << "   （如果调用会导致未定义行为）" << std::endl;
    // another_empty_vec.front();  // 危险！不要这样做
    
    std::cout << "❌ 不能访问 back()：" << std::endl;
    std::cout << "   （如果调用会导致未定义行为）" << std::endl;
    // another_empty_vec.back();   // 危险！不要这样做
    
    std::cout << "❌ 不能使用 operator[]：" << std::endl;
    std::cout << "   （如果调用会导致未定义行为）" << std::endl;
    // another_empty_vec[0];       // 危险！不要这样做
    
    std::cout << "❌ 不能使用 at()：" << std::endl;
    std::cout << "   （如果调用会抛出 std::out_of_range 异常）" << std::endl;
    try {
        another_empty_vec.at(0);
    } catch (const std::out_of_range& e) {
        std::cout << "   捕获到异常：" << e.what() << std::endl;
    }
    
    std::cout << "\n4. 空 vector 与其他容器的比较：" << std::endl;
    
    std::vector<int> vec1;
    std::vector<int> vec2;
    std::vector<int> vec3{1, 2, 3};
    
    std::cout << "两个空 vector 相等：" << (vec1 == vec2 ? "true" : "false") << std::endl;
    std::cout << "空 vector 与非空 vector 不相等：" << (vec1 == vec3 ? "true" : "false") << std::endl;
    
    std::cout << "\n5. 空 vector 的内存状态：" << std::endl;
    
    std::vector<int> mem_test_vec;
    std::cout << "默认构造的空 vector：" << std::endl;
    std::cout << "   size(): " << mem_test_vec.size() << std::endl;
    std::cout << "   capacity(): " << mem_test_vec.capacity() << std::endl;
    std::cout << "   data(): " << mem_test_vec.data() << std::endl;
    
    // 预分配内存但仍为空
    mem_test_vec.reserve(10);
    std::cout << "reserve(10) 后的空 vector：" << std::endl;
    std::cout << "   size(): " << mem_test_vec.size() << std::endl;
    std::cout << "   capacity(): " << mem_test_vec.capacity() << std::endl;
    std::cout << "   data(): " << mem_test_vec.data() << std::endl;
    std::cout << "   仍然是空的，但有预分配的内存" << std::endl;
    
    std::cout << "\n=== 结论 ===" << std::endl;
    std::cout << "✅ 空的 vector 是完全有效的 C++ 对象" << std::endl;
    std::cout << "✅ 它有明确定义的状态和行为" << std::endl;
    std::cout << "✅ 可以安全地调用大多数成员函数" << std::endl;
    std::cout << "✅ 支持所有标准容器操作（除了访问元素）" << std::endl;
    std::cout << "✅ 在算法和迭代器中表现正常" << std::endl;
    std::cout << "✅ 是 RAII 的完美体现：构造即可用，析构自动清理" << std::endl;
    
    std::cout << "\n在我们的异常安全测试中：" << std::endl;
    std::cout << "- broken_obj 的 data 是空 vector，这是完全有效的状态" << std::endl;
    std::cout << "- '未初始化' 是业务逻辑层面的概念，不是 C++ 对象层面的" << std::endl;
    std::cout << "- 空 vector 可以安全地进行状态比较和操作" << std::endl;
    
    return 0;
}








