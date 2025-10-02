#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：跟踪构造过程
class TrackingClass {
private:
    int value_;
    std::vector<int> data_;
    static int count_;
    
public:
    TrackingClass(int v, size_t size) : value_(v) {
        ++count_;
        std::cout << "  步骤1：开始构造 TrackingClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        // 分配内存
        std::cout << "  步骤2：分配 " << size << " 个元素的内存..." << std::endl;
        data_.resize(size);
        
        // 初始化数据
        std::cout << "  步骤3：初始化数据..." << std::endl;
        for (size_t i = 0; i < size; ++i) {
            data_[i] = static_cast<int>(i);
        }
        
        // 模拟构造失败
        if (count_ == 3) {
            std::cout << "  步骤4：检查失败条件，抛出异常！" << std::endl;
            throw std::runtime_error("第3次构造失败");
        }
        
        std::cout << "  步骤4：构造完成" << std::endl;
    }
    
    ~TrackingClass() {
        --count_;
        std::cout << "  析构 TrackingClass(" << value_ << "), 剩余: " << count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int TrackingClass::count_ = 0;

// MyTinySTL 风格的 construct 和 destroy
template<typename T, typename... Args>
void construct(T* ptr, Args&&... args) {
    if (ptr != nullptr) {
        new(ptr) T(std::forward<Args>(args)...);
    }
}

template<typename T>
void destroy(T* ptr) {
    if (ptr != nullptr) {
        ptr->~T();
    }
}

// 演示1：异常中断执行流程
void demo_exception_interrupts_execution() {
    std::cout << "=== 演示1：异常中断执行流程 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(TrackingClass) * capacity];
    TrackingClass* objects = reinterpret_cast<TrackingClass*>(buffer);
    
    TrackingClass::resetCount();
    
    std::cout << "预分配了容量为 " << capacity << " 的容器内存" << std::endl;
    
    size_t current_size = 0;
    
    try {
        std::cout << "开始添加对象..." << std::endl;
        
        // 添加对象
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "\n--- 添加第 " << (i + 1) << " 个对象 ---" << std::endl;
            std::cout << "  准备调用 construct..." << std::endl;
            
            construct(objects + i, static_cast<int>(i + 1), 100);
            
            std::cout << "  construct 调用完成，准备增加 current_size..." << std::endl;
            ++current_size;
            std::cout << "  current_size 增加到: " << current_size << std::endl;
        }
        
        std::cout << "\n所有对象添加成功" << std::endl;  // 这行不会执行
    } catch (const std::exception& e) {
        std::cout << "\n=== 异常被捕获 ===" << std::endl;
        std::cout << "异常信息: " << e.what() << std::endl;
        std::cout << "已添加的对象数量: " << current_size << std::endl;
        std::cout << "注意：++current_size 和后续代码都没有执行！" << std::endl;
    }
    
    // 清理所有对象
    std::cout << "\n--- 清理所有对象 ---" << std::endl;
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    std::cout << "最终对象数量: " << TrackingClass::getCount() << std::endl;
}

// 演示2：异常处理的不同策略
void demo_exception_handling_strategies() {
    std::cout << "\n=== 演示2：异常处理的不同策略 ===" << std::endl;
    
    const size_t capacity = 5;
    char buffer[sizeof(TrackingClass) * capacity];
    TrackingClass* objects = reinterpret_cast<TrackingClass*>(buffer);
    
    TrackingClass::resetCount();
    
    // 策略1：继续尝试（基本异常保证）
    std::cout << "策略1：继续尝试（基本异常保证）" << std::endl;
    size_t current_size = 0;
    
    for (size_t i = 0; i < capacity; ++i) {
        try {
            std::cout << "尝试添加第 " << (i + 1) << " 个对象..." << std::endl;
            construct(objects + i, static_cast<int>(i + 1), 100);
            ++current_size;
            std::cout << "第 " << (i + 1) << " 个对象添加成功" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "第 " << (i + 1) << " 个对象添加失败: " << e.what() << std::endl;
            std::cout << "继续尝试下一个对象..." << std::endl;
        }
    }
    
    std::cout << "策略1结果：成功添加 " << current_size << " 个对象" << std::endl;
    
    // 清理
    for (size_t i = 0; i < current_size; ++i) {
        destroy(objects + i);
    }
    
    // 策略2：全部回滚（强异常保证）
    std::cout << "\n策略2：全部回滚（强异常保证）" << std::endl;
    TrackingClass::resetCount();
    current_size = 0;
    
    try {
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "添加第 " << (i + 1) << " 个对象..." << std::endl;
            construct(objects + i, static_cast<int>(i + 1), 100);
            ++current_size;
        }
        std::cout << "所有对象添加成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "添加失败: " << e.what() << std::endl;
        std::cout << "强异常保证：回滚所有已添加的对象" << std::endl;
        
        // 清理已添加的对象
        for (size_t i = 0; i < current_size; ++i) {
            destroy(objects + i);
        }
        current_size = 0;
    }
    
