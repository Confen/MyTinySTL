#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <sstream>
#include <windows.h>

#include "../iterator.h"

using namespace mystl;

// 测试迭代器萃取
void test_iterator_traits() {
    std::cout << "=== 测试迭代器萃取 ===" << std::endl;
    
    // 测试指针的迭代器萃取
    typedef iterator_traits<int*> traits_int_ptr;
    std::cout << "int* 的迭代器类型: " << typeid(traits_int_ptr::iterator_category).name() << std::endl;
    std::cout << "int* 的值类型: " << typeid(traits_int_ptr::value_type).name() << std::endl;
    
    // 测试 const 指针的迭代器萃取
    typedef iterator_traits<const int*> traits_const_int_ptr;
    std::cout << "const int* 的迭代器类型: " << typeid(traits_const_int_ptr::iterator_category).name() << std::endl;
    std::cout << "const int* 的值类型: " << typeid(traits_const_int_ptr::value_type).name() << std::endl;
    
    // 测试标准容器的迭代器萃取
    typedef iterator_traits<std::vector<int>::iterator> traits_vec_iter;
    std::cout << "vector<int>::iterator 的迭代器类型: " << typeid(traits_vec_iter::iterator_category).name() << std::endl;
    
    typedef iterator_traits<std::list<int>::iterator> traits_list_iter;
    std::cout << "list<int>::iterator 的迭代器类型: " << typeid(traits_list_iter::iterator_category).name() << std::endl;
}

// 测试距离计算
void test_distance() {
    std::cout << "\n=== 测试距离计算 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};
    
    // 测试随机访问迭代器的距离
    auto dist_vec = mystl::distance(vec.begin(), vec.end());
    std::cout << "vector 距离: " << dist_vec << std::endl;
    
    // 测试双向迭代器的距离
    auto dist_list = mystl::distance(lst.begin(), lst.end());
    std::cout << "list 距离: " << dist_list << std::endl;
    
    // 测试数组的距离
    int arr[] = {1, 2, 3, 4, 5};
    auto dist_arr = mystl::distance(arr, arr + 5);
    std::cout << "数组距离: " << dist_arr << std::endl;
}

// 测试迭代器前进
void test_advance() {
    std::cout << "\n=== 测试迭代器前进 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 测试随机访问迭代器前进
    auto vec_it = vec.begin();
    mystl::advance(vec_it, 3);
    std::cout << "vector 前进3位后: " << *vec_it << std::endl;
    
    // 测试双向迭代器前进
    auto lst_it = lst.begin();
    mystl::advance(lst_it, 3);
    std::cout << "list 前进3位后: " << *lst_it << std::endl;
    
    // 测试双向迭代器后退
    mystl::advance(lst_it, -2);
    std::cout << "list 后退2位后: " << *lst_it << std::endl;
}

