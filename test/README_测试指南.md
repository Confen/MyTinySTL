# MyTinySTL 测试指南

## 如何测试 MyTinySTL 项目

### 1. 环境要求

- **编译器**: GCC 4.8+ 或 MSVC 2015+ (支持 C++11)
- **操作系统**: Windows/Linux/macOS
- **构建工具**: Make 或 CMake

### 2. 编译命令

#### Windows (MinGW)
```bash
# 基础测试
g++ -std=c++11 -static-libgcc -static-libstdc++ test/test_type_traits_simple.cpp -o test_type_traits_simple.exe

# 完整测试
g++ -std=c++11 -static-libgcc -static-libstdc++ test/test_type_traits.cpp -o test_type_traits.exe
```

#### Linux/macOS
```bash
# 基础测试
g++ -std=c++11 test/test_type_traits_simple.cpp -o test_type_traits_simple

# 完整测试
g++ -std=c++11 test/test_type_traits.cpp -o test_type_traits
```

### 3. 运行测试

#### Windows
```cmd
.\test_type_traits_simple.exe
.\test_type_traits.exe
```

#### Linux/macOS
```bash
./test_type_traits_simple
./test_type_traits
```

### 4. 测试结果解读

#### 成功标志
- 程序正常编译，无错误和警告
- 程序正常运行，输出测试结果
- 所有 `static_assert` 通过
- 输出显示 "=== 所有测试完成 ==="

#### 测试内容
1. **基础类型定义测试**
   - `m_integral_constant` 值测试
   - `m_bool_constant` 布尔值测试
   - `m_true_type` 和 `m_false_type` 类型测试
   - `is_pair` 类型判断测试

2. **标准库类型萃取测试**
   - 基本类型特征 (void, integral, floating_point 等)
   - 引用和指针特征 (reference, pointer, lvalue_reference 等)
   - 数组和函数特征 (array, function, class 等)
   - 类型关系特征 (same, convertible 等)
   - 构造和赋值特征 (constructible, assignable 等)
   - 异常安全特征 (nothrow 系列)

3. **类型修饰符操作测试**
   - 移除修饰符 (remove_const, remove_volatile 等)
   - 添加修饰符 (add_const, add_volatile 等)
   - 引用和指针操作 (remove_reference, add_pointer 等)

4. **类型转换测试**
   - 有符号/无符号转换 (make_signed, make_unsigned)
   - 数组维度操作 (remove_extent, remove_all_extents)
   - 类型衰减 (decay)

5. **条件类型测试**
   - 条件选择 (conditional)
   - 公共类型 (common_type)
   - 函数调用结果类型 (result_of)

6. **自定义类型萃取测试**
   - 函数指针特征 (is_function_pointer)
   - 成员函数指针特征 (is_member_function_pointer)
   - 成员对象指针特征 (is_member_object_pointer)
   - 可调用对象特征 (is_callable)
   - 函数对象特征 (is_function_object)
   - 智能指针特征 (is_smart_pointer)
   - 容器特征 (is_container)
   - 迭代器特征 (is_iterator)

### 5. 常见问题解决

#### 问题1: 编译错误 "无法找到入口点"
**原因**: 缺少 C++ 标准库链接
**解决**: 添加 `-static-libgcc -static-libstdc++` 参数

#### 问题2: 模板特化错误
**原因**: C++11 对模板特化语法要求严格
**解决**: 检查模板特化语法，确保符合 C++11 标准

#### 问题3: 头文件包含错误
**原因**: 缺少必要的头文件
**解决**: 确保包含所有必要的标准库头文件

### 6. 性能测试

#### 编译时间测试
```bash
time g++ -std=c++11 test/test_type_traits_simple.cpp -o test_type_traits_simple
```

#### 运行时间测试
```bash
time ./test_type_traits_simple
```

### 7. 调试技巧

#### 启用详细输出
```bash
g++ -std=c++11 -v test/test_type_traits_simple.cpp -o test_type_traits_simple
```

#### 启用调试信息
```bash
g++ -std=c++11 -g -O0 test/test_type_traits_simple.cpp -o test_type_traits_simple
```

#### 检查模板实例化
```bash
g++ -std=c++11 -ftemplate-backtrace-limit=0 test/test_type_traits_simple.cpp -o test_type_traits_simple
```

### 8. 扩展测试

#### 添加新的测试用例
1. 在测试文件中添加新的测试函数
2. 使用 `static_assert` 进行编译期测试
3. 使用 `std::cout` 进行运行期测试
4. 确保测试覆盖所有边界情况

#### 性能基准测试
1. 与标准库进行性能对比
2. 测试大量类型萃取的编译时间
3. 测试运行期性能开销

### 9. 持续集成

#### 自动化测试脚本
```bash
#!/bin/bash
# 编译测试
g++ -std=c++11 -static-libgcc -static-libstdc++ test/test_type_traits_simple.cpp -o test_type_traits_simple.exe
if [ $? -eq 0 ]; then
    echo "编译成功"
    # 运行测试
    ./test_type_traits_simple.exe
    if [ $? -eq 0 ]; then
        echo "测试通过"
    else
        echo "测试失败"
        exit 1
    fi
else
    echo "编译失败"
    exit 1
fi
```

### 10. 贡献指南

#### 提交测试
1. 确保所有测试通过
2. 添加适当的注释
3. 遵循代码规范
4. 更新测试文档

#### 报告问题
1. 提供完整的错误信息
2. 说明复现步骤
3. 提供环境信息
4. 附上相关代码片段