    std::cout << "策略2结果：最终对象数量 " << current_size << std::endl;
}

// 演示3：异常传播机制
void demo_exception_propagation() {
    std::cout << "\n=== 演示3：异常传播机制 ===" << std::endl;
    
    // 内层函数
    auto inner_function = [](size_t index) {
        std::cout << "  内层函数：尝试构造对象 " << index << std::endl;
        
        char buffer[sizeof(TrackingClass)];
        TrackingClass* obj = reinterpret_cast<TrackingClass*>(buffer);
        
        construct(obj, static_cast<int>(index), 100);
        
        std::cout << "  内层函数：构造成功" << std::endl;
        return obj;
    };
    
    // 外层函数
    auto outer_function = [&](size_t count) {
        std::cout << "外层函数：开始处理 " << count << " 个对象" << std::endl;
        
        for (size_t i = 0; i < count; ++i) {
            std::cout << "外层函数：处理第 " << (i + 1) << " 个对象" << std::endl;
            
            try {
                auto obj = inner_function(i + 1);
                std::cout << "外层函数：第 " << (i + 1) << " 个对象处理成功" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "外层函数：第 " << (i + 1) << " 个对象处理失败: " << e.what() << std::endl;
                std::cout << "外层函数：异常传播，中断处理" << std::endl;
                throw;  // 重新抛出异常
            }
        }
        
        std::cout << "外层函数：所有对象处理完成" << std::endl;
    };
    
    TrackingClass::resetCount();
    
    try {
        outer_function(5);
    } catch (const std::exception& e) {
        std::cout << "最外层：捕获异常 " << e.what() << std::endl;
        std::cout << "最外层：异常处理完成" << std::endl;
    }
    
    std::cout << "最终对象数量: " << TrackingClass::getCount() << std::endl;
}

// 演示4：异常安全的不同级别
void demo_exception_safety_levels() {
    std::cout << "\n=== 演示4：异常安全的不同级别 ===" << std::endl;
    
    // 无异常保证
    std::cout << "无异常保证：" << std::endl;
    std::cout << "异常发生时，对象状态不确定" << std::endl;
    
    // 基本异常保证
    std::cout << "\n基本异常保证：" << std::endl;
    std::cout << "异常发生时，对象处于有效状态" << std::endl;
    std::cout << "但状态可能与操作前不同" << std::endl;
    
    // 强异常保证
    std::cout << "\n强异常保证：" << std::endl;
    std::cout << "异常发生时，对象状态完全不变" << std::endl;
    std::cout << "操作要么完全成功，要么完全失败" << std::endl;
    
    // 无异常保证
    std::cout << "\n无异常保证：" << std::endl;
    std::cout << "操作不会抛出异常" << std::endl;
    std::cout << "但可能无法完成预期操作" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    TrackingClass::resetCount();
    
    demo_exception_interrupts_execution();
    demo_exception_handling_strategies();
    demo_exception_propagation();
    demo_exception_safety_levels();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "关于异常中断执行的问题：" << std::endl;
    std::cout << "1. 异常会立即中断当前执行" << std::endl;
    std::cout << "2. 异常后的代码不会执行" << std::endl;
    std::cout << "3. 异常会传播到最近的 catch 块" << std::endl;
    std::cout << "4. 异常安全机制确保资源被正确清理" << std::endl;
    std::cout << "5. 不同的异常安全级别提供不同的保证" << std::endl;
    
    return 0;
}








