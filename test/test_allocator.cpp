#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <windows.h>

// 简化的空间配置器实现，避免头文件冲突
namespace mystl {

// 简化的标准分配器
template<typename T>
class simple_allocator {
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    template<typename U>
    struct rebind {
        typedef simple_allocator<U> other;
    };

    simple_allocator() noexcept = default;
    simple_allocator(const simple_allocator&) noexcept = default;
    template<typename U>
    simple_allocator(const simple_allocator<U>&) noexcept {}
    ~simple_allocator() noexcept = default;

    pointer address(reference x) const noexcept {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept {
        return &x;
    }

    pointer allocate(size_type n, const void* hint = 0) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        
        size_type total_size = n * sizeof(T);
        pointer result = static_cast<pointer>(::operator new(total_size));
        
        if (result == nullptr) {
            throw std::bad_alloc();
        }
        
        return result;
    }

    void deallocate(pointer p, size_type n) {
        if (p != nullptr) {
            ::operator delete(p);
        }
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) {
        if (p != nullptr) {
            p->~U();
        }
    }

    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    template<typename U>
    bool operator==(const simple_allocator<U>&) const noexcept {
        return true;
    }

    template<typename U>
    bool operator!=(const simple_allocator<U>&) const noexcept {
        return false;
    }
};

// 简化的内存池分配器
class simple_pool_allocator {
private:
    static const size_t POOL_SIZE = 1024 * 1024; // 1MB 内存池
    static char pool_[POOL_SIZE];
    static size_t pool_offset_;
    static std::mutex mutex_;

public:
    static void* allocate(size_t n) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (pool_offset_ + n > POOL_SIZE) {
            throw std::bad_alloc();
        }
        
        void* result = pool_ + pool_offset_;
        pool_offset_ += n;
        return result;
    }

    static void deallocate(void* p, size_t n) {
        // 简化的内存池不回收内存
        // 实际实现中需要更复杂的内存管理
    }

    static void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        pool_offset_ = 0;
    }

    static size_t get_used_size() {
        return pool_offset_;
    }

    static size_t get_total_size() {
        return POOL_SIZE;
    }
};

// 静态成员定义
char simple_pool_allocator::pool_[POOL_SIZE];
size_t simple_pool_allocator::pool_offset_ = 0;
std::mutex simple_pool_allocator::mutex_;

// 内存池分配器包装器
template<typename T>
class pool_allocator {
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    template<typename U>
    struct rebind {
        typedef pool_allocator<U> other;
    };

    pool_allocator() noexcept = default;
    pool_allocator(const pool_allocator&) noexcept = default;
    template<typename U>
    pool_allocator(const pool_allocator<U>&) noexcept {}
    ~pool_allocator() noexcept = default;

    pointer address(reference x) const noexcept {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept {
        return &x;
    }

    pointer allocate(size_type n, const void* hint = 0) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        
        size_type total_size = n * sizeof(T);
        pointer result = static_cast<pointer>(simple_pool_allocator::allocate(total_size));
        
        if (result == nullptr) {
            throw std::bad_alloc();
        }
        
        return result;
    }

    void deallocate(pointer p, size_type n) {
        if (p != nullptr) {
            simple_pool_allocator::deallocate(p, n * sizeof(T));
        }
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) {
        if (p != nullptr) {
            p->~U();
        }
    }

    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    template<typename U>
    bool operator==(const pool_allocator<U>&) const noexcept {
        return true;
    }

    template<typename U>
    bool operator!=(const pool_allocator<U>&) const noexcept {
        return false;
    }
};

} // namespace mystl

// 测试用的类
class TestClass {
private:
    int value_;
    std::string name_;

public:
    TestClass(int v, const std::string& n) : value_(v), name_(n) {
        std::cout << "构造 TestClass(" << v << ", \"" << n << "\")" << std::endl;
    }
    
    TestClass(const TestClass& other) : value_(other.value_), name_(other.name_) {
        std::cout << "拷贝构造 TestClass(" << value_ << ", \"" << name_ << "\")" << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass(" << value_ << ", \"" << name_ << "\")" << std::endl;
    }
    
    int getValue() const { return value_; }
    std::string getName() const { return name_; }
    
    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value_ = other.value_;
            name_ = other.name_;
        }
        return *this;
    }
};

// 测试函数
void test_standard_allocator() {
    std::cout << "=== 标准分配器测试 ===" << std::endl;
    
    mystl::simple_allocator<int> alloc;
    
    // 分配内存
    int* p = alloc.allocate(5);
    std::cout << "分配了 5 个 int 的内存" << std::endl;
    
    // 构造对象
    for (int i = 0; i < 5; ++i) {
        alloc.construct(p + i, i * 10);
    }
    
    // 使用对象
    std::cout << "对象值: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
    
    // 析构对象
    for (int i = 0; i < 5; ++i) {
        alloc.destroy(p + i);
    }
    
    // 释放内存
    alloc.deallocate(p, 5);
    std::cout << "释放了内存" << std::endl;
}

