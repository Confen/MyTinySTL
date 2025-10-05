#include <iostream>
#include <cstddef>

// 测试不同的数据类型
void testBasicTypes() {
    std::cout << "=== 基本类型对齐 ===" << std::endl;
    
    std::cout << "char:        " << alignof(char) << " 字节" << std::endl;
    std::cout << "short:       " << alignof(short) << " 字节" << std::endl;
    std::cout << "int:         " << alignof(int) << " 字节" << std::endl;
    std::cout << "long:        " << alignof(long) << " 字节" << std::endl;
    std::cout << "long long:   " << alignof(long long) << " 字节" << std::endl;
    std::cout << "float:       " << alignof(float) << " 字节" << std::endl;
    std::cout << "double:      " << alignof(double) << " 字节" << std::endl;
    std::cout << "long double: " << alignof(long double) << " 字节" << std::endl;
    std::cout << "void*:       " << alignof(void*) << " 字节" << std::endl;
    std::cout << "int*:        " << alignof(int*) << " 字节" << std::endl;
}

// 测试结构体对齐
void testStructAlignment() {
    std::cout << "\n=== 结构体对齐 ===" << std::endl;
    
    struct Test1 {
        char a;
        int b;
    };
    
    struct Test2 {
        char a;
        double b;
    };
    
    struct Test3 {
        char a;
        char b;
        int c;
    };
    
    struct Test4 {
        int a;
        char b;
        char c;
    };
    
    std::cout << "struct { char a; int b; }:        " << alignof(Test1) << " 字节" << std::endl;
    std::cout << "struct { char a; double b; }:     " << alignof(Test2) << " 字节" << std::endl;
    std::cout << "struct { char a; char b; int c; }:" << alignof(Test3) << " 字节" << std::endl;
    std::cout << "struct { int a; char b; char c; }:" << alignof(Test4) << " 字节" << std::endl;
}

// 测试数组对齐
void testArrayAlignment() {
    std::cout << "\n=== 数组对齐 ===" << std::endl;
    
    std::cout << "char[10]:    " << alignof(char[10]) << " 字节" << std::endl;
    std::cout << "int[10]:     " << alignof(int[10]) << " 字节" << std::endl;
    std::cout << "double[10]:  " << alignof(double[10]) << " 字节" << std::endl;
}

// 测试联合体对齐
void testUnionAlignment() {
    std::cout << "\n=== 联合体对齐 ===" << std::endl;
    
    union TestUnion {
        char a;
        int b;
        double c;
    };
    
    std::cout << "union { char a; int b; double c; }:" << alignof(TestUnion) << " 字节" << std::endl;
}

// 测试虚函数对齐
void testVirtualAlignment() {
    std::cout << "\n=== 虚函数对齐 ===" << std::endl;
    
    struct Base {
        virtual void func() {}
        char a;
    };
    
    struct Derived : public Base {
        int b;
    };
    
    std::cout << "struct with virtual: " << alignof(Base) << " 字节" << std::endl;
    std::cout << "derived struct:      " << alignof(Derived) << " 字节" << std::endl;
}

// 测试自定义对齐
void testCustomAlignment() {
    std::cout << "\n=== 自定义对齐 ===" << std::endl;
    
    // 使用 alignas 指定对齐
    struct alignas(1) Packed1 {
        char a;
        int b;
    };
    
    struct alignas(4) Aligned4 {
        char a;
        int b;
    };
    
    struct alignas(8) Aligned8 {
        char a;
        int b;
    };
    
    struct alignas(16) Aligned16 {
        char a;
        int b;
    };
    
    std::cout << "alignas(1):  " << alignof(Packed1) << " 字节" << std::endl;
    std::cout << "alignas(4):  " << alignof(Aligned4) << " 字节" << std::endl;
    std::cout << "alignas(8):  " << alignof(Aligned8) << " 字节" << std::endl;
    std::cout << "alignas(16): " << alignof(Aligned16) << " 字节" << std::endl;
}

// 测试成员对齐
void testMemberAlignment() {
    std::cout << "\n=== 成员对齐测试 ===" << std::endl;
    
    struct TestStruct {
        char a;
        int b;
        char c;
    };
    
    TestStruct obj;
    const char* base = reinterpret_cast<const char*>(&obj);
    
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj.a) - base) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj.b) - base) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj.c) - base) << std::endl;
    
    std::cout << "\n地址对齐检查:" << std::endl;
    std::cout << "  a 地址 % 1 = " << (reinterpret_cast<uintptr_t>(&obj.a) % 1) << std::endl;
    std::cout << "  b 地址 % 4 = " << (reinterpret_cast<uintptr_t>(&obj.b) % 4) << std::endl;
    std::cout << "  c 地址 % 1 = " << (reinterpret_cast<uintptr_t>(&obj.c) % 1) << std::endl;
}

// 测试平台相关对齐
void testPlatformAlignment() {
    std::cout << "\n=== 平台相关对齐 ===" << std::endl;
    
    std::cout << "sizeof(void*): " << sizeof(void*) << " 字节" << std::endl;
    std::cout << "alignof(void*): " << alignof(void*) << " 字节" << std::endl;
    
    std::cout << "sizeof(long): " << sizeof(long) << " 字节" << std::endl;
    std::cout << "alignof(long): " << alignof(long) << " 字节" << std::endl;
    
    std::cout << "sizeof(long long): " << sizeof(long long) << " 字节" << std::endl;
    std::cout << "alignof(long long): " << alignof(long long) << " 字节" << std::endl;
}

int main() {
    std::cout << "=== C++ 对齐要求详解 ===" << std::endl;
    
    testBasicTypes();
    testStructAlignment();
    testArrayAlignment();
    testUnionAlignment();
    testVirtualAlignment();
    testCustomAlignment();
    testMemberAlignment();
    testPlatformAlignment();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "1. 使用 alignof(T) 获取类型 T 的对齐要求" << std::endl;
    std::cout << "2. 结构体的对齐 = 最大成员的对齐" << std::endl;
    std::cout << "3. 地址 % 对齐要求 = 0 表示正确对齐" << std::endl;
    std::cout << "4. 可以使用 alignas 指定自定义对齐" << std::endl;
    
    return 0;
}




















