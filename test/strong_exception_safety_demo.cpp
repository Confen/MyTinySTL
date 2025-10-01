#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <windows.h>

// 模拟会抛出异常的类
class ExpensiveResource {
private:
    int id_;
    std::string name_;
    static int construction_count_;

public:
    ExpensiveResource(int id, const std::string& name) : id_(id), name_(name) {
        ++construction_count_;
        std::cout << "创建资源 " << id_ << ": " << name_ << " (总数: " << construction_count_ << ")" << std::endl;
        
        // 模拟创建失败
        if (id_ == 3) {
            std::cout << "  资源 " << id_ << " 创建失败！" << std::endl;
            throw std::runtime_error("资源创建失败");
        }
    }
    
    ~ExpensiveResource() {
        --construction_count_;
        std::cout << "销毁资源 " << id_ << ": " << name_ << " (剩余: " << construction_count_ << ")" << std::endl;
    }
    
    int getId() const { return id_; }
    std::string getName() const { return name_; }
    
    static int getCount() { return construction_count_; }
    static void resetCount() { construction_count_ = 0; }
};

int ExpensiveResource::construction_count_ = 0;

// 场景1：容器操作 - 需要强异常安全
class ResourceContainer {
private:
    std::vector<std::unique_ptr<ExpensiveResource>> resources_;

public:
    // 强异常安全版本
    void addResourcesStrongSafety(const std::vector<std::pair<int, std::string>>& new_resources) {
        std::cout << "\n=== 强异常安全版本 ===" << std::endl;
        
        // 先创建临时容器
        std::vector<std::unique_ptr<ExpensiveResource>> temp_resources;
        
        try {
            // 尝试创建所有资源
            for (const auto& resource_info : new_resources) {
                temp_resources.push_back(
                    std::unique_ptr<ExpensiveResource>(new ExpensiveResource(resource_info.first, resource_info.second))
                );
            }
            
            // 如果都成功，才添加到主容器
            for (auto& resource : temp_resources) {
                resources_.push_back(std::move(resource));
            }
            
            std::cout << "所有资源添加成功！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "添加失败: " << e.what() << std::endl;
            std::cout << "容器保持原状，没有资源泄漏" << std::endl;
            // temp_resources 会自动清理
        }
    }
    
    // 弱异常安全版本（有问题）
    void addResourcesWeakSafety(const std::vector<std::pair<int, std::string>>& new_resources) {
        std::cout << "\n=== 弱异常安全版本（有问题）===" << std::endl;
        
        try {
            // 直接添加到主容器
            for (const auto& resource_info : new_resources) {
                resources_.push_back(
                    std::unique_ptr<ExpensiveResource>(new ExpensiveResource(resource_info.first, resource_info.second))
                );
            }
            std::cout << "所有资源添加成功！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "添加失败: " << e.what() << std::endl;
            std::cout << "问题：部分资源已经被添加！" << std::endl;
        }
    }
    
    void printResources() const {
        std::cout << "容器中的资源: ";
        for (const auto& resource : resources_) {
            std::cout << resource->getId() << " ";
        }
        std::cout << std::endl;
    }
    
    void clear() {
        resources_.clear();
    }
};

// 场景2：配置更新 - 需要强异常安全
class SystemConfig {
private:
    std::vector<std::pair<std::string, std::string>> config_;

public:
    void updateConfigStrongSafety(const std::vector<std::pair<std::string, std::string>>& new_config) {
        std::cout << "\n=== 配置更新（强异常安全）===" << std::endl;
        
        // 保存原配置
        auto old_config = config_;
        
        try {
            // 清空并重新设置
            config_.clear();
            for (const auto& item : new_config) {
                config_.push_back(item);
            }
            std::cout << "配置更新成功！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "配置更新失败: " << e.what() << std::endl;
            // 恢复原配置
            config_ = old_config;
            std::cout << "配置已恢复原状" << std::endl;
        }
    }
    
    void printConfig() const {
        std::cout << "当前配置: ";
        for (const auto& item : config_) {
            std::cout << item.first << "=" << item.second << " ";
        }
        std::cout << std::endl;
    }
};

// 场景3：事务操作 - 需要强异常安全
class DatabaseTransaction {
private:
    std::vector<std::string> commands_;

public:
    void addCommand(const std::string& cmd) {
        commands_.push_back(cmd);
    }
    
    void executeStrongSafety() {
        std::cout << "\n=== 事务执行（强异常安全）===" << std::endl;
        
        std::cout << "开始事务，命令数量: " << commands_.size() << std::endl;
        
        try {
            for (size_t i = 0; i < commands_.size(); ++i) {
                std::cout << "执行命令 " << (i + 1) << ": " << commands_[i] << std::endl;
                
                // 模拟命令执行失败
                if (commands_[i] == "DELETE FROM users") {
                    throw std::runtime_error("DELETE 命令失败");
                }
            }
            std::cout << "事务提交成功！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "事务执行失败: " << e.what() << std::endl;
            std::cout << "事务回滚，所有命令都被撤销" << std::endl;
        }
    }
};

void demonstrate_strong_exception_safety() {
    std::cout << "=== 强异常安全保证演示 ===" << std::endl;
    
    ExpensiveResource::resetCount();
    
    // 场景1：容器操作
    ResourceContainer container;
    
    // 测试强异常安全版本
    std::vector<std::pair<int, std::string>> resources1 = {
        {1, "Resource1"},
        {2, "Resource2"},
        {3, "Resource3"},  // 这个会失败
        {4, "Resource4"}
    };
    
    container.addResourcesStrongSafety(resources1);
    container.printResources();
    std::cout << "当前资源总数: " << ExpensiveResource::getCount() << std::endl;
    
    // 清理
    container.clear();
    ExpensiveResource::resetCount();
    
    // 测试弱异常安全版本
    container.addResourcesWeakSafety(resources1);
    container.printResources();
    std::cout << "当前资源总数: " << ExpensiveResource::getCount() << std::endl;
    
    // 场景2：配置更新
    SystemConfig config;
    config.updateConfigStrongSafety({{"key1", "value1"}, {"key2", "value2"}});
    config.printConfig();
    
    // 场景3：事务操作
    DatabaseTransaction transaction;
    transaction.addCommand("INSERT INTO users VALUES (1, 'Alice')");
    transaction.addCommand("INSERT INTO users VALUES (2, 'Bob')");
    transaction.addCommand("DELETE FROM users");  // 这个会失败
    transaction.addCommand("UPDATE users SET name = 'Charlie'");
    
    transaction.executeStrongSafety();
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demonstrate_strong_exception_safety();
    
    return 0;
}
