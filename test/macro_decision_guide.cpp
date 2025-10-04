#include "../exceptdef.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <windows.h>

// 示例：简单类 - 不需要状态检查
class SimpleCalculator {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    int divide(int a, int b) {
        if (b == 0) {
            throw std::runtime_error("除零错误");
        }
        return a / b;
    }
};

// 示例：复杂类 - 需要状态检查
class DatabaseConnection {
private:
    bool connected;
    std::vector<std::string> queries;
    std::string last_error;
    
public:
    DatabaseConnection() : connected(false) {}
    
    bool is_valid() const {
        return connected && last_error.empty();
    }
    
    std::vector<std::string> get_state() const {
        return queries;
    }
    
    void connect() {
        connected = true;
        queries.clear();
        last_error.clear();
    }
    
    void disconnect() {
        connected = false;
        queries.clear();
        last_error = "连接已断开";
    }
    
    void execute_query(const std::string& query) {
        if (!connected) {
            throw std::runtime_error("数据库未连接");
        }
        
        // 模拟查询失败
        if (query.find("DROP") != std::string::npos) {
            throw std::runtime_error("危险操作被阻止");
        }
        
        queries.push_back(query);
    }
    
    void print_state() const {
        std::cout << "连接状态: " << (connected ? "已连接" : "未连接") << std::endl;
        std::cout << "查询数量: " << queries.size() << std::endl;
        std::cout << "错误信息: " << (last_error.empty() ? "无" : last_error) << std::endl;
    }
};

// 示例：资源管理类 - 需要状态检查
class FileManager {
private:
    std::vector<std::string> open_files;
    bool resource_leak_detected;
    
public:
    FileManager() : resource_leak_detected(false) {}
    
    bool is_valid() const {
        return !resource_leak_detected;
    }
    
    std::vector<std::string> get_state() const {
        return open_files;
    }
    
    void open_file(const std::string& filename) {
        if (filename.empty()) {
            throw std::runtime_error("文件名不能为空");
        }
        
        // 模拟资源泄漏检测
        if (open_files.size() > 10) {
            resource_leak_detected = true;
            throw std::runtime_error("检测到资源泄漏");
        }
        
        open_files.push_back(filename);
    }
    
    void close_file(const std::string& filename) {
        auto it = std::find(open_files.begin(), open_files.end(), filename);
        if (it != open_files.end()) {
            open_files.erase(it);
        }
    }
    
