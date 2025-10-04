#include <iostream>
#include <cstddef>

struct WithoutPadding {
    char a;
    char c;
    int b;
};

struct WithPadding {
    char a;
    int b;
    char c;
};

#pragma pack(1)
struct ReallyNoPadding {
    char a;
    char c;
    int b;
};
#pragma pack()

struct Optimized {
    int b;
    char a;
    char c;
};

int main() {
    std::cout << "=== 真实的 Padding 分析 ===" << std::endl;
    
    // 1. 分析 WithoutPadding（实际上有 padding）
    std::cout << "\n1. WithoutPadding 分析:" << std::endl;
    std::cout << "大小: " << sizeof(WithoutPadding) << " 字节" << std::endl;
    std::cout << "对齐: " << alignof(WithoutPadding) << " 字节" << std::endl;
    
    WithoutPadding obj1;
    const char* base1 = reinterpret_cast<const char*>(&obj1);
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj1.a) - base1) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj1.c) - base1) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj1.b) - base1) << std::endl;
    
    // 计算 padding
    int padding_before_b = (reinterpret_cast<const char*>(&obj1.b) - base1) - 
                          (reinterpret_cast<const char*>(&obj1.c) - base1) - 1;
    std::cout << "  b 前面的 padding: " << padding_before_b << " 字节" << std::endl;
    
    // 2. 分析 WithPadding
    std::cout << "\n2. WithPadding 分析:" << std::endl;
    std::cout << "大小: " << sizeof(WithPadding) << " 字节" << std::endl;
    std::cout << "对齐: " << alignof(WithPadding) << " 字节" << std::endl;
    
    WithPadding obj2;
    const char* base2 = reinterpret_cast<const char*>(&obj2);
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj2.a) - base2) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj2.b) - base2) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj2.c) - base2) << std::endl;
    
    // 3. 分析 ReallyNoPadding
    std::cout << "\n3. ReallyNoPadding 分析:" << std::endl;
    std::cout << "大小: " << sizeof(ReallyNoPadding) << " 字节" << std::endl;
    std::cout << "对齐: " << alignof(ReallyNoPadding) << " 字节" << std::endl;
    
    ReallyNoPadding obj3;
    const char* base3 = reinterpret_cast<const char*>(&obj3);
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj3.a) - base3) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj3.c) - base3) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj3.b) - base3) << std::endl;
    
    // 4. 分析 Optimized
    std::cout << "\n4. Optimized 分析:" << std::endl;
    std::cout << "大小: " << sizeof(Optimized) << " 字节" << std::endl;
    std::cout << "对齐: " << alignof(Optimized) << " 字节" << std::endl;
    
    Optimized obj4;
    const char* base4 = reinterpret_cast<const char*>(&obj4);
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj4.b) - base4) << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj4.a) - base4) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj4.c) - base4) << std::endl;
    
    // 5. 内存内容对比
    std::cout << "\n5. 内存内容对比:" << std::endl;
    
    std::cout << "WithoutPadding 内存内容:" << std::endl;
    const char* ptr1 = reinterpret_cast<const char*>(&obj1);
    for (size_t i = 0; i < sizeof(WithoutPadding); ++i) {
        printf("%02X ", static_cast<unsigned char>(ptr1[i]));
    }
    std::cout << std::endl;
    
    std::cout << "ReallyNoPadding 内存内容:" << std::endl;
    const char* ptr3 = reinterpret_cast<const char*>(&obj3);
    for (size_t i = 0; i < sizeof(ReallyNoPadding); ++i) {
        printf("%02X ", static_cast<unsigned char>(ptr3[i]));
    }
    std::cout << std::endl;
    
    // 6. 总结
    std::cout << "\n6. 总结:" << std::endl;
    std::cout << "WithoutPadding: " << sizeof(WithoutPadding) << " 字节 (有 padding)" << std::endl;
    std::cout << "WithPadding:    " << sizeof(WithPadding) << " 字节 (有 padding)" << std::endl;
    std::cout << "ReallyNoPadding:" << sizeof(ReallyNoPadding) << " 字节 (无 padding)" << std::endl;
    std::cout << "Optimized:      " << sizeof(Optimized) << " 字节 (padding 最少)" << std::endl;
    
    return 0;
}



















