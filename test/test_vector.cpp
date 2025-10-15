#include "util.h"
#include "vector.h"
#include <iostream>
int main() {
  mystl::vector<int> v;
  for (int i=0;i<10;++i) v.push_back(i);
  v.push_back(42);
  v.pop_back();
  v.resize(20, 7);
  std::cout << v.size() << " " << v.capacity() << " " << v.back() << "\n";
  // 拷贝/移动构造与赋值
    mystl::vector<int> a(3, 9);         // [9,9,9]
    mystl::vector<int> b = a;           // 拷贝构造
    mystl::vector<int> c(std::move(a)); // 移动构造，a 置空
    b = c;                              // 拷贝赋值
    c = mystl::vector<int>{1,2,3,4};    // 移动赋值
    std::cout << b.size() << " " << c.back() << "\n";

    // 触发 reallocate_and_insert 的路径（push_back 足够）
    for (int i=0;i<1000;++i) v.push_back(i);
    std::cout << v.size() << " " << v.capacity() << "\n";
}