#include <iostream>
#include <windows.h>

// 错误的宏定义方式1：没有包装
#define BAD_MACRO1(x) \
    std::cout << "执行操作1: " << x << std::endl; \
    std::cout << "执行操作2: " << x << std::endl

// 错误的宏定义方式2：用花括号包装
#define BAD_MACRO2(x) \
    { \
        std::cout << "执行操作1: " << x << std::endl; \
        std::cout << "执行操作2: " << x << std::endl; \
    }

// 正确的宏定义方式：使用 do-while(0)
#define GOOD_MACRO(x) \
    do { \
        std::cout << "执行操作1: " << x << std::endl; \
        std::cout << "执行操作2: " << x << std::endl; \
    } while(0)

// 模拟异常安全宏
#define SIMPLE_EXCEPTION_ASSERT(expr) \
    do { \
        try { \
            expr; \
        } catch (...) { \
            std::cout << "捕获到异常，进行处理" << std::endl; \
        } \
    } while(0)

void test_function() {
    throw std::runtime_error("测试异常");
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== do-while(0) 宏技巧详解 ===" << std::endl;
    
    // 测试1：展示不同宏定义的问题
    std::cout << "\n1. 测试不同宏定义方式的问题：" << std::endl;
    
    // 使用 BAD_MACRO1 - 没有包装
    std::cout << "\n1a. 使用 BAD_MACRO1（没有包装）：" << std::endl;
    if (true)
        BAD_MACRO1("测试");
    // 这会展开为：
    // if (true)
    //     std::cout << "执行操作1: " << "测试" << std::endl;
    //     std::cout << "执行操作2: " << "测试" << std::endl;  // 这行不在 if 内！
    
    std::cout << "问题：第二个语句不在 if 条件内，总是会执行" << std::endl;
    
    // 使用 BAD_MACRO2 - 花括号包装
    std::cout << "\n1b. 使用 BAD_MACRO2（花括号包装）：" << std::endl;
    if (true)
        BAD_MACRO2("测试");
    // 这会展开为：
    // if (true)
    //     {
    //         std::cout << "执行操作1: " << "测试" << std::endl;
    //         std::cout << "执行操作2: " << "测试" << std::endl;
    //     };  // 注意这里有分号！
    
    std::cout << "这个看起来正常，但在某些情况下会有问题..." << std::endl;
    
    // 演示 BAD_MACRO2 的问题
    std::cout << "\n1c. BAD_MACRO2 的问题场景：" << std::endl;
    std::cout << "（注释掉了有问题的代码，因为会导致编译错误）" << std::endl;
    // if (true)
    //     BAD_MACRO2("测试");
    // else
    //     std::cout << "else 分支" << std::endl;
    // 这会展开为：
    // if (true)
    //     { ... };  // 注意分号
    // else          // 编译错误！因为上面的分号结束了 if 语句
    //     std::cout << "else 分支" << std::endl;
    
    std::cout << "注意：上面的代码实际上有语法问题，但编译器可能会处理" << std::endl;
    
    // 使用 GOOD_MACRO - do-while(0) 包装
    std::cout << "\n1d. 使用 GOOD_MACRO（do-while(0) 包装）：" << std::endl;
    if (true)
        GOOD_MACRO("测试");
    else
        std::cout << "else 分支不会执行" << std::endl;
    // 这会展开为：
    // if (true)
    //     do { ... } while(0);  // 这是一个完整的语句
    // else
    //     std::cout << "else 分支不会执行" << std::endl;
    
    std::cout << "完美！语法正确，行为符合预期" << std::endl;
    
    // 测试2：展示 do-while(0) 的优势
    std::cout << "\n2. do-while(0) 的优势：" << std::endl;
    
    std::cout << "\n2a. 可以安全地在任何地方使用：" << std::endl;
    
    // 在 if-else 中使用
    bool condition = true;
    if (condition)
        GOOD_MACRO("if分支");
    else
        GOOD_MACRO("else分支");
    
    // 在循环中使用
    for (int i = 0; i < 2; ++i)
        GOOD_MACRO(i);
    
    // 在 switch 中使用
    int value = 1;
    switch (value) {
        case 1:
            GOOD_MACRO("case 1");
            break;
        case 2:
            GOOD_MACRO("case 2");
            break;
    }
    
    std::cout << "\n2b. 创建局部作用域：" << std::endl;
    {
        int x = 10;
        GOOD_MACRO(x);  // 宏内部可以声明局部变量，不会与外部冲突
    }
    
    // 测试3：异常安全宏的应用
    std::cout << "\n3. 异常安全宏的应用：" << std::endl;
    
    std::cout << "使用 SIMPLE_EXCEPTION_ASSERT：" << std::endl;
    SIMPLE_EXCEPTION_ASSERT(test_function());
    std::cout << "宏执行完毕，程序继续" << std::endl;
    
    // 测试4：展示 do-while(0) 的执行流程
    std::cout << "\n4. do-while(0) 的执行流程：" << std::endl;
    std::cout << "do-while(0) 循环只执行一次，然后退出" << std::endl;
    
    int count = 0;
    do {
        std::cout << "循环体执行，count = " << count << std::endl;
        count++;
        if (count > 5) {
            std::cout << "理论上可以 break，但我们不需要" << std::endl;
            break;
        }
    } while(0);  // 条件是 false，所以只执行一次
    
    std::cout << "循环结束，count = " << count << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "do-while(0) 在宏中的作用：" << std::endl;
    std::cout << "1. 创建一个语句块，可以包含多个语句" << std::endl;
    std::cout << "2. 整个结构被视为一个单独的语句" << std::endl;
    std::cout << "3. 可以安全地在任何需要单个语句的地方使用" << std::endl;
    std::cout << "4. 避免了花括号 + 分号导致的语法问题" << std::endl;
    std::cout << "5. 创建局部作用域，避免变量名冲突" << std::endl;
    std::cout << "6. 支持 break 语句（虽然在异常宏中不常用）" << std::endl;
    
    return 0;
}
