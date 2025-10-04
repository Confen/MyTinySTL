#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

// 测试类 - 用于观察析构函数调用
class TestClass {
private:
    int id;
    std::string name;
    
public:
    TestClass(int i, const std::string& n) : id(i), name(n) {
        std::cout << "构造 TestClass[" << id << "] - " << name << std::endl;
    }
    
    ~TestClass() {
        std::cout << "析构 TestClass[" << id << "] - " << name << std::endl;
    }
    
    void print() const {
        std::cout << "TestClass[" << id << "] - " << name << std::endl;
    }
};

// 测试类 - 用于观察数组析构
class ArrayTestClass {
private:
    int* data;
    size_t size;
    
public:
    ArrayTestClass(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = i;
        }
        std::cout << "构造 ArrayTestClass，大小: " << size << std::endl;
    }
    
    ~ArrayTestClass() {
        std::cout << "析构 ArrayTestClass，释放内存" << std::endl;
        delete[] data;  // 这里会调用每个元素的析构函数（如果有的话）
    }
    
    void print() const {
        std::cout << "ArrayTestClass 数据: [";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

// 测试类 - 用于观察对象数组的析构
class ObjectArrayTest {
private:
    TestClass** objects;  // 指向 TestClass 指针的数组
    size_t count;
    
public:
    ObjectArrayTest(size_t c) : count(c) {
        objects = new TestClass*[count];
        for (size_t i = 0; i < count; ++i) {
            objects[i] = new TestClass(i, "对象" + std::to_string(i));
        }
        std::cout << "构造 ObjectArrayTest，对象数量: " << count << std::endl;
    }
    
    ~ObjectArrayTest() {
        std::cout << "析构 ObjectArrayTest" << std::endl;
        // 先删除每个对象
        for (size_t i = 0; i < count; ++i) {
            delete objects[i];  // 这会调用 TestClass 的析构函数
        }
        // 再删除指针数组
        delete[] objects;  // 这只删除指针数组，不调用 TestClass 析构函数
    }
    
    void print() const {
        std::cout << "ObjectArrayTest 包含 " << count << " 个对象:" << std::endl;
        for (size_t i = 0; i < count; ++i) {
            objects[i]->print();
        }
    }
};

// 测试类 - 用于观察对象数组（直接存储对象）
class DirectObjectArrayTest {
private:
    TestClass* objects;  // 直接存储 TestClass 对象的数组
    size_t count;
    
public:
    DirectObjectArrayTest(size_t c) : count(c) {
        objects = new TestClass[count];  // 分配对象数组
        // 注意：这里不能直接初始化，需要逐个构造
        for (size_t i = 0; i < count; ++i) {
            new (objects + i) TestClass(i, "直接对象" + std::to_string(i));
        }
        std::cout << "构造 DirectObjectArrayTest，对象数量: " << count << std::endl;
    }
    
    ~DirectObjectArrayTest() {
        std::cout << "析构 DirectObjectArrayTest" << std::endl;
        // 先调用每个对象的析构函数
        for (size_t i = 0; i < count; ++i) {
            objects[i].~TestClass();  // 显式调用析构函数
        }
        // 再删除对象数组
        delete[] objects;  // 这会自动调用每个对象的析构函数
    }
    
    void print() const {
        std::cout << "DirectObjectArrayTest 包含 " << count << " 个对象:" << std::endl;
        for (size_t i = 0; i < count; ++i) {
            objects[i].print();
        }
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== delete[] 与析构函数的关系 ===" << std::endl;
    
    std::cout << "\n📋 核心问题：" << std::endl;
    std::cout << "delete[] data; 会自动调用析构函数吗？" << std::endl;
    std::cout << "答案：取决于 data 指向的是什么！" << std::endl;
    
    // 测试1：基本类型数组
    std::cout << "\n🔹 测试1：基本类型数组" << std::endl;
    std::cout << "int* arr = new int[5];" << std::endl;
    std::cout << "delete[] arr;  // 不会调用析构函数（int 没有析构函数）" << std::endl;
    
    {
        int* arr = new int[5];
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }
        std::cout << "数组内容: [";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i];
            if (i < 4) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        delete[] arr;  // 只释放内存，不调用析构函数
        std::cout << "delete[] arr 完成（无析构函数调用）" << std::endl;
    }
    
    // 测试2：对象数组（直接存储对象）
    std::cout << "\n🔹 测试2：对象数组（直接存储对象）" << std::endl;
    std::cout << "TestClass* arr = new TestClass[3];" << std::endl;
    std::cout << "delete[] arr;  // 会自动调用每个对象的析构函数" << std::endl;
    
    {
        TestClass* arr = new TestClass[3];
        // 注意：这里会调用默认构造函数
        for (int i = 0; i < 3; ++i) {
            new (arr + i) TestClass(i, "数组对象" + std::to_string(i));
        }
        
        std::cout << "数组对象:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            arr[i].print();
        }
        
        delete[] arr;  // 会自动调用每个对象的析构函数
        std::cout << "delete[] arr 完成（已调用析构函数）" << std::endl;
    }
    
    // 测试3：指针数组
    std::cout << "\n🔹 测试3：指针数组" << std::endl;
    std::cout << "TestClass** arr = new TestClass*[3];" << std::endl;
    std::cout << "delete[] arr;  // 只删除指针数组，不调用对象析构函数" << std::endl;
    
    {
        TestClass** arr = new TestClass*[3];
        for (int i = 0; i < 3; ++i) {
            arr[i] = new TestClass(i, "指针对象" + std::to_string(i));
        }
        
        std::cout << "指针数组对象:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            arr[i]->print();
        }
        
        // 先删除每个对象
        for (int i = 0; i < 3; ++i) {
            delete arr[i];  // 调用 TestClass 析构函数
        }
        
        delete[] arr;  // 只删除指针数组，不调用 TestClass 析构函数
        std::cout << "delete[] arr 完成（只删除指针数组）" << std::endl;
    }
    
    // 测试4：复杂对象数组
    std::cout << "\n🔹 测试4：复杂对象数组" << std::endl;
    
    {
        ArrayTestClass obj(5);
        obj.print();
        std::cout << "obj 即将析构..." << std::endl;
    }  // 这里会调用 ArrayTestClass 的析构函数
    
    // 测试5：对象数组测试
    std::cout << "\n🔹 测试5：对象数组测试" << std::endl;
    
    {
        ObjectArrayTest obj(3);
        obj.print();
        std::cout << "obj 即将析构..." << std::endl;
    }  // 这里会调用 ObjectArrayTest 的析构函数
    
    // 测试6：直接对象数组测试
    std::cout << "\n🔹 测试6：直接对象数组测试" << std::endl;
    
    {
        DirectObjectArrayTest obj(3);
        obj.print();
        std::cout << "obj 即将析构..." << std::endl;
    }  // 这里会调用 DirectObjectArrayTest 的析构函数
    
    // 总结
    std::cout << "\n📊 delete[] 的行为总结：" << std::endl;
    
    std::cout << "\n1. 基本类型数组：" << std::endl;
    std::cout << "   int* arr = new int[5];" << std::endl;
    std::cout << "   delete[] arr;  // 只释放内存，不调用析构函数" << std::endl;
    
    std::cout << "\n2. 对象数组（直接存储）：" << std::endl;
    std::cout << "   MyClass* arr = new MyClass[5];" << std::endl;
    std::cout << "   delete[] arr;  // 自动调用每个对象的析构函数" << std::endl;
    
    std::cout << "\n3. 指针数组：" << std::endl;
    std::cout << "   MyClass** arr = new MyClass*[5];" << std::endl;
    std::cout << "   delete[] arr;  // 只删除指针数组，不调用对象析构函数" << std::endl;
    
    std::cout << "\n4. 智能指针数组：" << std::endl;
    std::cout << "   std::unique_ptr<MyClass>* arr = new std::unique_ptr<MyClass>[5];" << std::endl;
    std::cout << "   delete[] arr;  // 调用 unique_ptr 析构函数，进而调用对象析构函数" << std::endl;
    
    std::cout << "\n⚠️ 重要注意事项：" << std::endl;
    std::cout << "1. delete[] 会自动调用每个元素的析构函数（如果元素有析构函数）" << std::endl;
    std::cout << "2. 对于指针数组，delete[] 只删除指针，不删除指针指向的对象" << std::endl;
    std::cout << "3. 对于对象数组，delete[] 会调用每个对象的析构函数" << std::endl;
    std::cout << "4. 析构函数的调用顺序是逆序的（最后一个元素先析构）" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 delete[] data 的行为：" << std::endl;
    std::cout << "• 如果 data 指向基本类型数组 → 只释放内存" << std::endl;
    std::cout << "• 如果 data 指向对象数组 → 自动调用每个对象的析构函数" << std::endl;
    std::cout << "• 如果 data 指向指针数组 → 只删除指针数组" << std::endl;
    std::cout << "• 析构函数调用是自动的，不需要手动调用" << std::endl;
    
    return 0;
}