// 测试反向迭代器
void test_reverse_iterator() {
    std::cout << "\n=== 测试反向迭代器 ===" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 创建反向迭代器
    reverse_iterator<std::vector<int>::iterator> rbegin(vec.end());
    reverse_iterator<std::vector<int>::iterator> rend(vec.begin());
    
    std::cout << "反向遍历 vector: ";
    for (auto it = rbegin; it != rend; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 测试反向迭代器的操作
    auto rit = rbegin;
    std::cout << "反向迭代器解引用: " << *rit << std::endl;
    std::cout << "反向迭代器前进后: " << *(++rit) << std::endl;
    std::cout << "反向迭代器随机访问: " << rbegin[2] << std::endl;
    
    // 测试反向迭代器的比较
    std::cout << "rbegin == rbegin: " << (rbegin == rbegin) << std::endl;
    std::cout << "rbegin < rend: " << (rbegin < rend) << std::endl;
}

// 测试插入迭代器
void test_insert_iterators() {
    std::cout << "\n=== 测试插入迭代器 ===" << std::endl;
    
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = {4, 5, 6};
    std::list<int> lst = {7, 8, 9};
    
    // 测试 back_insert_iterator
    std::cout << "使用 back_inserter 插入: ";
    std::copy(vec2.begin(), vec2.end(), mystl::back_inserter(vec1));
    for (int val : vec1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 测试 insert_iterator
    std::vector<int> vec3 = {1, 2, 3, 7, 8, 9};
    std::cout << "使用 inserter 插入: ";
    std::copy(lst.begin(), lst.end(), mystl::inserter(vec3, vec3.begin() + 3));
    for (int val : vec3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// 测试流迭代器
void test_stream_iterators() {
    std::cout << "\n=== 测试流迭代器 ===" << std::endl;
    
    // 测试 istream_iterator
    std::istringstream iss("1 2 3 4 5");
    istream_iterator<int> input_it(iss);
    istream_iterator<int> input_end;
    
    std::cout << "从输入流读取: ";
    while (input_it != input_end) {
        std::cout << *input_it << " ";
        ++input_it;
    }
    std::cout << std::endl;
    
    // 测试 ostream_iterator
    std::ostringstream oss;
    ostream_iterator<int> output_it(oss, " ");
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "写入输出流: ";
    for (int val : vec) {
        *output_it = val;
    }
    std::cout << oss.str() << std::endl;
}

// 测试自定义迭代器
void test_custom_iterator() {
    std::cout << "\n=== 测试自定义迭代器 ===" << std::endl;
    
    // 简单的自定义迭代器
    class SimpleIterator : public iterator<random_access_iterator_tag, int> {
    private:
        int* ptr;
        
    public:
        SimpleIterator(int* p) : ptr(p) {}
        
        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }
        
        SimpleIterator& operator++() { ++ptr; return *this; }
        SimpleIterator operator++(int) { SimpleIterator tmp = *this; ++ptr; return tmp; }
        
        SimpleIterator& operator--() { --ptr; return *this; }
        SimpleIterator operator--(int) { SimpleIterator tmp = *this; --ptr; return tmp; }
        
        SimpleIterator& operator+=(difference_type n) { ptr += n; return *this; }
        SimpleIterator& operator-=(difference_type n) { ptr -= n; return *this; }
        
        SimpleIterator operator+(difference_type n) const { return SimpleIterator(ptr + n); }
        SimpleIterator operator-(difference_type n) const { return SimpleIterator(ptr - n); }
        
        difference_type operator-(const SimpleIterator& other) const { return ptr - other.ptr; }
        
        reference operator[](difference_type n) const { return ptr[n]; }
        
        bool operator==(const SimpleIterator& other) const { return ptr == other.ptr; }
        bool operator!=(const SimpleIterator& other) const { return ptr != other.ptr; }
        bool operator<(const SimpleIterator& other) const { return ptr < other.ptr; }
        bool operator<=(const SimpleIterator& other) const { return ptr <= other.ptr; }
        bool operator>(const SimpleIterator& other) const { return ptr > other.ptr; }
        bool operator>=(const SimpleIterator& other) const { return ptr >= other.ptr; }
    };
    
    int arr[] = {1, 2, 3, 4, 5};
    SimpleIterator begin(arr);
    SimpleIterator end(arr + 5);
    
    std::cout << "自定义迭代器遍历: ";
    for (auto it = begin; it != end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 测试自定义迭代器的萃取
    typedef iterator_traits<SimpleIterator> traits;
    std::cout << "自定义迭代器类型: " << typeid(traits::iterator_category).name() << std::endl;
    
    // 测试距离和前进
    auto dist = mystl::distance(begin, end);
    std::cout << "自定义迭代器距离: " << dist << std::endl;
    
    auto it = begin;
    mystl::advance(it, 2);
    std::cout << "自定义迭代器前进2位后: " << *it << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    test_iterator_traits();
    test_distance();
    test_advance();
    test_reverse_iterator();
    test_insert_iterators();
    test_stream_iterators();
    test_custom_iterator();
    
    std::cout << "\n=== 迭代器系统测试完成 ===" << std::endl;
    
    return 0;
}
