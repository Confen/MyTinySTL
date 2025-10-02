@echo off
echo 编译和运行内存泄露检测测试
echo =============================

echo.
echo 1. 编译基础内存泄露检测演示...
g++ -std=c++11 -I.. -DMYTINYSTL_DEBUG -o memory_leak_detection_demo.exe memory_leak_detection_demo.cpp
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！
    echo 运行基础演示：
    memory_leak_detection_demo.exe
) else (
    echo 编译失败！
)

echo.
echo 2. 编译详细内存泄露分析演示...
g++ -std=c++11 -I.. -DMYTINYSTL_DEBUG -o memory_leak_analysis_demo.exe memory_leak_analysis_demo.cpp
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！
    echo 运行详细分析：
    memory_leak_analysis_demo.exe
) else (
    echo 编译失败！
)

echo.
echo 所有测试完成！
pause
