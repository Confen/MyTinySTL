#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

// 测试用的类
class TestClass {
public:
    TestClass() = default;
    TestClass(const TestClass&) = default;
    TestClass(TestClass&&) = default;
    TestClass& operator=(const TestClass&) = default;
    TestClass& operator=(TestClass&&) = default;
    ~TestClass() = default;
};

class TestClassNoThrow {
public:
    TestClassNoThrow() noexcept = default;
    TestClassNoThrow(const TestClassNoThrow&) noexcept = default;
    TestClassNoThrow(TestClassNoThrow&&) noexcept = default;
    TestClassNoThrow& operator=(const TestClassNoThrow&) noexcept = default;
    TestClassNoThrow& operator=(TestClassNoThrow&&) noexcept = default;
    ~TestClassNoThrow() noexcept = default;
};

class TestClassThrowing {
public:
    TestClassThrowing() { throw std::runtime_error("Constructor throws"); }
    TestClassThrowing(const TestClassThrowing&) { throw std::runtime_error("Copy constructor throws"); }
    TestClassThrowing(TestClassThrowing&&) { throw std::runtime_error("Move constructor throws"); }
    TestClassThrowing& operator=(const TestClassThrowing&) { throw std::runtime_error("Copy assignment throws"); return *this; }
    TestClassThrowing& operator=(TestClassThrowing&&) { throw std::runtime_error("Move assignment throws"); return *this; }
    ~TestClassThrowing() { throw std::runtime_error("Destructor throws"); }
};