    void print_state() const {
        std::cout << "打开的文件: " << open_files.size() << " 个" << std::endl;
        std::cout << "资源泄漏: " << (resource_leak_detected ? "是" : "否") << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 何时使用改进版宏的决策指南 ===" << std::endl;
    
    std::cout << "\n📋 决策原则：" << std::endl;
    std::cout << "1. 对象是否有状态？" << std::endl;
    std::cout << "2. 异常安全是否重要？" << std::endl;
    std::cout << "3. 是否需要详细验证？" << std::endl;
    std::cout << "4. 是否是关键业务逻辑？" << std::endl;
    
    // 场景1：简单计算 - 使用原始宏
    std::cout << "\n🔹 场景1：简单计算（使用原始宏）" << std::endl;
    std::cout << "特点：无状态，纯函数，异常安全不重要" << std::endl;
    
    SimpleCalculator calc;
    std::cout << "计算 10 + 5 = " << calc.add(10, 5) << std::endl;
    
    std::cout << "测试除零异常：" << std::endl;
    MYSTL_BASIC_GUARANTEE_ASSERT(calc.divide(10, 0));
    std::cout << "✅ 使用原始宏 - 简单有效" << std::endl;
    
    // 场景2：数据库连接 - 使用改进宏
    std::cout << "\n🔹 场景2：数据库连接（使用改进宏）" << std::endl;
    std::cout << "特点：有状态，异常安全重要，需要验证" << std::endl;
    
    DatabaseConnection db;
    db.connect();
    std::cout << "连接后状态：";
    db.print_state();
    
    std::cout << "测试危险查询：" << std::endl;
    MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(db.execute_query("DROP TABLE users"), db);
    std::cout << "异常后状态：";
    db.print_state();
    std::cout << "✅ 使用改进宏 - 验证了异常安全" << std::endl;
    
    // 场景3：资源管理 - 使用改进宏
    std::cout << "\n🔹 场景3：资源管理（使用改进宏）" << std::endl;
    std::cout << "特点：资源管理，异常安全关键，需要状态验证" << std::endl;
    
    FileManager file_mgr;
    std::cout << "初始状态：";
    file_mgr.print_state();
    
    // 正常操作
    file_mgr.open_file("test1.txt");
    file_mgr.open_file("test2.txt");
    std::cout << "添加文件后：";
    file_mgr.print_state();
    
    // 测试资源泄漏
    std::cout << "测试资源泄漏：" << std::endl;
    MYSTL_STRONG_GUARANTEE_ASSERT_WITH_OBJ(file_mgr.open_file(""), file_mgr);
    std::cout << "异常后状态：";
    file_mgr.print_state();
    std::cout << "✅ 使用改进宏 - 验证了资源状态" << std::endl;
    
    // 决策树
    std::cout << "\n🌳 决策树：" << std::endl;
    std::cout << "┌─ 对象有状态吗？" << std::endl;
    std::cout << "│  ├─ 否 → 使用原始宏" << std::endl;
    std::cout << "│  └─ 是 → 继续判断" << std::endl;
    std::cout << "│     ┌─ 异常安全重要吗？" << std::endl;
    std::cout << "│     │  ├─ 否 → 使用原始宏" << std::endl;
    std::cout << "│     │  └─ 是 → 继续判断" << std::endl;
    std::cout << "│     │     ┌─ 需要详细验证吗？" << std::endl;
    std::cout << "│     │     │  ├─ 否 → 使用原始宏" << std::endl;
    std::cout << "│     │     │  └─ 是 → 使用改进宏" << std::endl;
    
    // 具体判断标准
    std::cout << "\n📊 具体判断标准：" << std::endl;
    
    std::cout << "\n✅ 使用原始宏的情况：" << std::endl;
    std::cout << "1. 纯函数（无副作用）" << std::endl;
    std::cout << "2. 简单计算" << std::endl;
    std::cout << "3. 工具函数" << std::endl;
    std::cout << "4. 快速原型" << std::endl;
    std::cout << "5. 异常安全不重要的场景" << std::endl;
    
    std::cout << "\n✅ 使用改进宏的情况：" << std::endl;
    std::cout << "1. 有状态的对象" << std::endl;
    std::cout << "2. 资源管理（文件、内存、网络）" << std::endl;
    std::cout << "3. 数据库操作" << std::endl;
    std::cout << "4. 事务性操作" << std::endl;
    std::cout << "5. 生产环境测试" << std::endl;
    std::cout << "6. 关键业务逻辑" << std::endl;
    std::cout << "7. 需要异常安全验证的场景" << std::endl;
    
    // 代码示例对比
    std::cout << "\n💻 代码示例对比：" << std::endl;
    
    std::cout << "\n原始宏使用：" << std::endl;
    std::cout << "```cpp" << std::endl;
    std::cout << "// 简单场景" << std::endl;
    std::cout << "MYSTL_BASIC_GUARANTEE_ASSERT(calc.divide(a, b));" << std::endl;
    std::cout << "MYSTL_STRONG_GUARANTEE_ASSERT(file.close());" << std::endl;
    std::cout << "```" << std::endl;
    
    std::cout << "\n改进宏使用：" << std::endl;
    std::cout << "```cpp" << std::endl;
    std::cout << "// 复杂场景" << std::endl;
    std::cout << "MYSTL_BASIC_GUARANTEE_ASSERT_WITH_OBJ(db.execute(sql), db);" << std::endl;
    std::cout << "MYSTL_STRONG_GUARANTEE_ASSERT_WITH_OBJ(tx.commit(), tx);" << std::endl;
    std::cout << "```" << std::endl;
    
    // 性能考虑
    std::cout << "\n⚡ 性能考虑：" << std::endl;
    std::cout << "原始宏：" << std::endl;
    std::cout << "  ✅ 编译时开销小" << std::endl;
    std::cout << "  ✅ 运行时开销小" << std::endl;
    std::cout << "  ✅ 代码简洁" << std::endl;
    
    std::cout << "改进宏：" << std::endl;
    std::cout << "  ⚠️  需要调用 is_valid() 和 get_state()" << std::endl;
    std::cout << "  ⚠️  需要状态比较" << std::endl;
    std::cout << "  ✅ 提供详细验证信息" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 选择原则：" << std::endl;
    std::cout << "• 简单场景 → 原始宏" << std::endl;
    std::cout << "• 复杂场景 → 改进宏" << std::endl;
    std::cout << "• 不确定时 → 从原始宏开始，需要时再升级" << std::endl;
    std::cout << "• 关键业务 → 优先使用改进宏" << std::endl;
    
    return 0;
}
