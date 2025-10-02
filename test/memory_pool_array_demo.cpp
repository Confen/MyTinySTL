#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 简化的内存池演示
class SimpleMemoryPool {
private:
    char* pool_;           // 内存池数组
    size_t pool_size_;     // 内存池大小
    size_t block_size_;    // 块大小
    size_t max_blocks_;    // 最大块数
    bool* used_blocks_;    // 使用状态数组
    
public:
    SimpleMemoryPool(size_t pool_size, size_t block_size) 
        : pool_size_(pool_size), block_size_(block_size) {
        
        max_blocks_ = pool_size_ / block_size_;
        
        // 分配内存池
        pool_ = new char[pool_size_];
        
        // 分配使用状态数组
        used_blocks_ = new bool[max_blocks_];
        
        // 初始化状态
        for (size_t i = 0; i < max_blocks_; ++i) {
            used_blocks_[i] = false;
        }
        
        std::cout << "内存池构造完成：" << std::endl;
        std::cout << "  总大小: " << pool_size_ << " 字节" << std::endl;
        std::cout << "  块大小: " << block_size_ << " 字节" << std::endl;
        std::cout << "  最大块数: " << max_blocks_ << std::endl;
    }
    
    ~SimpleMemoryPool() {
        delete[] pool_;
        delete[] used_blocks_;
        std::cout << "内存池析构完成" << std::endl;
    }
    
    // 分配内存块
    void* allocate() {
        for (size_t i = 0; i < max_blocks_; ++i) {
            if (!used_blocks_[i]) {
                used_blocks_[i] = true;
                void* ptr = pool_ + (i * block_size_);
                std::cout << "分配块 " << i << "，地址: " << ptr << std::endl;
                return ptr;
            }
        }
        std::cout << "内存池已满，无法分配" << std::endl;
        return nullptr;
    }
    
    // 释放内存块
    void deallocate(void* ptr) {
        if (ptr == nullptr) return;
        
        // 计算块索引
        size_t block_index = (static_cast<char*>(ptr) - pool_) / block_size_;
        
        if (block_index < max_blocks_ && used_blocks_[block_index]) {
            used_blocks_[block_index] = false;
            std::cout << "释放块 " << block_index << "，地址: " << ptr << std::endl;
        } else {
            std::cout << "无效的指针: " << ptr << std::endl;
        }
    }
    