void testExceptionClasses() {
    std::cout << "\n=== 测试异常类 ===" << std::endl;
    
    // 测试 mystl_exception
    try {
        throw mystl::mystl_exception("测试基础异常");
    } catch (const mystl::mystl_exception& e) {
        std::cout << "  捕获 mystl_exception: " << e.what() << std::endl;
    }
    
    // 测试 mystl_logic_error
    try {
        throw mystl::mystl_logic_error("测试逻辑错误异常");
    } catch (const mystl::mystl_logic_error& e) {
        std::cout << "  捕获 mystl_logic_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_runtime_error
    try {
        throw mystl::mystl_runtime_error("测试运行时错误异常");
    } catch (const mystl::mystl_runtime_error& e) {
        std::cout << "  捕获 mystl_runtime_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_bad_alloc
    try {
        throw mystl::mystl_bad_alloc("测试内存分配失败异常");
    } catch (const mystl::mystl_bad_alloc& e) {
        std::cout << "  捕获 mystl_bad_alloc: " << e.what() << std::endl;
    }
    
    // 测试 mystl_bad_cast
    try {
        throw mystl::mystl_bad_cast("测试类型转换失败异常");
    } catch (const mystl::mystl_bad_cast& e) {
        std::cout << "  捕获 mystl_bad_cast: " << e.what() << std::endl;
    }
    
    // 测试 mystl_bad_typeid
    try {
        throw mystl::mystl_bad_typeid("测试类型信息获取失败异常");
    } catch (const mystl::mystl_bad_typeid& e) {
        std::cout << "  捕获 mystl_bad_typeid: " << e.what() << std::endl;
    }
    
    // 测试 mystl_length_error
    try {
        throw mystl::mystl_length_error("测试长度错误异常");
    } catch (const mystl::mystl_length_error& e) {
        std::cout << "  捕获 mystl_length_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_out_of_range
    try {
        throw mystl::mystl_out_of_range("测试越界访问异常");
    } catch (const mystl::mystl_out_of_range& e) {
        std::cout << "  捕获 mystl_out_of_range: " << e.what() << std::endl;
    }
    
    // 测试 mystl_invalid_argument
    try {
        throw mystl::mystl_invalid_argument("测试无效参数异常");
    } catch (const mystl::mystl_invalid_argument& e) {
        std::cout << "  捕获 mystl_invalid_argument: " << e.what() << std::endl;
    }
    
    // 测试 mystl_domain_error
    try {
        throw mystl::mystl_domain_error("测试域错误异常");
    } catch (const mystl::mystl_domain_error& e) {
        std::cout << "  捕获 mystl_domain_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_range_error
    try {
        throw mystl::mystl_range_error("测试范围错误异常");
    } catch (const mystl::mystl_range_error& e) {
        std::cout << "  捕获 mystl_range_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_overflow_error
    try {
        throw mystl::mystl_overflow_error("测试溢出错误异常");
    } catch (const mystl::mystl_overflow_error& e) {
        std::cout << "  捕获 mystl_overflow_error: " << e.what() << std::endl;
    }
    
    // 测试 mystl_underflow_error
    try {
        throw mystl::mystl_underflow_error("测试下溢错误异常");
    } catch (const mystl::mystl_underflow_error& e) {
        std::cout << "  捕获 mystl_underflow_error: " << e.what() << std::endl;
    }
}

void testExceptionSafety() {
    std::cout << "\n=== 测试异常安全保证 ===" << std::endl;
    
    // 测试无异常构造
    std::cout << "  is_nothrow_constructible<TestClass>::value = " 
              << mystl::is_nothrow_constructible<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_constructible<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_constructible<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_constructible<TestClassThrowing>::value = " 
              << mystl::is_nothrow_constructible<TestClassThrowing>::value << std::endl;
    
    // 测试无异常拷贝构造
    std::cout << "  is_nothrow_copy_constructible<TestClass>::value = " 
              << mystl::is_nothrow_copy_constructible<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_copy_constructible<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_copy_constructible<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_copy_constructible<TestClassThrowing>::value = " 
              << mystl::is_nothrow_copy_constructible<TestClassThrowing>::value << std::endl;
    
    // 测试无异常移动构造
    std::cout << "  is_nothrow_move_constructible<TestClass>::value = " 
              << mystl::is_nothrow_move_constructible<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_move_constructible<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_move_constructible<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_move_constructible<TestClassThrowing>::value = " 
              << mystl::is_nothrow_move_constructible<TestClassThrowing>::value << std::endl;
    
    // 测试无异常赋值
    std::cout << "  is_nothrow_assignable<TestClass&, TestClass>::value = " 
              << mystl::is_nothrow_assignable<TestClass&, TestClass>::value << std::endl;
    std::cout << "  is_nothrow_assignable<TestClassNoThrow&, TestClassNoThrow>::value = " 
              << mystl::is_nothrow_assignable<TestClassNoThrow&, TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_assignable<TestClassThrowing&, TestClassThrowing>::value = " 
              << mystl::is_nothrow_assignable<TestClassThrowing&, TestClassThrowing>::value << std::endl;
    
    // 测试无异常拷贝赋值
    std::cout << "  is_nothrow_copy_assignable<TestClass>::value = " 
              << mystl::is_nothrow_copy_assignable<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_copy_assignable<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_copy_assignable<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_copy_assignable<TestClassThrowing>::value = " 
              << mystl::is_nothrow_copy_assignable<TestClassThrowing>::value << std::endl;
    
    // 测试无异常移动赋值
    std::cout << "  is_nothrow_move_assignable<TestClass>::value = " 
              << mystl::is_nothrow_move_assignable<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_move_assignable<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_move_assignable<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_move_assignable<TestClassThrowing>::value = " 
              << mystl::is_nothrow_move_assignable<TestClassThrowing>::value << std::endl;
    
    // 测试无异常析构
    std::cout << "  is_nothrow_destructible<TestClass>::value = " 
              << mystl::is_nothrow_destructible<TestClass>::value << std::endl;
    std::cout << "  is_nothrow_destructible<TestClassNoThrow>::value = " 
              << mystl::is_nothrow_destructible<TestClassNoThrow>::value << std::endl;
    std::cout << "  is_nothrow_destructible<TestClassThrowing>::value = " 
              << mystl::is_nothrow_destructible<TestClassThrowing>::value << std::endl;
}

void testExceptionSafetyChecks() {
    std::cout << "\n=== 测试异常安全保证检查 ===" << std::endl;
    
    TestClass obj1;
    TestClassNoThrow obj2;
    TestClassThrowing obj3;
    
    // 测试无异常保证检查
    std::cout << "  check_no_throw<TestClass>() = " 
              << mystl::check_no_throw<TestClass>() << std::endl;
    std::cout << "  check_no_throw<TestClassNoThrow>() = " 
              << mystl::check_no_throw<TestClassNoThrow>() << std::endl;
    std::cout << "  check_no_throw<TestClassThrowing>() = " 
              << mystl::check_no_throw<TestClassThrowing>() << std::endl;
    
    // 测试基本异常保证检查
    std::cout << "  check_basic_guarantee<TestClass>() = " 
              << mystl::check_basic_guarantee<TestClass>() << std::endl;
    std::cout << "  check_basic_guarantee<TestClassNoThrow>() = " 
              << mystl::check_basic_guarantee<TestClassNoThrow>() << std::endl;
    std::cout << "  check_basic_guarantee<TestClassThrowing>() = " 
              << mystl::check_basic_guarantee<TestClassThrowing>() << std::endl;
    
    // 测试强异常保证检查
    std::cout << "  check_strong_guarantee<TestClass>() = " 
              << mystl::check_strong_guarantee<TestClass>() << std::endl;
    std::cout << "  check_strong_guarantee<TestClassNoThrow>() = " 
              << mystl::check_strong_guarantee<TestClassNoThrow>() << std::endl;
    std::cout << "  check_strong_guarantee<TestClassThrowing>() = " 
              << mystl::check_strong_guarantee<TestClassThrowing>() << std::endl;
    
    // 测试无异常保证检查
    std::cout << "  check_no_guarantee<TestClass>() = " 
              << mystl::check_no_guarantee<TestClass>() << std::endl;
    std::cout << "  check_no_guarantee<TestClassNoThrow>() = " 
              << mystl::check_no_guarantee<TestClassNoThrow>() << std::endl;
    std::cout << "  check_no_guarantee<TestClassThrowing>() = " 
              << mystl::check_no_guarantee<TestClassThrowing>() << std::endl;
}

void testExceptionSafetyLevels() {
    std::cout << "\n=== 测试异常安全级别 ===" << std::endl;
    
    TestClass obj1;
    TestClassNoThrow obj2;
    TestClassThrowing obj3;
    
    // 测试异常安全级别
    auto level1 = mystl::test_exception_safety(obj1);
    auto level2 = mystl::test_exception_safety(obj2);
    auto level3 = mystl::test_exception_safety(obj3);
    
    std::cout << "  TestClass 异常安全级别: ";
    switch (level1) {
        case mystl::exception_safety_level::no_throw:
            std::cout << "no_throw" << std::endl;
            break;
        case mystl::exception_safety_level::basic_guarantee:
            std::cout << "basic_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::strong_guarantee:
            std::cout << "strong_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::no_guarantee:
            std::cout << "no_guarantee" << std::endl;
            break;
    }
    
    std::cout << "  TestClassNoThrow 异常安全级别: ";
    switch (level2) {
        case mystl::exception_safety_level::no_throw:
            std::cout << "no_throw" << std::endl;
            break;
        case mystl::exception_safety_level::basic_guarantee:
            std::cout << "basic_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::strong_guarantee:
            std::cout << "strong_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::no_guarantee:
            std::cout << "no_guarantee" << std::endl;
            break;
    }
    
    std::cout << "  TestClassThrowing 异常安全级别: ";
    switch (level3) {
        case mystl::exception_safety_level::no_throw:
            std::cout << "no_throw" << std::endl;
            break;
        case mystl::exception_safety_level::basic_guarantee:
            std::cout << "basic_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::strong_guarantee:
            std::cout << "strong_guarantee" << std::endl;
            break;
        case mystl::exception_safety_level::no_guarantee:
            std::cout << "no_guarantee" << std::endl;
            break;
    }
    
    // 测试各种异常安全测试函数
    std::cout << "  test_no_throw_safety<TestClass>(obj1) = " 
              << mystl::test_no_throw_safety(obj1) << std::endl;
    std::cout << "  test_no_throw_safety<TestClassNoThrow>(obj2) = " 
              << mystl::test_no_throw_safety(obj2) << std::endl;
    std::cout << "  test_no_throw_safety<TestClassThrowing>(obj3) = " 
              << mystl::test_no_throw_safety(obj3) << std::endl;
    
    std::cout << "  test_basic_guarantee_safety<TestClass>(obj1) = " 
              << mystl::test_basic_guarantee_safety(obj1) << std::endl;
    std::cout << "  test_basic_guarantee_safety<TestClassNoThrow>(obj2) = " 
              << mystl::test_basic_guarantee_safety(obj2) << std::endl;
    std::cout << "  test_basic_guarantee_safety<TestClassThrowing>(obj3) = " 
              << mystl::test_basic_guarantee_safety(obj3) << std::endl;
    
    std::cout << "  test_strong_guarantee_safety<TestClass>(obj1) = " 
              << mystl::test_strong_guarantee_safety(obj1) << std::endl;
    std::cout << "  test_strong_guarantee_safety<TestClassNoThrow>(obj2) = " 
              << mystl::test_strong_guarantee_safety(obj2) << std::endl;
    std::cout << "  test_strong_guarantee_safety<TestClassThrowing>(obj3) = " 
              << mystl::test_strong_guarantee_safety(obj3) << std::endl;
}

void testExceptionMacros() {
    std::cout << "\n=== 测试异常处理宏 ===" << std::endl;
    
    // 测试 MYSTL_TRY/CATCH 宏
    std::cout << "  测试 MYSTL_TRY/CATCH 宏:" << std::endl;
    MYSTL_TRY {
        throw mystl::mystl_exception("测试 MYSTL_TRY/CATCH 宏");
    } MYSTL_CATCH(mystl::mystl_exception) {
        std::cout << "    捕获异常: " << e.what() << std::endl;
    }
    
    // 测试 MYSTL_CATCH_ALL 宏
    std::cout << "  测试 MYSTL_CATCH_ALL 宏:" << std::endl;
    MYSTL_TRY {
        throw mystl::mystl_logic_error("测试 MYSTL_CATCH_ALL 宏");
    } MYSTL_CATCH_ALL {
        std::cout << "    捕获所有异常" << std::endl;
    }
    
    // 测试 MYSTL_THROW 宏
    std::cout << "  测试 MYSTL_THROW 宏:" << std::endl;
    MYSTL_TRY {
        MYSTL_THROW(mystl::mystl_runtime_error, "测试 MYSTL_THROW 宏");
    } MYSTL_CATCH(mystl::mystl_runtime_error) {
        std::cout << "    捕获异常: " << e.what() << std::endl;
    }
    
    // 测试调试宏
    std::cout << "  测试调试宏:" << std::endl;
    MYSTL_DEBUG_EXCEPTION("调试异常信息");
    MYSTL_EXCEPTION_TRACE("异常跟踪信息");
    MYSTL_EXCEPTION_LOG("异常日志信息");
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    std::cout << "=== MyTinySTL 异常定义系统测试 ===" << std::endl;
    
    // 测试异常类
    testExceptionClasses();
    
    // 测试异常安全保证
    testExceptionSafety();
    
    // 测试异常安全保证检查
    testExceptionSafetyChecks();
    
    // 测试异常安全级别
    testExceptionSafetyLevels();
    
    // 测试异常处理宏
    testExceptionMacros();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}
