#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <windows.h>

void demonstrate_shuffle_basic() {
    std::cout << "\n=== shuffle基本功能演示 ===" << std::endl;
    
    // 创建顺序数组
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::cout << "原始数据: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用shuffle打乱
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(data.begin(), data.end(), gen);
    
    std::cout << "打乱后: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void demonstrate_shuffle_algorithm() {
    std::cout << "\n=== shuffle算法原理演示 ===" << std::endl;
    
    std::cout << "shuffle使用Fisher-Yates洗牌算法:" << std::endl;
    std::cout << "1. 从最后一个元素开始" << std::endl;
    std::cout << "2. 生成一个随机索引(0到当前位置)" << std::endl;
    std::cout << "3. 交换当前元素和随机索引位置的元素" << std::endl;
    std::cout << "4. 移动到前一个位置，重复步骤2-3" << std::endl;
    
    // 手动实现shuffle算法演示
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "\n手动实现shuffle过程:" << std::endl;
    std::cout << "初始: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 从最后一个元素开始
    for (int i = data.size() - 1; i > 0; --i) {
        // 生成随机索引
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(gen);
        
        // 交换元素
        std::swap(data[i], data[j]);
        
        std::cout << "步骤" << (data.size() - i) << ": ";
        for (int val : data) {
            std::cout << val << " ";
        }
        std::cout << " (交换位置" << i << "和" << j << ")" << std::endl;
    }
}

void demonstrate_shuffle_randomness() {
    std::cout << "\n=== shuffle随机性演示 ===" << std::endl;
    
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "多次shuffle结果:" << std::endl;
    
    for (int round = 1; round <= 5; ++round) {
        // 重置数据
        data = {1, 2, 3, 4, 5};
        
        // 打乱
        std::shuffle(data.begin(), data.end(), gen);
        
        std::cout << "第" << round << "次: ";
        for (int val : data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void demonstrate_shuffle_with_different_generators() {
    std::cout << "\n=== 不同随机数生成器的shuffle ===" << std::endl;
    
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用不同的随机数生成器
    std::random_device rd;
    std::mt19937 gen1(rd());
    std::mt19937 gen2(12345);  // 固定种子
    std::mt19937 gen3(54321);  // 不同固定种子
    
    std::cout << "原始数据: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用gen1
    std::vector<int> data1 = data;
    std::shuffle(data1.begin(), data1.end(), gen1);
    std::cout << "随机种子: ";
    for (int val : data1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用gen2
    std::vector<int> data2 = data;
    std::shuffle(data2.begin(), data2.end(), gen2);
    std::cout << "固定种子1: ";
    for (int val : data2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用gen3
    std::vector<int> data3 = data;
    std::shuffle(data3.begin(), data3.end(), gen3);
    std::cout << "固定种子2: ";
    for (int val : data3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void demonstrate_shuffle_in_memory_access_test() {
    std::cout << "\n=== shuffle在内存访问测试中的作用 ===" << std::endl;
    
    const size_t test_size = 10;
    std::vector<size_t> indices(test_size);
    
    // 创建顺序索引
    for (size_t i = 0; i < test_size; ++i) {
        indices[i] = i;
    }
    
    std::cout << "顺序索引: ";
    for (size_t val : indices) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用shuffle打乱索引
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);
    
    std::cout << "随机索引: ";
    for (size_t val : indices) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\n在内存访问测试中:" << std::endl;
    std::cout << "1. 创建顺序索引数组 [0,1,2,3,4,5,6,7,8,9]" << std::endl;
    std::cout << "2. 使用shuffle打乱索引顺序" << std::endl;
    std::cout << "3. 使用打乱后的索引访问数据" << std::endl;
    std::cout << "4. 这样就实现了随机访问模式" << std::endl;
}

void demonstrate_shuffle_performance() {
    std::cout << "\n=== shuffle性能测试 ===" << std::endl;
    
    const size_t test_sizes[] = {1000, 10000, 100000, 1000000};
    
    for (size_t test_size : test_sizes) {
        std::vector<size_t> data(test_size);
        
        // 初始化数据
        for (size_t i = 0; i < test_size; ++i) {
            data[i] = i;
        }
        
        // 测试shuffle性能
        auto start = std::chrono::high_resolution_clock::now();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(data.begin(), data.end(), gen);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "大小 " << test_size << ": " << duration.count() << " 微秒" << std::endl;
    }
}

void demonstrate_shuffle_vs_other_methods() {
    std::cout << "\n=== shuffle与其他方法的比较 ===" << std::endl;
    
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::cout << "原始数据: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 方法1：使用shuffle
    std::vector<int> data1 = data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(data1.begin(), data1.end(), gen);
    std::cout << "shuffle: ";
    for (int val : data1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 方法2：使用random_shuffle (已废弃)
    std::vector<int> data2 = data;
    std::random_shuffle(data2.begin(), data2.end());
    std::cout << "random_shuffle: ";
    for (int val : data2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 方法3：使用sort with random comparator
    std::vector<int> data3 = data;
    std::sort(data3.begin(), data3.end(), [](int a, int b) {
        return std::rand() % 2;
    });
    std::cout << "sort+random: ";
    for (int val : data3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== std::shuffle 详解 ===" << std::endl;
    
    try {
        demonstrate_shuffle_basic();
        demonstrate_shuffle_algorithm();
        demonstrate_shuffle_randomness();
        demonstrate_shuffle_with_different_generators();
        demonstrate_shuffle_in_memory_access_test();
        demonstrate_shuffle_performance();
        demonstrate_shuffle_vs_other_methods();
        
        std::cout << "\n=== 总结 ===" << std::endl;
        std::cout << "1. shuffle的作用：随机打乱容器中元素的顺序" << std::endl;
        std::cout << "2. 算法：使用Fisher-Yates洗牌算法" << std::endl;
        std::cout << "3. 随机性：每次调用产生不同的随机排列" << std::endl;
        std::cout << "4. 性能：O(n)时间复杂度" << std::endl;
        std::cout << "5. 应用：在内存访问测试中创建随机访问模式" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}




