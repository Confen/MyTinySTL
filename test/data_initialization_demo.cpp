#include <iostream>
#include <memory>
#include <new>
#include <windows.h>

// 演示类：展示如何初始化data的值
class DataInitializationDemo {
private:
    int* data_;
    int size_;
    std::string name_;
    
public:
    // 方法1：默认初始化（不推荐）
    DataInitializationDemo() : size_(0), name_("default") {
        data_ = new int[10];  // 分配内存
        // data_ 的值是未初始化的随机值！
        std::cout << "默认初始化 - data_ 的值（未初始化）：" << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // 方法2：零初始化
    DataInitializationDemo(int size, const std::string& n) : size_(size), name_(n) {
        data_ = new int[size];
        // 初始化所有值为0
        for (int i = 0; i < size; ++i) {
            data_[i] = 0;
        }
        std::cout << "零初始化 - data_ 的值：" << std::endl;
        for (int i = 0; i < size; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // 方法3：指定值初始化
    DataInitializationDemo(int size, int value, const std::string& n) : size_(size), name_(n) {
        data_ = new int[size];
        // 初始化所有值为指定值
        for (int i = 0; i < size; ++i) {
            data_[i] = value;
        }
        std::cout << "指定值初始化 - data_ 的值（" << value << "）：" << std::endl;
        for (int i = 0; i < size; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // 方法4：序列初始化
    DataInitializationDemo(int size, int start, int step, const std::string& n) : size_(size), name_(n) {
        data_ = new int[size];
        // 初始化序列值
        for (int i = 0; i < size; ++i) {
            data_[i] = start + i * step;
        }
        std::cout << "序列初始化 - data_ 的值（从" << start << "开始，步长" << step << "）：" << std::endl;
        for (int i = 0; i < size; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // 方法5：使用memset初始化
    DataInitializationDemo(int size, const std::string& n, bool useMemset) : size_(size), name_(n) {
        data_ = new int[size];
        if (useMemset) {
            // 使用memset将所有字节设为0
            memset(data_, 0, size * sizeof(int));
            std::cout << "memset初始化 - data_ 的值：" << std::endl;
        } else {
            // 使用memset将所有字节设为0xFF
            memset(data_, 0xFF, size * sizeof(int));
            std::cout << "memset初始化（0xFF）- data_ 的值：" << std::endl;
        }
        for (int i = 0; i < size; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // 析构函数
    ~DataInitializationDemo() {
        if (data_) {
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    int* getData() const { return data_; }
    int getSize() const { return size_; }
    const std::string& getName() const { return name_; }
    
    void printData() const {
        std::cout << name_ << " 的数据：";
        for (int i = 0; i < size_; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
};

// 演示1：不同的初始化方法
void demo_different_initialization_methods() {
    std::cout << "=== 演示1：不同的初始化方法 ===" << std::endl;
    
    std::cout << "1. 默认初始化（不推荐）：" << std::endl;
    DataInitializationDemo obj1;
    obj1.printData();
    
    std::cout << "\n2. 零初始化：" << std::endl;
    DataInitializationDemo obj2(5, "zero");
    obj2.printData();
    
    std::cout << "\n3. 指定值初始化：" << std::endl;
    DataInitializationDemo obj3(5, 42, "value");
    obj3.printData();
    
    std::cout << "\n4. 序列初始化：" << std::endl;
    DataInitializationDemo obj4(5, 10, 2, "sequence");
    obj4.printData();
    
    std::cout << "\n5. memset初始化：" << std::endl;
    DataInitializationDemo obj5(5, "memset", true);
    obj5.printData();
}

// 演示2：初始化列表的使用
void demo_initialization_list_usage() {
    std::cout << "\n=== 演示2：初始化列表的使用 ===" << std::endl;
    
    std::cout << "使用初始化列表初始化data的值：" << std::endl;
    std::cout << "class MyClass {" << std::endl;
    std::cout << "    int* data_;" << std::endl;
    std::cout << "    int size_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    MyClass(int size) : size_(size) {" << std::endl;
    std::cout << "        data_ = new int[size];" << std::endl;
    std::cout << "        // 初始化所有值为0" << std::endl;
    std::cout << "        for (int i = 0; i < size; ++i) {" << std::endl;
    std::cout << "            data_[i] = 0;" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
}

// 演示3：智能指针的初始化
void demo_smart_pointer_initialization() {
    std::cout << "\n=== 演示3：智能指针的初始化 ===" << std::endl;
    
    std::cout << "使用智能指针初始化data的值：" << std::endl;
    std::cout << "class SmartClass {" << std::endl;
    std::cout << "    std::unique_ptr<int[]> data_;" << std::endl;
    std::cout << "    int size_;" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    SmartClass(int size) : size_(size), data_(std::make_unique<int[]>(size)) {" << std::endl;
    std::cout << "        // 初始化所有值为0" << std::endl;
    std::cout << "        for (int i = 0; i < size; ++i) {" << std::endl;
    std::cout << "            data_[i] = 0;" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
}

// 演示4：实际应用示例
void demo_practical_examples() {
    std::cout << "\n=== 演示4：实际应用示例 ===" << std::endl;
    
    std::cout << "实际应用中的初始化方法：" << std::endl;
    
    std::cout << "\n1. 数组初始化：" << std::endl;
    std::cout << "   int* arr = new int[10];" << std::endl;
    std::cout << "   for (int i = 0; i < 10; ++i) {" << std::endl;
    std::cout << "       arr[i] = i * 2;  // 初始化序列值" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n2. 零初始化：" << std::endl;
    std::cout << "   int* arr = new int[10];" << std::endl;
    std::cout << "   memset(arr, 0, 10 * sizeof(int));  // 所有值设为0" << std::endl;
    
    std::cout << "\n3. 指定值初始化：" << std::endl;
    std::cout << "   int* arr = new int[10];" << std::endl;
    std::cout << "   for (int i = 0; i < 10; ++i) {" << std::endl;
    std::cout << "       arr[i] = 42;  // 所有值设为42" << std::endl;
    std::cout << "   }" << std::endl;
    
    std::cout << "\n4. 随机值初始化：" << std::endl;
    std::cout << "   int* arr = new int[10];" << std::endl;
    std::cout << "   srand(time(nullptr));" << std::endl;
    std::cout << "   for (int i = 0; i < 10; ++i) {" << std::endl;
    std::cout << "       arr[i] = rand() % 100;  // 随机值" << std::endl;
    std::cout << "   }" << std::endl;
}

// 演示5：最佳实践
void demo_best_practices() {
    std::cout << "\n=== 演示5：最佳实践 ===" << std::endl;
    
    std::cout << "初始化data值的最佳实践：" << std::endl;
    
    std::cout << "\n1. 总是初始化：" << std::endl;
    std::cout << "   - 不要使用未初始化的内存" << std::endl;
    std::cout << "   - 确保所有值都有意义" << std::endl;
    
    std::cout << "\n2. 选择合适的初始化方法：" << std::endl;
    std::cout << "   - 零初始化：memset 或 循环" << std::endl;
    std::cout << "   - 序列初始化：循环" << std::endl;
    std::cout << "   - 指定值初始化：循环" << std::endl;
    
    std::cout << "\n3. 使用智能指针：" << std::endl;
    std::cout << "   - std::unique_ptr<int[]> 自动管理内存" << std::endl;
    std::cout << "   - 减少手动内存管理错误" << std::endl;
    
    std::cout << "\n4. 异常安全：" << std::endl;
    std::cout << "   - 确保初始化过程中的异常安全" << std::endl;
    std::cout << "   - 使用RAII原则" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demo_different_initialization_methods();
    demo_initialization_list_usage();
    demo_smart_pointer_initialization();
    demo_practical_examples();
    demo_best_practices();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "如何初始化data的值：" << std::endl;
    std::cout << "1. 零初始化：for循环或memset" << std::endl;
    std::cout << "2. 指定值初始化：for循环" << std::endl;
    std::cout << "3. 序列初始化：for循环" << std::endl;
    std::cout << "4. 随机值初始化：for循环 + rand()" << std::endl;
    std::cout << "5. 使用智能指针：std::make_unique<int[]>" << std::endl;
    std::cout << "6. 总是初始化，不要使用未初始化的内存" << std::endl;
    
    return 0;
}















