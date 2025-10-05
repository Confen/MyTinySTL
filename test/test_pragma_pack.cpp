#include <iostream>
#include <cstddef>

// 默认对齐的结构体
struct DefaultAlign {
    char a;
    int b;
    char c;
    double d;
};

// 1 字节对齐的结构体
#pragma pack(1)
struct Pack1 {
    char a;
    int b;
    char c;
    double d;
};
#pragma pack()

// 2 字节对齐的结构体
#pragma pack(2)
struct Pack2 {
    char a;
    int b;
    char c;
    double d;
};
#pragma pack()

// 4 字节对齐的结构体
#pragma pack(4)
struct Pack4 {
    char a;
    int b;
    char c;
    double d;
};
#pragma pack()

// 8 字节对齐的结构体
#pragma pack(8)
struct Pack8 {
    char a;
    int b;
    char c;
    double d;
};
#pragma pack()

// 打印结构体信息的函数
template<typename T>
void printStructInfo(const char* name, const T& obj) {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << "大小: " << sizeof(T) << " 字节" << std::endl;
    std::cout << "对齐: " << alignof(T) << " 字节" << std::endl;
    
    const char* base = reinterpret_cast<const char*>(&obj);
    std::cout << "成员地址偏移:" << std::endl;
    std::cout << "  a: " << (reinterpret_cast<const char*>(&obj.a) - base) << std::endl;
    std::cout << "  b: " << (reinterpret_cast<const char*>(&obj.b) - base) << std::endl;
    std::cout << "  c: " << (reinterpret_cast<const char*>(&obj.c) - base) << std::endl;
    std::cout << "  d: " << (reinterpret_cast<const char*>(&obj.d) - base) << std::endl;
    
    // 计算 padding
    int padding_before_b = (reinterpret_cast<const char*>(&obj.b) - base) - 
                          (reinterpret_cast<const char*>(&obj.a) - base) - 1;
    int padding_before_c = (reinterpret_cast<const char*>(&obj.c) - base) - 
                          (reinterpret_cast<const char*>(&obj.b) - base) - 4;
    int padding_before_d = (reinterpret_cast<const char*>(&obj.d) - base) - 
                          (reinterpret_cast<const char*>(&obj.c) - base) - 1;
    
    std::cout << "Padding 分析:" << std::endl;
    std::cout << "  a 和 b 之间: " << padding_before_b << " 字节" << std::endl;
    std::cout << "  b 和 c 之间: " << padding_before_c << " 字节" << std::endl;
    std::cout << "  c 和 d 之间: " << padding_before_d << " 字节" << std::endl;
    
    // 打印内存内容
    std::cout << "内存内容:" << std::endl;
    const char* ptr = reinterpret_cast<const char*>(&obj);
    for (size_t i = 0; i < sizeof(T); ++i) {
        printf("%02X ", static_cast<unsigned char>(ptr[i]));
        if ((i + 1) % 8 == 0) std::cout << std::endl;
    }
    if (sizeof(T) % 8 != 0) std::cout << std::endl;
}

// 测试不同对齐的影响
void testAlignmentImpact() {
    std::cout << "\n=== 对齐影响测试 ===" << std::endl;
    
    const int N = 1000000;
    
    std::cout << "100万个对象的内存使用:" << std::endl;
    std::cout << "DefaultAlign: " << sizeof(DefaultAlign) * N << " 字节" << std::endl;
    std::cout << "Pack1:        " << sizeof(Pack1) * N << " 字节" << std::endl;
    std::cout << "Pack2:        " << sizeof(Pack2) * N << " 字节" << std::endl;
    std::cout << "Pack4:        " << sizeof(Pack4) * N << " 字节" << std::endl;
    std::cout << "Pack8:        " << sizeof(Pack8) * N << " 字节" << std::endl;
    
    std::cout << "\n内存节省 (相比默认对齐):" << std::endl;
    std::cout << "Pack1: " << (sizeof(DefaultAlign) - sizeof(Pack1)) * N << " 字节" << std::endl;
    std::cout << "Pack2: " << (sizeof(DefaultAlign) - sizeof(Pack2)) * N << " 字节" << std::endl;
    std::cout << "Pack4: " << (sizeof(DefaultAlign) - sizeof(Pack4)) * N << " 字节" << std::endl;
    std::cout << "Pack8: " << (sizeof(DefaultAlign) - sizeof(Pack8)) * N << " 字节" << std::endl;
}

// 测试性能影响
void testPerformanceImpact() {
    std::cout << "\n=== 性能影响测试 ===" << std::endl;
    
    // 创建测试数组
    const int N = 100000;
    DefaultAlign* default_array = new DefaultAlign[N];
    Pack1* pack1_array = new Pack1[N];
    
    // 初始化
    for (int i = 0; i < N; ++i) {
        default_array[i].a = 'A';
        default_array[i].b = i;
        default_array[i].c = 'C';
        default_array[i].d = i * 3.14;
        
        pack1_array[i].a = 'A';
        pack1_array[i].b = i;
        pack1_array[i].c = 'C';
        pack1_array[i].d = i * 3.14;
    }
    
    // 测试访问性能
    volatile int sum = 0;
    
    // 测试默认对齐
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += default_array[i].b;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto default_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试 1 字节对齐
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += pack1_array[i].b;
    }
    end = std::chrono::high_resolution_clock::now();
    auto pack1_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "默认对齐访问时间: " << default_time.count() << " 微秒" << std::endl;
    std::cout << "1字节对齐访问时间: " << pack1_time.count() << " 微秒" << std::endl;
    std::cout << "性能差异: " << (pack1_time.count() - default_time.count()) << " 微秒" << std::endl;
    
    delete[] default_array;
    delete[] pack1_array;
}

int main() {
    std::cout << "=== #pragma pack 详解 ===" << std::endl;
    
    // 创建测试对象
    DefaultAlign default_obj;
    Pack1 pack1_obj;
    Pack2 pack2_obj;
    Pack4 pack4_obj;
    Pack8 pack8_obj;
    
    // 打印各种对齐的信息
    printStructInfo("DefaultAlign (默认对齐)", default_obj);
    printStructInfo("Pack1 (1字节对齐)", pack1_obj);
    printStructInfo("Pack2 (2字节对齐)", pack2_obj);
    printStructInfo("Pack4 (4字节对齐)", pack4_obj);
    printStructInfo("Pack8 (8字节对齐)", pack8_obj);
    
    // 测试对齐影响
    testAlignmentImpact();
    
    // 测试性能影响
    testPerformanceImpact();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "1. #pragma pack(1) 强制 1 字节对齐，消除 padding" << std::endl;
    std::cout << "2. 减少内存使用，但可能降低访问性能" << std::endl;
    std::cout << "3. 适用于内存敏感的场景" << std::endl;
    std::cout << "4. 使用后要用 #pragma pack() 恢复默认对齐" << std::endl;
    
    return 0;
}




















