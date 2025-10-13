最小用例与依赖说明（README 片段）
头文件包含顺序（建议）
先基础设施再容器，避免环依赖：
1) iterator.h
2) type_traits.h
3) util.h
4) construct.h
5) uninitialized.h
6) allocator.h
7) 容器头：list.h / vector.h
如果你在单一 TU 中只用容器，直接包含容器头即可（容器头已包含其依赖），但在库内开发时建议按上面顺序组织。
工具与约定
移动/转发：mystl::move / mystl::forward
分配/构造：mystl::allocator / mystl::construct / mystl::destroy
非抛异常约定与基础：见 exceptdef.h
字符串占位：mystl_string.h 将 mystl::string 绑定为 std::string
list 快速示例
#include "list.h"
#include <iostream>
int main() {
  mystl::list<int> a;
  a.push_back(1);
  a.push_back(2);
  a.push_front(0);     // [0,1,2]
  a.emplace(a.cbegin(), -1);  // [-1,0,1,2]
  a.unique();          // 相邻去重
  a.reverse();         // 反转

  mystl::list<int> b;
  int arr[] = {5,6,7};
  b.assign(arr, arr+3);       // [5,6,7]
  a.splice(a.cend(), b);      // a:[2,-1,0,1,5,6,7]（顺序取决于前一步操作），b:[]

  std::cout << a.size() << " " << b.size() << "\n";
}
vector 快速示例
#include "vector.h"
#include <iostream>
int main() {
  mystl::vector<int> v;
  for (int i=0;i<10;++i) v.push_back(i);
  v.resize(20, 7);
  std::cout << v.size() << " " << v.capacity() << " " << v.back() << "\n";

  mystl::vector<int> a(3, 9), b = a;     // 拷贝构造
  mystl::vector<int> c(std::move(a));    // 移动构造
  b = c;                                 // 拷贝赋值
  c = mystl::vector<int>{1,2,3,4};       // 移动赋值
  std::cout << b.size() << " " << c.back() << "\n";
}
编译
注意
若单独包含 util.h，需确保：iterator.h → type_traits.h → util.h 的依赖顺序已在各头文件内部处理好（当前已修复）。
若启用异常模块消息字符串，需包含 mystl_string.h。