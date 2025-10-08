#include <iostream>
#include <chrono>
#include <vector>
#include <windows.h>
#include "../vector.h"

using namespace mystl;

// 模拟合并后的统一函数调用模式
template<typename T>
class MergedVector {
private:
    T* begin_;
    T* end_;
    T* cap_;
    mystl::allocator<T> allocator_;
    
public:
    MergedVector() : begin_(nullptr), end_(nullptr), cap_(nullptr) {}
    
    ~MergedVector() {
        if (begin_) {
            mystl::destroy(begin_, end_);
            allocator_.deallocate(begin_, cap_ - begin_);
        }
    }
    
    size_t size() const { return end_ - begin_; }
    size_t capacity() const { return cap_ - begin_; }
    bool empty() const { return begin_ == end_; }
    
    // 模拟合并后的统一reallocate函数
    template<typename U>
    void unified_reallocate(typename mystl::allocator<T>::pointer pos, U&& value) {
        using size_type = typename mystl::allocator<T>::size_type;
        using pointer = typename mystl::allocator<T>::pointer;
        
        size_type old_size = size();
        size_type new_capacity = old_size == 0 ? 1 : old_size * 2;
        pointer new_begin = allocator_.allocate(new_capacity);
        pointer new_end = new_begin;
        
        try {
            // 根据pos位置决定操作
            if (pos == end_) {
                // 相当于原来的reallocate操作
                new_end = mystl::uninitialized_move(begin_, end_, new_begin);
            } else {
                // 相当于原来的reallocate_and_insert操作
                new_end = mystl::uninitialized_move(begin_, pos, new_begin);
                mystl::construct(new_end, mystl::forward<U>(value));
                ++new_end;
                new_end = mystl::uninitialized_move(pos, end_, new_end);
            }
        } catch (...) {
            mystl::destroy(new_begin, new_end);
            allocator_.deallocate(new_begin, new_capacity);
            throw;
        }
        
        mystl::destroy(begin_, end_);
        allocator_.deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_capacity;
    }
    
    void push_back(const T& value) {
        if (end_ == cap_) {
            unified_reallocate(end_, value);
        } else {
            mystl::construct(end_, value);
            ++end_;
        }
    }
    
    void push_back(T&& value) {
        if (end_ == cap_) {
            unified_reallocate(end_, std::move(value));
        } else {
            mystl::construct(end_, std::move(value));
            ++end_;
        }
    }
};