void test_pool_allocator() {
    std::cout << "\n=== 内存池分配器测试 ===" << std::endl;
    
    mystl::pool_allocator<TestClass> pool_alloc;
    
    std::cout << "内存池使用前: " << mystl::simple_pool_allocator::get_used_size() 
              << " / " << mystl::simple_pool_allocator::get_total_size() << " 字节" << std::endl;
    
    // 分配内存
    TestClass* p = pool_alloc.allocate(3);
    std::cout << "分配了 3 个 TestClass 的内存" << std::endl;
    
    std::cout << "内存池使用后: " << mystl::simple_pool_allocator::get_used_size() 
              << " / " << mystl::simple_pool_allocator::get_total_size() << " 字节" << std::endl;
    
    // 构造对象
    pool_alloc.construct(p, 100, "Pool1");
    pool_alloc.construct(p + 1, 200, "Pool2");
    pool_alloc.construct(p + 2, 300, "Pool3");
    
    // 使用对象
    for (int i = 0; i < 3; ++i) {
        std::cout << "p[" << i << "] = (" << p[i].getValue() 
                  << ", \"" << p[i].getName() << "\")" << std::endl;
    }
    
    // 析构对象
    for (int i = 0; i < 3; ++i) {
        pool_alloc.destroy(p + i);
    }
    
    // 释放内存
    pool_alloc.deallocate(p, 3);
    std::cout << "释放了内存" << std::endl;
}

void test_allocator_comparison() {
    std::cout << "\n=== 分配器比较测试 ===" << std::endl;
    
    mystl::simple_allocator<int> std_alloc;
    mystl::pool_allocator<int> pool_alloc;
    
    // 测试分配器比较
    std::cout << "标准分配器比较: " << (std_alloc == std_alloc ? "相等" : "不等") << std::endl;
    std::cout << "内存池分配器比较: " << (pool_alloc == pool_alloc ? "相等" : "不等") << std::endl;
    
    // 测试最大分配大小
    std::cout << "标准分配器最大大小: " << std_alloc.max_size() << std::endl;
    std::cout << "内存池分配器最大大小: " << pool_alloc.max_size() << std::endl;
}

void test_allocator_with_containers() {
    std::cout << "\n=== 分配器与容器测试 ===" << std::endl;
    
    // 使用标准分配器
    std::vector<int, mystl::simple_allocator<int>> vec1;
    for (int i = 0; i < 5; ++i) {
        vec1.push_back(i * 10);
    }
    
    std::cout << "标准分配器容器: ";
    for (int val : vec1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 使用内存池分配器
    std::vector<TestClass, mystl::pool_allocator<TestClass>> vec2;
    vec2.emplace_back(1, "A");
    vec2.emplace_back(2, "B");
    vec2.emplace_back(3, "C");
    
    std::cout << "内存池分配器容器: ";
    for (const auto& obj : vec2) {
        std::cout << "(" << obj.getValue() << ", \"" << obj.getName() << "\") ";
    }
    std::cout << std::endl;
}

void test_memory_pool_performance() {
    std::cout << "\n=== 内存池性能测试 ===" << std::endl;
    
    const int NUM_ALLOCATIONS = 1000;
    const int ALLOCATION_SIZE = 100;
    
    // 重置内存池
    mystl::simple_pool_allocator::reset();
    
    std::cout << "开始性能测试..." << std::endl;
    
    // 测试内存池分配
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<void*> pointers;
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        void* p = mystl::simple_pool_allocator::allocate(ALLOCATION_SIZE);
        pointers.push_back(p);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "分配 " << NUM_ALLOCATIONS << " 个 " << ALLOCATION_SIZE 
              << " 字节的内存块耗时: " << duration.count() << " 微秒" << std::endl;
    
    std::cout << "内存池使用: " << mystl::simple_pool_allocator::get_used_size() 
              << " / " << mystl::simple_pool_allocator::get_total_size() << " 字节" << std::endl;
    
    // 清理
    for (void* p : pointers) {
        mystl::simple_pool_allocator::deallocate(p, ALLOCATION_SIZE);
    }
}

void test_exception_safety() {
    std::cout << "\n=== 异常安全测试 ===" << std::endl;
    
    mystl::simple_allocator<TestClass> alloc;
    
    try {
        // 尝试分配大量内存
        TestClass* p = alloc.allocate(1000000);
        std::cout << "大内存分配成功" << std::endl;
        alloc.deallocate(p, 1000000);
    } catch (const std::bad_alloc& e) {
        std::cout << "大内存分配失败: " << e.what() << std::endl;
    }
    
    try {
        // 测试内存池异常
        mystl::simple_pool_allocator::reset();
        // 分配超过内存池大小的内存
        void* p = mystl::simple_pool_allocator::allocate(mystl::simple_pool_allocator::get_total_size() + 1);
        std::cout << "内存池超限分配成功" << std::endl;
    } catch (const std::bad_alloc& e) {
        std::cout << "内存池超限分配失败: " << e.what() << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== MyTinySTL 空间配置器系统测试 ===" << std::endl;
    
    test_standard_allocator();
    test_pool_allocator();
    test_allocator_comparison();
    test_allocator_with_containers();
    test_memory_pool_performance();
    test_exception_safety();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}
