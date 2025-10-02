# MyTinySTL

一个基于 C++11 的简化版 STL 实现，用于学习和理解标准模板库的内部实现原理。

## 项目概述

MyTinySTL 是一个教学性质的 STL 实现，包含了标准库中的核心组件：

- **类型萃取系统** - `type_traits.h`
- **异常定义** - `exceptdef.h` 
- **工具函数** - `util.h`
- **函数对象** - `functional.h`
- **内存管理基础** - `construct.h`, `uninitialized.h`
- **空间配置器** - `allocator.h`, `alloc.h`
- **迭代器系统** - `iterator.h`
- **算法基础** - `algobase.h`
- **基本算法** - `algo.h`
- **内存管理** - `memory.h`

## 编译要求

- C++11 或更高版本
- GCC 4.8+ 或 MSVC 2013+ 或 Clang 3.3+

## 使用方法

```cpp
#include "MyTinySTL/type_traits.h"
#include "MyTinySTL/util.h"
#include "MyTinySTL/algorithm.h"

int main() {
    // 使用 MyTinySTL 的组件
    return 0;
}
```

## 测试

项目包含完整的测试套件，位于 `test/` 目录下。

## 学习目标

通过实现 MyTinySTL，可以深入理解：

1. C++ 模板元编程技术
2. STL 的设计思想和实现原理
3. 内存管理和异常安全
4. 迭代器模式和算法设计
5. 类型萃取和 SFINAE 技术

## 许可证

本项目仅用于学习目的，请勿用于商业用途。

## 贡献

欢迎提交 Issue 和 Pull Request 来改进这个项目。

