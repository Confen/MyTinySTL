#include "vector.h"
#include <cassert>
#include <iostream>

int main() {
  mystl::vector<int> v;
  assert(v.empty());

  // reserve/resize 组合
  v.reserve(8);
  assert(v.capacity() >= 8);
  v.resize(5, 3);
  assert(v.size() == 5 && v.back() == 3);
  v.resize(3);
  assert(v.size() == 3);
  v.resize(10, 7);
  assert(v.size() == 10 && v.back() == 7);

  // clear + shrink_to_fit 循环
  for (int r = 0; r < 2; ++r) {
    v.clear();
    auto cap_before = v.capacity();
    v.shrink_to_fit();
    assert(v.capacity() <= cap_before);
    v.resize(16, 1);
  }

  // push_back 压力路径
  v.clear();
  for (int i = 0; i < 20000; ++i) v.push_back(i);
  assert(v.size() == 20000);
  assert(v.back() == 19999);

  // 拷贝/移动赋值健壮性
  mystl::vector<int> a(3, 9);
  mystl::vector<int> b; b = a;           // 拷贝赋值
  mystl::vector<int> c; c = mystl::vector<int>{1,2,3,4}; // 移动赋值
  assert(b.size() == 3 && c.back() == 4);

  std::cout << "test_vector_edges OK\n";
  return 0;
}





