#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

// 模拟会抛出异常的类
class ThrowingClass {
private:
    int value_;
    static int construction_count_;

public:
    ThrowingClass(int v) : value_(v) {
        ++construction_count_;
        std::cout << "构造 ThrowingClass(" << v << "), 总数: " << construction_count_ << std::endl;
        
        // 在第3个对象构造时抛出异常
        if (construction_count_ == 3) {
            std::cout << "  抛出异常！" << std::endl;
            throw std::runtime_error("构造失败");
        }
    }
    
    ~ThrowingClass() {
        --construction_count_;
        std::cout << "析构 ThrowingClass(" << value_ << "), 剩余: " << construction_count_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
    static int getCount() { return construction_count_; }
    static void resetCount() { construction_count_ = 0; }
};

int ThrowingClass::construction_count_ = 0;

// 模拟 construct 函数
template<typename T>
void construct(T* ptr, const T& value) {
    if (ptr != nullptr) {
        new(ptr) T(value);  // 这里会抛出异常
    }
}

// 模拟 destroy 函数
template<typename T>
void destroy(T* ptr) {
    if (ptr != nullptr) {
        ptr->~T();
    }
}

// 模拟 uninitialized_fill 函数
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    std::cout << "uninitialized_fill 开始执行" << std::endl;
    ForwardIterator current = first;
    
    try {
        for (; current != last; ++current) {
            std::cout << "  尝试构造对象 " << (current - first + 1) << std::endl;
            construct(&*current, value);  // 这里可能抛出异常
            std::cout << "  对象 " << (current - first + 1) << " 构造成功" << std::endl;
        }
        std::cout << "uninitialized_fill 成功完成" << std::endl;
    } catch (...) {
        std::cout << "uninitialized_fill 捕获到异常，开始清理..." << std::endl;
        // 异常安全：析构已构造的对象
        for (ForwardIterator it = first; it != current; ++it) {
            std::cout << "  清理对象 " << (it - first + 1) << std::endl;
            destroy(&*it);
        }
        std::cout << "清理完成，重新抛出异常" << std::endl;
        throw;  // 重新抛出异常！
    }
}

// 演示1：异常传播链
void demonstrate_exception_propagation() {
    std::cout << "=== 异常传播链演示 ===" << std::endl;
    
    // 分配原始内存
    char buffer[sizeof(ThrowingClass) * 5];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    try {
        std::cout << "用户调用 uninitialized_fill" << std::endl;
        uninitialized_fill(objects, objects + 5, ThrowingClass(42));
        std::cout << "用户：操作成功！" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "用户捕获到异常: " << e.what() << std::endl;
        std::cout << "用户：操作失败，但对象状态已清理" << std::endl;
    }
    
    std::cout << "最终对象数量: " << ThrowingClass::getCount() << std::endl;
}

// 演示2：异常传播的详细过程
void demonstrate_detailed_propagation() {
    std::cout << "\n=== 异常传播详细过程 ===" << std::endl;
    
    char buffer[sizeof(ThrowingClass) * 3];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    std::cout << "步骤1：用户调用 uninitialized_fill" << std::endl;
    
    try {
        std::cout << "步骤2：uninitialized_fill 开始执行" << std::endl;
        uninitialized_fill(objects, objects + 3, ThrowingClass(42));
        std::cout << "步骤3：uninitialized_fill 成功完成" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "步骤4：用户捕获到 runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "步骤4：用户捕获到 exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "步骤4：用户捕获到未知异常" << std::endl;
    }
    
    std::cout << "步骤5：异常处理完成" << std::endl;
}

// 演示3：不同异常类型的传播
void demonstrate_different_exceptions() {
    std::cout << "\n=== 不同异常类型的传播 ===" << std::endl;
    
    // 模拟不同类型的异常
    class DifferentException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "不同类型的异常";
        }
    };
    
    // 使用全局计数器
    static int different_count = 0;
    
    class DifferentThrowingClass {
        private:
            int value_;

        public:
            DifferentThrowingClass(int v) : value_(v) {
                ++different_count;
                std::cout << "构造 DifferentThrowingClass(" << v << "), 总数: " << different_count << std::endl;
                
                if (different_count == 2) {
                    std::cout << "  抛出不同类型的异常！" << std::endl;
                    throw DifferentException();
                }
            }
            
            ~DifferentThrowingClass() {
                --different_count;
                std::cout << "析构 DifferentThrowingClass(" << value_ << "), 剩余: " << different_count << std::endl;
            }
        };
    
    char buffer[sizeof(DifferentThrowingClass) * 3];
    DifferentThrowingClass* objects = reinterpret_cast<DifferentThrowingClass*>(buffer);
    
    different_count = 0;
    
    try {
        std::cout << "尝试构造不同类型的异常对象" << std::endl;
        uninitialized_fill(objects, objects + 3, DifferentThrowingClass(42));
    } catch (const DifferentException& e) {
        std::cout << "捕获到 DifferentException: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获到 std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "捕获到未知异常" << std::endl;
    }
}

// 演示4：异常传播的栈展开
void demonstrate_stack_unwinding() {
    std::cout << "\n=== 异常传播的栈展开 ===" << std::endl;
    
    class StackUnwindingDemo {
    public:
        StackUnwindingDemo(const std::string& name) : name_(name) {
            std::cout << "构造 " << name_ << std::endl;
        }
        
        ~StackUnwindingDemo() {
            std::cout << "析构 " << name_ << std::endl;
        }
        
        void throwException() {
            std::cout << name_ << " 抛出异常" << std::endl;
            throw std::runtime_error("栈展开异常");
        }
        
    private:
        std::string name_;
    };
    
    try {
        std::cout << "进入 try 块" << std::endl;
        StackUnwindingDemo demo1("Demo1");
        StackUnwindingDemo demo2("Demo2");
        
        std::cout << "调用可能抛出异常的函数" << std::endl;
        demo1.throwException();
        
        std::cout << "这行不会执行" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
        std::cout << "注意：Demo1 和 Demo2 都被析构了（栈展开）" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demonstrate_exception_propagation();
    demonstrate_detailed_propagation();
    demonstrate_different_exceptions();
    demonstrate_stack_unwinding();
    
    return 0;
}
