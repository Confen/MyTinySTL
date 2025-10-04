#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <windows.h>

// 无状态对象示例
class MathUtils {
public:
    // 纯函数 - 无状态
    static int add(int a, int b) {
        return a + b;
    }
    
    static double calculate_area(double radius) {
        return 3.14159 * radius * radius;
    }
    
    static bool is_prime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }
};

// 有状态对象示例
class BankAccount {
private:
    double balance;           // 状态1：余额
    std::string account_id;  // 状态2：账户ID
    bool is_locked;          // 状态3：是否锁定
    std::vector<std::string> transaction_history; // 状态4：交易历史
    
public:
    BankAccount(const std::string& id, double initial_balance) 
        : balance(initial_balance), account_id(id), is_locked(false) {
        std::cout << "创建银行账户: " << id << ", 初始余额: " << initial_balance << std::endl;
    }
    
    // 状态查询方法
    double get_balance() const { return balance; }
    std::string get_account_id() const { return account_id; }
    bool is_account_locked() const { return is_locked; }
    std::vector<std::string> get_transaction_history() const { return transaction_history; }
    
    // 状态修改方法
    void deposit(double amount) {
        if (is_locked) {
            throw std::runtime_error("账户已锁定，无法存款");
        }
        balance += amount;
        transaction_history.push_back("存款: +" + std::to_string(amount));
        std::cout << "存款 " << amount << ", 新余额: " << balance << std::endl;
    }
    
    void withdraw(double amount) {
        if (is_locked) {
            throw std::runtime_error("账户已锁定，无法取款");
        }
        if (balance < amount) {
            throw std::runtime_error("余额不足");
        }
        balance -= amount;
        transaction_history.push_back("取款: -" + std::to_string(amount));
        std::cout << "取款 " << amount << ", 新余额: " << balance << std::endl;
    }
    
    void lock_account() {
        is_locked = true;
        std::cout << "账户已锁定" << std::endl;
    }
    
    void unlock_account() {
        is_locked = false;
        std::cout << "账户已解锁" << std::endl;
    }
    
    void print_state() const {
        std::cout << "=== 账户状态 ===" << std::endl;
        std::cout << "账户ID: " << account_id << std::endl;
        std::cout << "余额: " << balance << std::endl;
        std::cout << "锁定状态: " << (is_locked ? "已锁定" : "未锁定") << std::endl;
        std::cout << "交易历史: " << transaction_history.size() << " 条记录" << std::endl;
    }
};

// 部分有状态对象示例
class Counter {
private:
    int count;  // 只有一个状态
    
public:
    Counter() : count(0) {}
    
    int get_count() const { return count; }
    
    void increment() {
        count++;
        std::cout << "计数增加到: " << count << std::endl;
    }
    
    void reset() {
        count = 0;
        std::cout << "计数器重置为: " << count << std::endl;
    }
};

// 复杂有状态对象示例
class FileManager {
private:
    std::vector<std::string> open_files;     // 状态1：打开的文件列表
    std::vector<std::string> file_cache;     // 状态2：文件缓存
    bool cache_enabled;                      // 状态3：缓存是否启用
    size_t max_cache_size;                   // 状态4：最大缓存大小
    std::string current_directory;           // 状态5：当前目录
    
public:
    FileManager() : cache_enabled(true), max_cache_size(100), current_directory("/") {
        std::cout << "文件管理器初始化" << std::endl;
    }
    
    // 状态查询
    std::vector<std::string> get_open_files() const { return open_files; }
    std::vector<std::string> get_file_cache() const { return file_cache; }
    bool is_cache_enabled() const { return cache_enabled; }
    size_t get_max_cache_size() const { return max_cache_size; }
    std::string get_current_directory() const { return current_directory; }
    
    // 状态修改
    void open_file(const std::string& filename) {
        open_files.push_back(filename);
        std::cout << "打开文件: " << filename << std::endl;
    }
    
    void close_file(const std::string& filename) {
        auto it = std::find(open_files.begin(), open_files.end(), filename);
        if (it != open_files.end()) {
            open_files.erase(it);
            std::cout << "关闭文件: " << filename << std::endl;
        }
    }
    
    void change_directory(const std::string& new_dir) {
        current_directory = new_dir;
        std::cout << "切换目录到: " << new_dir << std::endl;
    }
    
