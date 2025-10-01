#include <iostream>
#include <vector>
#include <windows.h>

// 会抛出异常的类
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

// 简化的 uninitialized_fill 实现
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    ForwardIterator current = first;
    
    std::cout << "开始 uninitialized_fill..." << std::endl;
    
    try {
        for (; current != last; ++current) {
            std::cout << "  构造对象 " << (current - first + 1) << std::endl;
            new(&*current) T(value);  // 可能抛出异常
        }
        std::cout << "uninitialized_fill 成功完成" << std::endl;
    } catch (...) {
        std::cout << "构造过程中发生异常，开始清理..." << std::endl;
        // 异常安全：析构已构造的对象
        for (ForwardIterator it = first; it != current; ++it) {
            std::cout << "  清理对象 " << (it - first + 1) << std::endl;
            it->~T();
        }
        std::cout << "清理完成，重新抛出异常" << std::endl;
        throw;  // 重新抛出异常
    }
}

void demonstrate_exception_safety() {
    std::cout << "=== 异常安全演示 ===" << std::endl;
    
    // 分配原始内存
    char buffer[sizeof(ThrowingClass) * 5];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    try {
        // 尝试构造5个对象
        uninitialized_fill(objects, objects + 5, ThrowingClass(42));
        std::cout << "所有对象构造成功！" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "当前对象数量: " << ThrowingClass::getCount() << std::endl;
    }
    
    std::cout << "演示结束，最终对象数量: " << ThrowingClass::getCount() << std::endl;
}

void demonstrate_no_rollback() {
    std::cout << "\n=== 不进行回滚的后果演示 ===" << std::endl;
    
    char buffer[sizeof(ThrowingClass) * 5];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    try {
        // 不进行回滚的版本
        for (int i = 0; i < 5; ++i) {
            std::cout << "构造对象 " << (i + 1) << std::endl;
            new(&objects[i]) ThrowingClass(42);
        }
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        std::cout << "当前对象数量: " << ThrowingClass::getCount() << std::endl;
        std::cout << "问题：已构造的对象没有被清理！" << std::endl;
    }
    
    std::cout << "演示结束，最终对象数量: " << ThrowingClass::getCount() << std::endl;
    
    // 手动清理
    for (int i = 0; i < ThrowingClass::getCount(); ++i) {
        objects[i].~ThrowingClass();
    }
}

void demonstrate_why_rethrow() {
    std::cout << "\n=== 为什么需要重新抛出异常 ===" << std::endl;
    
    std::cout << "1. 调用者需要知道操作失败了" << std::endl;
    std::cout << "2. 调用者可能需要采取其他措施" << std::endl;
    std::cout << "3. 强异常安全保证：要么完全成功，要么完全失败" << std::endl;
    std::cout << "4. 如果不重新抛出，调用者会认为操作成功了" << std::endl;
    
    char buffer[sizeof(ThrowingClass) * 3];
    ThrowingClass* objects = reinterpret_cast<ThrowingClass*>(buffer);
    
    ThrowingClass::resetCount();
    
    // 模拟不重新抛出的版本
    bool success = true;
    try {
        uninitialized_fill(objects, objects + 3, ThrowingClass(42));
    } catch (...) {
        success = false;
        // 不重新抛出，调用者不知道失败了
    }
    
    if (success) {
        std::cout << "调用者认为操作成功了，但实际上失败了！" << std::endl;
    } else {
        std::cout << "调用者知道操作失败了，可以采取相应措施" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demonstrate_exception_safety();
    demonstrate_no_rollback();
    demonstrate_why_rethrow();
    
    return 0;
}