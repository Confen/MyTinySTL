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
    int h = 10;
  a.insert(a.cbegin(),h);
  a.emplace(a.cbegin(),h);
  std::cout << a.size() << " " << b.size() << "\n"; // 2 1
  a.swap(b);
  std::cout << a.size() << " " << b.size() << "\n"; // 1 2
  std::cout<<std::endl;
  mystl::list<int> c;
  int arr[] = {1,2,3,4,5,6,7};
  c.insert(c.cbegin(),arr,arr+7);
  for(auto it = c.cbegin(); it != c.cend(); ++it) std::cout << *it << " ";
  std::cout << std::endl;
  int arr2[] = {6,7,8,9,10,11};
  c.assign(arr2,arr2+6);
  for(auto it = c.cbegin(); it != c.cend(); ++it) std::cout << *it << " ";
  std::cout << std::endl;
  mystl::list<int> r;
  r.assign(2, 1);        // [1,1]
  r.resize(5);           // [1,1,0,0,0]
  r.resize(3);           // [1,1,0]
  r.resize(6, 9);        // [1,1,0,9,9,9]
  std::cout << r.size() << "\n";
  std::cout<<r.front()<<" "<<r.back()<<"\n";
  mystl::list<int> z;
  int cz[] = {1,2,2,3,2,4};
  z.assign(cz, cz+6);
  std::cout << z.remove(2) << "\n";           // 3
  std::cout << z.size() << "\n";               // 3
  z.remove_if([](int x){ return x%2==1; });    // 移除奇数
  std::cout << z.size() << "\n";   
  mystl::list<int> u;
int uu[] = {1,1,2,2,2,3,3,4};
u.assign(uu, uu+8);
std::cout << u.unique() << "\n";         
std::cout << u.size() << "\n";          
u.reverse();                             
std::cout << u.size() << "\n";     
mystl::list<int> a1, b1;
int aa[] = {1,2,3}, bb[] = {7,8,9,10};
a1.assign(aa, aa+3);    // [1,2,3]
b1.assign(bb, bb+4);    // [7,8,9,10]

// 单元素 splice
b1.splice(b1.cbegin(), a1, ++a1.cbegin()); // 把 2 移到 b 头：b=[2,7,8,9,10], a=[1,3]
std::cout << a1.size() << " " << b1.size() << "\n"; // 2 5

// 区间 splice：把 b 的 [next(next(b.begin())), b.end()) → 即 [8,9,10] 移到 a 的尾
auto b_first = ++(++b1.begin()); // 指向 8
a1.splice(a1.cend(), b1, b_first, b1.cend()); // a=[1,3,8,9,10], b=[2,7]
std::cout << a1.size() << " " << b1.size() << "\n"; // 5 2    

mystl::list<int> c1; int src[] = {1,2,3}; c1.assign(src, src+3);
mystl::list<int> cc1(c1);                  // 拷贝构造
mystl::list<int> mm1(std::move(cc1));      // 移动构造（cc 置空）
mystl::list<int> aa1; aa1 = mm1;            // 拷贝赋值
mystl::list<int> bb1; bb1 = std::move(aa1); // 移动赋值（aa 置空）
std::cout << mm1.size() << " " << bb1.size() << "\n"; // 都应为 3
mystl::list<int> d1{1,2,3,4,5};
std::cout<<d1.size()<<"\n";

//比较操作符与swap
mystl::list<int> x{1,2,3}, y{1,2,3}, p{1,2,4};
std::cout << (x == y) << " " << (x != p) << "\n"; // 1 1
std::cout << (x < p) << " " << (p > x) << "\n";   // 1 1
swap(x, p);
std::cout << x.size() << " " << p.size() << "\n"; // 3 3

mystl::list<int> e;
std::cout << e.size() << " " << e.empty() << "\n"; // 0 1
e.push_back(42);
std::cout << e.front() << " " << e.back() << " " << e.size() << "\n"; // 42 42 1
e.clear();
std::cout << e.size() << " " << e.empty() << "\n"; // 0 1

mystl::list<int> er{1};
er.erase(er.cbegin());
std::cout << er.size() << " " << er.empty() << "\n"; // 0 1

mystl::list<int> s1{1,2,3}, s2{4,5};
s1.splice(s1.cbegin(), s2);                  // s1=[4,5,1,2,3], s2=[]
std::cout << s1.size() << " " << s2.size() << "\n"; // 5 0
s1.splice(++s1.cbegin(), s1, s1.begin());    // 同容器、pos==next(it)，无变化
std::cout << s1.size() << "\n";                      // 5

mystl::list<int> u1{1,1,2,2,2,3,3,4};
std::cout << u1.unique() << " " << u1.size() << "\n"; // 4 4
u1.reverse();
std::cout << u1.size() << "\n";                       // 4

}