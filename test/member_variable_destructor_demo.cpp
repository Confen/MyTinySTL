#include <iostream>
#include <string>
#include <vector>
#include <memory>
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

// 测试类 - 包含各种类型的成员变量
class ContainerClass {
private:
    // 基本类型成员变量
    int* intArray;
    size_t intArraySize;
    
    // 对象类型成员变量
    TestClass* objectArray;
    size_t objectArraySize;
    
    // 指针数组成员变量
    TestClass** pointerArray;
    size_t pointerArraySize;
    
    // 智能指针成员变量
    std::unique_ptr<TestClass> smartPtr;
    
    // 标准容器成员变量
    std::vector<TestClass> vectorContainer;
    
    // 字符串成员变量
    std::string stringMember;
    
public:
    ContainerClass(size_t intSize, size_t objSize, size_t ptrSize) 
        : intArraySize(intSize), objectArraySize(objSize), pointerArraySize(ptrSize) {
        
        std::cout << "构造 ContainerClass" << std::endl;
        
        // 分配基本类型数组
        intArray = new int[intArraySize];
        for (size_t i = 0; i < intArraySize; ++i) {
            intArray[i] = i * 10;
        }
        
        // 分配对象数组
        objectArray = new TestClass[objectArraySize];
        for (size_t i = 0; i < objectArraySize; ++i) {
            new (objectArray + i) TestClass(i, "对象数组" + std::to_string(i));
        }
        
        // 分配指针数组
        pointerArray = new TestClass*[pointerArraySize];
        for (size_t i = 0; i < pointerArraySize; ++i) {
            pointerArray[i] = new TestClass(i, "指针数组" + std::to_string(i));
        }
        
        // 创建智能指针
        smartPtr = std::make_unique<TestClass>(999, "智能指针对象");
        
        // 填充向量容器
        for (size_t i = 0; i < 3; ++i) {
            vectorContainer.emplace_back(i + 100, "向量对象" + std::to_string(i));
        }
        
        // 设置字符串
        stringMember = "这是一个字符串成员变量";
        
        std::cout << "ContainerClass 构造完成" << std::endl;
    }
    
    ~ContainerClass() {
        std::cout << "析构 ContainerClass" << std::endl;
        
        // 释放基本类型数组
        std::cout << "释放基本类型数组..." << std::endl;
        delete[] intArray;  // 只释放内存，不调用析构函数
        
        // 释放对象数组
        std::cout << "释放对象数组..." << std::endl;
        delete[] objectArray;  // 自动调用每个对象的析构函数
        
        // 释放指针数组
        std::cout << "释放指针数组..." << std::endl;
        for (size_t i = 0; i < pointerArraySize; ++i) {
            delete pointerArray[i];  // 调用 TestClass 析构函数
        }
        delete[] pointerArray;  // 只删除指针数组
        
        // 智能指针会自动释放（不需要手动删除）
        std::cout << "智能指针自动释放..." << std::endl;
        
        // 向量容器会自动释放（不需要手动删除）
        std::cout << "向量容器自动释放..." << std::endl;
        
        // 字符串会自动释放（不需要手动删除）
        std::cout << "字符串自动释放..." << std::endl;
        
        std::cout << "ContainerClass 析构完成" << std::endl;
    }
    
