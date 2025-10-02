#include <iostream>
#include <windows.h>

// 基类
class Vehicle {
public:
    virtual void start() {
        std::cout << "Vehicle 启动" << std::endl;
    }
    
    virtual void stop() {
        std::cout << "Vehicle 停止" << std::endl;
    }
    
    virtual ~Vehicle() {
        std::cout << "Vehicle 析构" << std::endl;
    }
};

// 派生类1
class Car : public Vehicle {
public:
    virtual void start() override {
        std::cout << "Car 启动：点火，挂挡，踩油门" << std::endl;
    }
    
    virtual void stop() override {
        std::cout << "Car 停止：踩刹车，挂空挡" << std::endl;
    }
    
    virtual ~Car() {
        std::cout << "Car 析构" << std::endl;
    }
};

// 派生类2
class Bicycle : public Vehicle {
public:
    virtual void start() override {
        std::cout << "Bicycle 启动：踩踏板" << std::endl;
    }
    
    virtual void stop() override {
        std::cout << "Bicycle 停止：停止踩踏板，刹车" << std::endl;
    }
    
    virtual ~Bicycle() {
        std::cout << "Bicycle 析构" << std::endl;
    }
};

// 派生类3
class Airplane : public Vehicle {
public:
    virtual void start() override {
        std::cout << "Airplane 启动：启动引擎，滑行，起飞" << std::endl;
    }
    
    virtual void stop() override {
        std::cout << "Airplane 停止：降落，滑行，关闭引擎" << std::endl;
    }
    
    virtual ~Airplane() {
        std::cout << "Airplane 析构" << std::endl;
    }
};

void demonstrate_pointer_precision() {
    std::cout << "=== 基类指针精确定位派生类演示 ===" << std::endl;
    
    // 1. 基类指针指向不同的派生类对象
    std::cout << "1. 基类指针指向不同派生类对象:" << std::endl;
    
    Vehicle* vehicles[3];
    vehicles[0] = new Car();
    vehicles[1] = new Bicycle();
    vehicles[2] = new Airplane();
    
    // 2. 通过基类指针调用虚函数
    std::cout << "\n2. 通过基类指针调用虚函数:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "   车辆 " << (i+1) << ":" << std::endl;
        vehicles[i]->start();  // 精确定位到派生类的实现
        vehicles[i]->stop();   // 精确定位到派生类的实现
        std::cout << std::endl;
    }
    
    // 3. 清理资源
    std::cout << "3. 清理资源:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        delete vehicles[i];  // 虚析构函数确保正确调用派生类析构函数
    }
}

void demonstrate_runtime_polymorphism() {
    std::cout << "\n=== 运行时多态性演示 ===" << std::endl;
    
    // 1. 运行时决定对象类型
    std::cout << "1. 运行时决定对象类型:" << std::endl;
    
    Vehicle* vehicle = nullptr;
    
    // 模拟用户选择
    int choice = 2;  // 假设用户选择了飞机
    
    switch (choice) {
        case 1:
            vehicle = new Car();
            break;
        case 2:
            vehicle = new Bicycle();
            break;
        case 3:
            vehicle = new Airplane();
            break;
    }
    
    // 2. 统一接口调用
    std::cout << "2. 统一接口调用:" << std::endl;
    if (vehicle) {
        vehicle->start();  // 根据实际对象类型调用相应函数
        vehicle->stop();   // 根据实际对象类型调用相应函数
        delete vehicle;
    }
}

void demonstrate_virtual_table_concept() {
    std::cout << "\n=== 虚函数表概念演示 ===" << std::endl;
    
    // 1. 对象大小
    std::cout << "1. 对象大小:" << std::endl;
    std::cout << "   Vehicle 对象大小: " << sizeof(Vehicle) << " 字节" << std::endl;
    std::cout << "   Car 对象大小: " << sizeof(Car) << " 字节" << std::endl;
    std::cout << "   Bicycle 对象大小: " << sizeof(Bicycle) << " 字节" << std::endl;
    std::cout << "   Airplane 对象大小: " << sizeof(Airplane) << " 字节" << std::endl;
    
    // 2. 虚函数表指针
    std::cout << "\n2. 虚函数表指针:" << std::endl;
    std::cout << "   每个对象都包含一个指向虚函数表的指针" << std::endl;
    std::cout << "   这个指针指向该类的虚函数表" << std::endl;
    
    // 3. 虚函数表内容
    std::cout << "\n3. 虚函数表内容:" << std::endl;
    std::cout << "   Car 的虚函数表包含:" << std::endl;
    std::cout << "     - Car::start() 的地址" << std::endl;
    std::cout << "     - Car::stop() 的地址" << std::endl;
    std::cout << "     - Car::~Car() 的地址" << std::endl;
}

void demonstrate_practical_benefits() {
    std::cout << "\n=== 实际应用优势 ===" << std::endl;
    
    // 1. 代码复用
    std::cout << "1. 代码复用:" << std::endl;
    std::cout << "   可以编写通用的代码来处理不同类型的对象" << std::endl;
    
    // 2. 扩展性
    std::cout << "\n2. 扩展性:" << std::endl;
    std::cout << "   可以轻松添加新的派生类，无需修改现有代码" << std::endl;
    
    // 3. 维护性
    std::cout << "\n3. 维护性:" << std::endl;
    std::cout << "   每个派生类可以独立实现自己的逻辑" << std::endl;
    
    // 4. 多态性
    std::cout << "\n4. 多态性:" << std::endl;
    std::cout << "   同一个接口可以有不同的实现" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 虚函数精确定位演示 ===" << std::endl;
    
    demonstrate_pointer_precision();
    demonstrate_runtime_polymorphism();
    demonstrate_virtual_table_concept();
    demonstrate_practical_benefits();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
    return 0;
}











