#!/bin/bash

echo "MyTinySTL 集合算法优化性能测试脚本"
echo "====================================="

echo ""
echo "正在编译测试文件..."

# 编译无分支优化测试
echo "编译无分支优化测试..."
g++ -std=c++11 -O3 -march=native -funroll-loops -I.. test_set_algo_branchless_performance.cpp -o test_branchless_performance
if [ $? -ne 0 ]; then
    echo "编译无分支优化测试失败！"
    exit 1
fi

# 编译高级优化测试
echo "编译高级优化测试..."
g++ -std=c++11 -O3 -march=native -funroll-loops -I.. test_set_algo_advanced_performance.cpp -o test_advanced_performance
if [ $? -ne 0 ]; then
    echo "编译高级优化测试失败！"
    exit 1
fi

# 编译综合对比测试
echo "编译综合对比测试..."
g++ -std=c++11 -O3 -march=native -funroll-loops -I.. test_set_algo_comprehensive_comparison.cpp -o test_comprehensive_comparison
if [ $? -ne 0 ]; then
    echo "编译综合对比测试失败！"
    exit 1
fi

echo ""
echo "编译完成！开始运行测试..."
echo ""

# 运行无分支优化测试
echo "========================================"
echo "运行无分支优化性能测试"
echo "========================================"
./test_branchless_performance
echo ""

# 运行高级优化测试
echo "========================================"
echo "运行高级优化性能测试"
echo "========================================"
./test_advanced_performance
echo ""

# 运行综合对比测试
echo "========================================"
echo "运行综合对比性能测试"
echo "========================================"
./test_comprehensive_comparison
echo ""

echo "========================================"
echo "所有测试完成！"
echo "========================================"
echo ""
echo "测试结果说明："
echo "1. 无分支优化测试：测试位运算和条件移动指令的优化效果"
echo "2. 高级优化测试：测试查找表、预取、SIMD等高级优化技术"
echo "3. 综合对比测试：对比三种方案的整体性能表现"
echo ""
echo "性能提升预期："
echo "- 无分支优化：15-30% 性能提升"
echo "- 高级优化：20-50% 性能提升"
echo "- 特殊优化：25-60% 性能提升"
echo ""
echo "结果文件："
echo "- performance_results.csv：详细的性能测试数据"
echo ""

# 清理编译文件
echo "清理编译文件..."
rm -f test_branchless_performance test_advanced_performance test_comprehensive_comparison
echo "清理完成！"