    void print() const {
        std::cout << "ContainerClass 内容:" << std::endl;
        
        std::cout << "基本类型数组: [";
        for (size_t i = 0; i < intArraySize; ++i) {
            std::cout << intArray[i];
            if (i < intArraySize - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        std::cout << "对象数组:" << std::endl;
        for (size_t i = 0; i < objectArraySize; ++i) {
            objectArray[i].print();
        }
        
        std::cout << "指针数组:" << std::endl;
        for (size_t i = 0; i < pointerArraySize; ++i) {
            pointerArray[i]->print();
        }
        
        std::cout << "智能指针: ";
        smartPtr->print();
        
        std::cout << "向量容器:" << std::endl;
        for (const auto& obj : vectorContainer) {
            obj.print();
        }
        
        std::cout << "字符串: " << stringMember << std::endl;
    }
};

// 测试类 - 演示错误的释放方式
class BadContainerClass {
private:
    TestClass* objectArray;
    size_t arraySize;
    
public:
    BadContainerClass(size_t size) : arraySize(size) {
        std::cout << "构造 BadContainerClass" << std::endl;
        
        objectArray = new TestClass[arraySize];
        for (size_t i = 0; i < arraySize; ++i) {
            new (objectArray + i) TestClass(i, "错误示例" + std::to_string(i));
        }
    }
    
    ~BadContainerClass() {
        std::cout << "析构 BadContainerClass" << std::endl;
        
        // 错误：使用 delete 而不是 delete[]
        // delete objectArray;  // 这会导致未定义行为！
        
        // 正确：使用 delete[]
        delete[] objectArray;  // 这会调用每个对象的析构函数
    }
};

// 测试类 - 演示手动析构
class ManualDestructorClass {
private:
    TestClass* objectArray;
    size_t arraySize;
    
public:
    ManualDestructorClass(size_t size) : arraySize(size) {
        std::cout << "构造 ManualDestructorClass" << std::endl;
        
        objectArray = new TestClass[arraySize];
        for (size_t i = 0; i < arraySize; ++i) {
            new (objectArray + i) TestClass(i, "手动析构" + std::to_string(i));
        }
    }
    
    ~ManualDestructorClass() {
        std::cout << "析构 ManualDestructorClass" << std::endl;
        
        // 手动调用每个对象的析构函数
        for (size_t i = 0; i < arraySize; ++i) {
            objectArray[i].~TestClass();
        }
        
        // 然后释放内存
        delete[] objectArray;  // 这不会再次调用析构函数
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 类中成员变量的释放和析构函数 ===" << std::endl;
    
    std::cout << "\n📋 核心问题：" << std::endl;
    std::cout << "类中的成员变量在析构时如何释放？" << std::endl;
    std::cout << "delete[] 会自动调用析构函数吗？" << std::endl;
    
    // 测试1：完整的容器类
    std::cout << "\n🔹 测试1：完整的容器类" << std::endl;
    std::cout << "包含各种类型的成员变量" << std::endl;
    
    {
        ContainerClass container(5, 3, 2);
        container.print();
        std::cout << "container 即将析构..." << std::endl;
    }  // 这里会调用 ContainerClass 的析构函数
    
    // 测试2：错误的释放方式
    std::cout << "\n🔹 测试2：错误的释放方式" << std::endl;
    std::cout << "使用 delete 而不是 delete[] 会导致未定义行为" << std::endl;
    
    {
        BadContainerClass badContainer(3);
        std::cout << "badContainer 即将析构..." << std::endl;
    }  // 这里会调用 BadContainerClass 的析构函数
    
    // 测试3：手动析构
    std::cout << "\n🔹 测试3：手动析构" << std::endl;
    std::cout << "手动调用析构函数，然后使用 delete[]" << std::endl;
    
    {
        ManualDestructorClass manualContainer(3);
        std::cout << "manualContainer 即将析构..." << std::endl;
    }  // 这里会调用 ManualDestructorClass 的析构函数
    
    // 测试4：智能指针成员变量
    std::cout << "\n🔹 测试4：智能指针成员变量" << std::endl;
    std::cout << "智能指针会自动管理内存" << std::endl;
    
    {
        class SmartPtrContainer {
        private:
            std::unique_ptr<TestClass> smartPtr1;
            std::unique_ptr<TestClass> smartPtr2;
            
        public:
            SmartPtrContainer() {
                std::cout << "构造 SmartPtrContainer" << std::endl;
                smartPtr1 = std::make_unique<TestClass>(1, "智能指针1");
                smartPtr2 = std::make_unique<TestClass>(2, "智能指针2");
            }
            
            ~SmartPtrContainer() {
                std::cout << "析构 SmartPtrContainer" << std::endl;
                // 智能指针会自动释放，不需要手动删除
            }
        };
        
        SmartPtrContainer smartContainer;
        std::cout << "smartContainer 即将析构..." << std::endl;
    }  // 这里会调用 SmartPtrContainer 的析构函数
    
    // 测试5：标准容器成员变量
    std::cout << "\n🔹 测试5：标准容器成员变量" << std::endl;
    std::cout << "标准容器会自动管理内存" << std::endl;
    
    {
        class StandardContainer {
        private:
            std::vector<TestClass> vectorMember;
            std::string stringMember;
            
        public:
            StandardContainer() {
                std::cout << "构造 StandardContainer" << std::endl;
                vectorMember.emplace_back(1, "向量成员1");
                vectorMember.emplace_back(2, "向量成员2");
                stringMember = "字符串成员";
            }
            
            ~StandardContainer() {
                std::cout << "析构 StandardContainer" << std::endl;
                // 标准容器会自动释放，不需要手动删除
            }
        };
        
        StandardContainer standardContainer;
        std::cout << "standardContainer 即将析构..." << std::endl;
    }  // 这里会调用 StandardContainer 的析构函数
    
    // 总结
    std::cout << "\n📊 类中成员变量释放总结：" << std::endl;
    
    std::cout << "\n1. 基本类型数组成员变量：" << std::endl;
    std::cout << "   int* data = new int[5];" << std::endl;
    std::cout << "   delete[] data;  // 只释放内存，不调用析构函数" << std::endl;
    
    std::cout << "\n2. 对象数组成员变量：" << std::endl;
    std::cout << "   MyClass* data = new MyClass[5];" << std::endl;
    std::cout << "   delete[] data;  // 自动调用每个对象的析构函数" << std::endl;
    
    std::cout << "\n3. 指针数组成员变量：" << std::endl;
    std::cout << "   MyClass** data = new MyClass*[5];" << std::endl;
    std::cout << "   for (int i = 0; i < 5; ++i) delete data[i];  // 先删除对象" << std::endl;
    std::cout << "   delete[] data;  // 再删除指针数组" << std::endl;
    
    std::cout << "\n4. 智能指针成员变量：" << std::endl;
    std::cout << "   std::unique_ptr<MyClass> data;" << std::endl;
    std::cout << "   // 自动释放，不需要手动删除" << std::endl;
    
    std::cout << "\n5. 标准容器成员变量：" << std::endl;
    std::cout << "   std::vector<MyClass> data;" << std::endl;
    std::cout << "   // 自动释放，不需要手动删除" << std::endl;
    
    std::cout << "\n⚠️ 重要注意事项：" << std::endl;
    std::cout << "1. 析构函数中必须释放所有动态分配的内存" << std::endl;
    std::cout << "2. 使用 delete[] 释放数组，使用 delete 释放单个对象" << std::endl;
    std::cout << "3. 智能指针和标准容器会自动管理内存" << std::endl;
    std::cout << "4. 析构函数的调用顺序是逆序的" << std::endl;
    std::cout << "5. 如果析构函数抛出异常，程序可能崩溃" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 类中成员变量的释放：" << std::endl;
    std::cout << "• 基本类型数组 → delete[] 只释放内存" << std::endl;
    std::cout << "• 对象数组 → delete[] 自动调用析构函数" << std::endl;
    std::cout << "• 指针数组 → 先 delete 对象，再 delete[] 指针" << std::endl;
    std::cout << "• 智能指针 → 自动管理，无需手动删除" << std::endl;
    std::cout << "• 标准容器 → 自动管理，无需手动删除" << std::endl;
    
    return 0;
}




