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

int main() {
    std::cout << "=== Padding 验证 ===" << std::endl;
    
    WithoutPadding obj1;
    WithPadding obj2;
    
    // 打印结构体大小
    std::cout << "WithoutPadding 大小: " << sizeof(WithoutPadding) << " 字节" << std::endl;
    std::cout << "WithPadding 大小: " << sizeof(WithPadding) << " 字节" << std::endl;
    
    // 打印成员地址
    const char* base1 = reinterpret_cast<const char*>(&obj1);
    const char* base2 = reinterpret_cast<const char*>(&obj2);
    
    std::cout << "\nWithoutPadding 成员地址:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj1.a) - base1) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj1.c) - base1) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj1.b) - base1) << std::endl;
    
    std::cout << "\nWithPadding 成员地址:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj2.a) - base2) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj2.b) - base2) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj2.c) - base2) << std::endl;
    
    // 打印内存内容
    std::cout << "\nWithoutPadding 内存内容:" << std::endl;
    const char* ptr1 = reinterpret_cast<const char*>(&obj1);
    for (size_t i = 0; i < sizeof(WithoutPadding); ++i) {
        printf("%02X ", static_cast<unsigned char>(ptr1[i]));
    }
    std::cout << std::endl;
    
    std::cout << "\nWithPadding 内存内容:" << std::endl;
    const char* ptr2 = reinterpret_cast<const char*>(&obj2);
    for (size_t i = 0; i < sizeof(WithPadding); ++i) {
        printf("%02X ", static_cast<unsigned char>(ptr2[i]));
    }
    std::cout << std::endl;
    
    // 分析地址对齐
    std::cout << "\n地址对齐分析:" << std::endl;
    std::cout << "WithoutPadding::b 地址: " << &obj1.b << std::endl;
    std::cout << "地址 % 4 = " << (reinterpret_cast<uintptr_t>(&obj1.b) % 4) << std::endl;
    
    std::cout << "WithPadding::b 地址: " << &obj2.b << std::endl;
    std::cout << "地址 % 4 = " << (reinterpret_cast<uintptr_t>(&obj2.b) % 4) << std::endl;
    
    return 0;
}