    // 打印内存池状态
    void printStatus() {
        std::cout << "\n内存池状态：" << std::endl;
        std::cout << "  已使用块: ";
        for (size_t i = 0; i < max_blocks_; ++i) {
            if (used_blocks_[i]) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
        
        std::cout << "  空闲块: ";
        for (size_t i = 0; i < max_blocks_; ++i) {
            if (!used_blocks_[i]) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }
    
    // 获取可用块数
    size_t getAvailableBlocks() const {
        size_t count = 0;
        for (size_t i = 0; i < max_blocks_; ++i) {
            if (!used_blocks_[i]) {
                ++count;
            }
        }
        return count;
    }
};

// 演示类：使用内存池
class PooledObject {
private:
    int value_;
    static int count_;
    
public:
    PooledObject(int v) : value_(v) {
        ++count_;
        std::cout << "构造 PooledObject(" << value_ << "), 总数: " << count_ << std::endl;
    }
    
    ~PooledObject() {
        --count_;
        std::cout << "析构 PooledObject(" << value_ << "), 剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int PooledObject::count_ = 0;

// 演示1：内存池的数组特性
void demo_memory_pool_array_property() {
    std::cout << "=== 演示1：内存池的数组特性 ===" << std::endl;
    
    // 创建内存池：1000字节，每块100字节
    SimpleMemoryPool pool(1000, 100);
    
    std::cout << "\n初始状态：" << std::endl;
    pool.printStatus();
    
    // 分配一些块
    std::vector<void*> allocated_blocks;
    
    std::cout << "\n分配块：" << std::endl;
    for (int i = 0; i < 5; ++i) {
        void* block = pool.allocate();
        if (block) {
            allocated_blocks.push_back(block);
        }
    }
    
    pool.printStatus();
    
    // 释放一些块（不按顺序）
    std::cout << "\n释放块（不按顺序）：" << std::endl;
    pool.deallocate(allocated_blocks[1]);  // 释放第2个
    pool.deallocate(allocated_blocks[3]);  // 释放第4个
    
    pool.printStatus();
    
    // 重新分配
    std::cout << "\n重新分配：" << std::endl;
    void* new_block1 = pool.allocate();
    void* new_block2 = pool.allocate();
    
    pool.printStatus();
}

// 演示2：内存池的灵活性
void demo_memory_pool_flexibility() {
    std::cout << "\n=== 演示2：内存池的灵活性 ===" << std::endl;
    
    SimpleMemoryPool pool(800, 80);  // 800字节，每块80字节
    
    std::cout << "可用块数: " << pool.getAvailableBlocks() << std::endl;
    
    // 分配块
    std::vector<void*> blocks;
    for (int i = 0; i < 3; ++i) {
        void* block = pool.allocate();
        if (block) {
            blocks.push_back(block);
        }
    }
    
    std::cout << "分配3个块后，可用块数: " << pool.getAvailableBlocks() << std::endl;
    
    // 释放中间块
    pool.deallocate(blocks[1]);
    std::cout << "释放中间块后，可用块数: " << pool.getAvailableBlocks() << std::endl;
    
    // 重新分配
    void* new_block = pool.allocate();
    std::cout << "重新分配后，可用块数: " << pool.getAvailableBlocks() << std::endl;
    
    pool.printStatus();
}

// 演示3：内存池与对象构造
void demo_memory_pool_with_objects() {
    std::cout << "\n=== 演示3：内存池与对象构造 ===" << std::endl;
    
    SimpleMemoryPool pool(1000, 100);
    PooledObject::resetCount();
    
    // 在内存池中构造对象
    std::vector<PooledObject*> objects;
    
    std::cout << "在内存池中构造对象：" << std::endl;
    for (int i = 0; i < 3; ++i) {
        void* block = pool.allocate();
        if (block) {
            PooledObject* obj = new(block) PooledObject(i + 1);
            objects.push_back(obj);
        }
    }
    
    std::cout << "构造完成，对象数量: " << PooledObject::getCount() << std::endl;
    pool.printStatus();
    
    // 析构对象
    std::cout << "\n析构对象：" << std::endl;
    for (auto* obj : objects) {
        obj->~PooledObject();
        pool.deallocate(obj);
    }
    
    std::cout << "析构完成，对象数量: " << PooledObject::getCount() << std::endl;
    pool.printStatus();
}

// 演示4：内存池的优势
void demo_memory_pool_advantages() {
    std::cout << "\n=== 演示4：内存池的优势 ===" << std::endl;
    
    const int iterations = 1000;
    
    // 方式1：直接 new/delete
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        int* ptr = new int(i);
        delete ptr;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 方式2：内存池
    SimpleMemoryPool pool(10000, sizeof(int));
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        void* block = pool.allocate();
        if (block) {
            int* ptr = new(block) int(i);
            ptr->~int();
            pool.deallocate(block);
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    std::cout << "new/delete 方式耗时: " << duration1.count() << " 微秒" << std::endl;
    std::cout << "内存池方式耗时: " << duration2.count() << " 微秒" << std::endl;
    std::cout << "性能提升: " << (double)duration1.count() / duration2.count() << " 倍" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demo_memory_pool_array_property();
    demo_memory_pool_flexibility();
    demo_memory_pool_with_objects();
    demo_memory_pool_advantages();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "内存池的数组特性：" << std::endl;
    std::cout << "1. 连续内存：像数组一样连续分配" << std::endl;
    std::cout << "2. 灵活分配：可以随意分配和释放任意位置" << std::endl;
    std::cout << "3. 状态跟踪：通过状态数组跟踪使用情况" << std::endl;
    std::cout << "4. 高效管理：避免频繁的系统调用" << std::endl;
    std::cout << "5. 内存复用：释放的内存可以重新使用" << std::endl;
    
    return 0;
}








