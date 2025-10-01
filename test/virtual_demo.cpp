#include "../exceptdef.h"
#include <iostream>
#include <memory>
#include <windows.h>

// 演示虚函数的基本概念
class Animal {
public:
    virtual void makeSound() {
        std::cout << "动物发出声音" << std::endl;
    }
    
    virtual ~Animal() {
        std::cout << "Animal 析构函数" << std::endl;
    }
};

class Dog : public Animal {
public:
    virtual void makeSound() override {
        std::cout << "汪汪汪" << std::endl;
    }
    
    virtual ~Dog() {
        std::cout << "Dog 析构函数" << std::endl;
    }
};

class Cat : public Animal {
public:
    virtual void makeSound() override {
        std::cout << "喵喵喵" << std::endl;
    }
    
    virtual ~Cat() {
        std::cout << "Cat 析构函数" << std::endl;
    }
};

void demonstrate_virtual_functions() {
    std::cout << "=== 虚函数演示 ===" << std::endl;
    
    // 1. 多态性演示
    std::cout << "1. 多态性演示:" << std::endl;
    Animal* animal1 = new Dog();
    Animal* animal2 = new Cat();
    
    animal1->makeSound();  // 调用 Dog::makeSound()
    animal2->makeSound();  // 调用 Cat::makeSound()
    
    delete animal1;
    delete animal2;
    
    // 2. 虚析构函数的重要性
    std::cout << "\n2. 虚析构函数的重要性:" << std::endl;
    std::cout << "   通过基类指针删除派生类对象时，会正确调用派生类的析构函数" << std::endl;
}

void demonstrate_exception_hierarchy() {
    std::cout << "\n=== 异常类层次结构演示 ===" << std::endl;
    
    // 1. 多态异常处理
    std::cout << "1. 多态异常处理:" << std::endl;
    try {
        throw mystl::mystl_logic_error("逻辑错误");
    } catch (const std::exception& e) {  // 通过基类捕获
        std::cout << "   捕获异常: " << e.what() << std::endl;
        std::cout << "   异常类型: " << typeid(e).name() << std::endl;
    }
    
    // 2. 异常类继承关系
    std::cout << "\n2. 异常类继承关系:" << std::endl;
    mystl::mystl_exception* ex1 = new mystl::mystl_logic_error("测试");
    mystl::mystl_exception* ex2 = new mystl::mystl_runtime_error("测试");
    
    std::cout << "   ex1 类型: " << typeid(*ex1).name() << std::endl;
    std::cout << "   ex2 类型: " << typeid(*ex2).name() << std::endl;
    
    // 通过基类指针调用虚函数
    std::cout << "   ex1 消息: " << ex1->what() << std::endl;
    std::cout << "   ex2 消息: " << ex2->what() << std::endl;
    
    delete ex1;
    delete ex2;
}

void demonstrate_virtual_table() {
    std::cout << "\n=== 虚函数表演示 ===" << std::endl;
    
    // 1. 虚函数表的存在
    std::cout << "1. 虚函数表的存在:" << std::endl;
    std::cout << "   每个包含虚函数的类都有一个虚函数表" << std::endl;
    std::cout << "   对象包含指向虚函数表的指针" << std::endl;
    
    // 2. 虚函数调用开销
    std::cout << "\n2. 虚函数调用开销:" << std::endl;
    std::cout << "   虚函数调用需要通过虚函数表，有轻微的性能开销" << std::endl;
    std::cout << "   但提供了灵活的多态性" << std::endl;
    
    // 3. 虚函数表的大小
    std::cout << "\n3. 虚函数表的大小:" << std::endl;
    std::cout << "   Animal 对象大小: " << sizeof(Animal) << " 字节" << std::endl;
    std::cout << "   Dog 对象大小: " << sizeof(Dog) << " 字节" << std::endl;
    std::cout << "   Cat 对象大小: " << sizeof(Cat) << " 字节" << std::endl;
}

void demonstrate_pure_virtual() {
    std::cout << "\n=== 纯虚函数演示 ===" << std::endl;
    
    // 纯虚函数示例
    class AbstractShape {
    public:
        virtual double area() = 0;  // 纯虚函数
        virtual ~AbstractShape() = default;
    };
    
    class Circle : public AbstractShape {
        double radius;
    public:
        Circle(double r) : radius(r) {}
        virtual double area() override {
            return 3.14159 * radius * radius;
        }
    };
    
    class Rectangle : public AbstractShape {
        double width, height;
    public:
        Rectangle(double w, double h) : width(w), height(h) {}
        virtual double area() override {
            return width * height;
        }
    };
    
    // 使用纯虚函数
    std::cout << "1. 纯虚函数使用:" << std::endl;
    AbstractShape* shape1 = new Circle(5.0);
    AbstractShape* shape2 = new Rectangle(4.0, 6.0);
    
    std::cout << "   圆形面积: " << shape1->area() << std::endl;
    std::cout << "   矩形面积: " << shape2->area() << std::endl;
    
    delete shape1;
    delete shape2;
}

void demonstrate_virtual_vs_non_virtual() {
    std::cout << "\n=== 虚函数 vs 非虚函数 ===" << std::endl;
    
    class Base {
    public:
        void nonVirtualFunc() {
            std::cout << "Base::nonVirtualFunc()" << std::endl;
        }
        
        virtual void virtualFunc() {
            std::cout << "Base::virtualFunc()" << std::endl;
        }
    };
    
    class Derived : public Base {
    public:
        void nonVirtualFunc() {
            std::cout << "Derived::nonVirtualFunc()" << std::endl;
        }
        
        virtual void virtualFunc() override {
            std::cout << "Derived::virtualFunc()" << std::endl;
        }
    };
    
    std::cout << "1. 非虚函数调用:" << std::endl;
    Base* base = new Derived();
    base->nonVirtualFunc();  // 调用 Base::nonVirtualFunc()
    
    std::cout << "\n2. 虚函数调用:" << std::endl;
    base->virtualFunc();     // 调用 Derived::virtualFunc()
    
    delete base;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== virtual 关键字详解 ===" << std::endl;
    
    demonstrate_virtual_functions();
    demonstrate_exception_hierarchy();
    demonstrate_virtual_table();
    demonstrate_pure_virtual();
    demonstrate_virtual_vs_non_virtual();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}










