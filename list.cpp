#include "list.h"
#include<iostream>

int main() {
  mystl::list<int> a;
  mystl::list<int> b;
  a.push_back(1);
  a.push_back(2);
  b.push_back(9);
  b.push_back(10);
  b.pop_back();
  b.clear();
    const mystl::list<int>& ca = a;
    int sum2 = 0;
    for (auto it = ca.cbegin(); it != ca.cend(); ++it) sum2 += *it;
    std::cout << sum2 << "\n"; // 与之前 sum 一致
  std::cout << a.size() << " " << b.size() << "\n"; // 2 1
  a.swap(b);
  std::cout << a.size() << " " << b.size() << "\n"; // 1 2
}