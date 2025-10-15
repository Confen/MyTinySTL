#include "list.h"
#include <cassert>
#include <iostream>

int main() {
  mystl::list<int> L;
  assert(L.empty());
  L.assign(3, 1);                 // [1,1,1]
  assert(L.size() == 3);
  L.resize(5);                     // [1,1,1,0,0]
  assert(L.size() == 5);
  assert(L.back() == 0);
  L.resize(2);                     // [1,1]
  assert(L.size() == 2);

  // 头尾插入
  L.push_front(9);                 // [9,1,1]
  L.push_back(8);                  // [9,1,1,8]
  assert(L.front() == 9 && L.back() == 8);

  // 自身 splice：pos == next(it) 无变化
  {
    mystl::list<int> s{1,2,3};
    const auto before = s;         // 拷贝用于对比
    s.splice(++s.begin(), s, s.begin());
    assert(s.size() == before.size());
  }

  // unique / remove_if
  {
    int a[] = {1,1,2,2,2,3,3,4};
    mystl::list<int> u; u.assign(a, a+8);
    size_t removed_adj = u.unique();
    assert(removed_adj == 4);      // [1,2,3,4]
    assert(u.size() == 4);
    u.remove_if([](int x){ return (x & 1) == 1; }); // 移除奇数 => [2,4]
    assert(u.size() == 2 && u.front() == 2 && u.back() == 4);
  }

  // 区间 erase
  {
    int a[] = {1,2,3,4,5,6};
    mystl::list<int> e; e.assign(a, a+6); // [1,2,3,4,5,6]
    auto first = ++(++e.begin());         // 指向 3
    auto last  = ++(++(++e.begin()));     // 指向 4 的后一个
    e.erase(first, last);                 // 只删 3 => [1,2,4,5,6]
    assert(e.size() == 5);
    assert(*(++(++e.begin())) == 4);
  }

  // 清理与复用
  L.clear();
  assert(L.empty());
  L.push_back(42);
  assert(!L.empty() && L.front() == 42 && L.back() == 42);

  std::cout << "test_list_edges OK\n";
  return 0;
}