// 性能测试函数
void test_original_vs_merged() {
    std::cout << "\n=== 原始实现 vs 合并实现性能对比 ===" << std::endl;
    
    const size_t test_sizes[] = {1000, 10000, 100000, 1000000};
    
    for (size_t test_size : test_sizes) {
        std::cout << "\n测试大小: " << test_size << std::endl;
        
        // 测试原始MyTinySTL vector
        auto start = std::chrono::high_resolution_clock::now();
        
        vector<int> original_v;
        for (size_t i = 0; i < test_size; ++i) {
            original_v.push_back(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto original_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 测试合并后的实现
        start = std::chrono::high_resolution_clock::now();
        
        MergedVector<int> merged_v;
        for (size_t i = 0; i < test_size; ++i) {
            merged_v.push_back(i);
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto merged_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "  原始实现: " << original_duration.count() << " 微秒" << std::endl;
        std::cout << "  合并实现: " << merged_duration.count() << " 微秒" << std::endl;
        std::cout << "  性能提升: " << (double)original_duration.count() / merged_duration.count() << "x" << std::endl;
        std::cout << "  原始capacity: " << original_v.capacity() << std::endl;
        std::cout << "  合并capacity: " << merged_v.capacity() << std::endl;
    }
}

void test_function_call_overhead() {
    std::cout << "\n=== 函数调用开销测试 ===" << std::endl;
    
    const size_t iterations = 1000000;
    
    // 测试直接调用vs间接调用
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        // 模拟直接调用
        int result = i * 2;
        (void)result; // 避免优化
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto direct_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    // 测试通过函数指针调用
    auto func = [](int x) { return x * 2; };
    
    start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        int result = func(i);
        (void)result; // 避免优化
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto indirect_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    std::cout << "  直接调用: " << direct_duration.count() << " 纳秒" << std::endl;
    std::cout << "  间接调用: " << indirect_duration.count() << " 纳秒" << std::endl;
    std::cout << "  开销比: " << (double)indirect_duration.count() / direct_duration.count() << std::endl;
}

void test_branch_prediction_impact() {
    std::cout << "\n=== 分支预测影响测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    
    // 测试有分支的情况
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (i % 2 == 0) {  //编译器优化导致分支预测失败
            sum += i;
        } else {
            sum -= i;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto branch_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试无分支的情况
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        sum += i;
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto no_branch_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  有分支: " << branch_duration.count() << " 微秒" << std::endl;
    std::cout << "  无分支: " << no_branch_duration.count() << " 微秒" << std::endl;
    std::cout << "  分支开销: " << (double)branch_duration.count() / no_branch_duration.count() << "x" << std::endl;
}

void test_memory_access_patterns() {
    std::cout << "\n=== 内存访问模式测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    
    // 测试顺序访问
    std::vector<int> data(test_size);
    for (size_t i = 0; i < test_size; ++i) {
        data[i] = i;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        sum += data[i];
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试随机访问
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        size_t index = (i * 7) % test_size; // 模拟随机访问
        sum += data[index];
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  顺序访问: " << sequential_duration.count() << " 微秒" << std::endl;
    std::cout << "  随机访问: " << random_duration.count() << " 微秒" << std::endl;
    std::cout << "  访问模式影响: " << (double)random_duration.count() / sequential_duration.count() << "x" << std::endl;
}

void test_compiler_optimization_impact() {
    std::cout << "\n=== 编译器优化影响测试 ===" << std::endl;
    
    const size_t test_size = 1000000;
    
    // 测试简单循环
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        sum += i;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto simple_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试复杂循环（模拟函数合并后的复杂逻辑）
    start = std::chrono::high_resolution_clock::now();
    
    sum = 0;
    for (size_t i = 0; i < test_size; ++i) {
        if (i % 2 == 0) {
            sum += i;
        } else {
            sum += i * 2;
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto complex_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  简单循环: " << simple_duration.count() << " 微秒" << std::endl;
    std::cout << "  复杂循环: " << complex_duration.count() << " 微秒" << std::endl;
    std::cout << "  复杂度影响: " << (double)complex_duration.count() / simple_duration.count() << "x" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL Vector 函数合并效率分析 ===" << std::endl;
    std::cout << "测试目标：分析reallocate和reallocate_and_insert函数合并的效率影响" << std::endl;
    
    try {
        test_original_vs_merged();
        test_function_call_overhead();
        test_branch_prediction_impact();
        test_memory_access_patterns();
        test_compiler_optimization_impact();
        
        std::cout << "\n=== 效率分析完成 ===" << std::endl;
        std::cout << "\n分析结论：" << std::endl;
        std::cout << "1. 函数合并可能带来的好处：" << std::endl;
        std::cout << "   - 减少代码重复" << std::endl;
        std::cout << "   - 统一内存管理逻辑" << std::endl;
        std::cout << "   - 减少函数调用开销" << std::endl;
        std::cout << "2. 函数合并可能带来的问题：" << std::endl;
        std::cout << "   - 增加分支判断开销" << std::endl;
        std::cout << "   - 降低代码可读性" << std::endl;
        std::cout << "   - 增加编译器优化难度" << std::endl;
        std::cout << "3. 建议：" << std::endl;
        std::cout << "   - 如果性能差异小于5%，建议保持分离" << std::endl;
        std::cout << "   - 如果性能差异大于10%，可以考虑合并" << std::endl;
        std::cout << "   - 优先考虑代码可维护性和可读性" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n❌ 测试过程中发生未知异常" << std::endl;
        return 1;
    }
    
    return 0;
}