    void print_state() const {
        std::cout << "=== 文件管理器状态 ===" << std::endl;
        std::cout << "打开的文件: " << open_files.size() << " 个" << std::endl;
        std::cout << "缓存文件: " << file_cache.size() << " 个" << std::endl;
        std::cout << "缓存启用: " << (cache_enabled ? "是" : "否") << std::endl;
        std::cout << "当前目录: " << current_directory << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    std::cout << "=== 什么是有状态对象？ ===" << std::endl;
    
    std::cout << "\n📋 状态的定义：" << std::endl;
    std::cout << "状态 = 对象内部存储的数据，这些数据会影响对象的行为" << std::endl;
    std::cout << "状态 = 对象在内存中的当前情况" << std::endl;
    std::cout << "状态 = 对象记住的信息" << std::endl;
    
    // 示例1：无状态对象
    std::cout << "\n🔹 示例1：无状态对象（MathUtils）" << std::endl;
    std::cout << "特点：没有成员变量，每次调用都是独立的" << std::endl;
    
    int result1 = MathUtils::add(5, 3);
    int result2 = MathUtils::add(10, 20);
    std::cout << "5 + 3 = " << result1 << std::endl;
    std::cout << "10 + 20 = " << result2 << std::endl;
    std::cout << "✅ 无状态：每次调用结果只依赖于输入参数" << std::endl;
    
    // 示例2：有状态对象
    std::cout << "\n🔹 示例2：有状态对象（BankAccount）" << std::endl;
    std::cout << "特点：有成员变量，操作会改变内部状态" << std::endl;
    
    BankAccount account("12345", 1000.0);
    account.print_state();
    
    std::cout << "\n执行操作：" << std::endl;
    account.deposit(500.0);
    account.withdraw(200.0);
    account.print_state();
    
    std::cout << "✅ 有状态：操作改变了余额、交易历史等内部数据" << std::endl;
    
    // 示例3：状态的影响
    std::cout << "\n🔹 示例3：状态如何影响行为" << std::endl;
    
    std::cout << "正常状态下的操作：" << std::endl;
    try {
        account.deposit(100.0);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n锁定状态下的操作：" << std::endl;
    account.lock_account();
    try {
        account.deposit(100.0);
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    std::cout << "✅ 状态影响行为：锁定状态阻止了存款操作" << std::endl;
    
    // 示例4：简单有状态对象
    std::cout << "\n🔹 示例4：简单有状态对象（Counter）" << std::endl;
    
    Counter counter;
    std::cout << "初始计数: " << counter.get_count() << std::endl;
    
    counter.increment();
    counter.increment();
    std::cout << "当前计数: " << counter.get_count() << std::endl;
    
    counter.reset();
    std::cout << "重置后计数: " << counter.get_count() << std::endl;
    
    std::cout << "✅ 简单状态：只有一个状态变量" << std::endl;
    
    // 示例5：复杂有状态对象
    std::cout << "\n🔹 示例5：复杂有状态对象（FileManager）" << std::endl;
    
    FileManager file_mgr;
    file_mgr.print_state();
    
    std::cout << "\n执行操作：" << std::endl;
    file_mgr.open_file("test1.txt");
    file_mgr.open_file("test2.txt");
    file_mgr.change_directory("/home/user");
    file_mgr.print_state();
    
    std::cout << "✅ 复杂状态：多个相互关联的状态变量" << std::endl;
    
    // 状态的特征
    std::cout << "\n📊 状态的特征：" << std::endl;
    std::cout << "1. 持久性：状态在操作之间保持" << std::endl;
    std::cout << "2. 可变性：状态可以被修改" << std::endl;
    std::cout << "3. 影响性：状态影响对象的行为" << std::endl;
    std::cout << "4. 可查询性：状态可以被外部查询" << std::endl;
    
    // 状态 vs 无状态对比
    std::cout << "\n🔄 状态 vs 无状态对比：" << std::endl;
    
    std::cout << "\n无状态对象：" << std::endl;
    std::cout << "✅ 线程安全" << std::endl;
    std::cout << "✅ 易于测试" << std::endl;
    std::cout << "✅ 无副作用" << std::endl;
    std::cout << "❌ 无法记住信息" << std::endl;
    std::cout << "❌ 功能有限" << std::endl;
    
    std::cout << "\n有状态对象：" << std::endl;
    std::cout << "✅ 功能丰富" << std::endl;
    std::cout << "✅ 可以记住信息" << std::endl;
    std::cout << "✅ 支持复杂行为" << std::endl;
    std::cout << "❌ 需要考虑线程安全" << std::endl;
    std::cout << "❌ 测试更复杂" << std::endl;
    std::cout << "❌ 异常安全更重要" << std::endl;
    
    // 异常安全的重要性
    std::cout << "\n⚠️ 为什么有状态对象的异常安全更重要？" << std::endl;
    std::cout << "1. 状态可能被部分修改" << std::endl;
    std::cout << "2. 需要保证状态的一致性" << std::endl;
    std::cout << "3. 异常可能导致资源泄漏" << std::endl;
    std::cout << "4. 状态损坏可能导致程序崩溃" << std::endl;
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "🎯 有状态对象 = 有成员变量的对象" << std::endl;
    std::cout << "🎯 状态 = 对象内部存储的数据" << std::endl;
    std::cout << "🎯 状态影响 = 对象的行为依赖于内部数据" << std::endl;
    std::cout << "🎯 异常安全 = 保证状态在异常时的一致性" << std::endl;
    
    return 0;
}
