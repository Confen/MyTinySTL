#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <windows.h>

// 模拟会抛出异常的类
class DataProcessor {
private:
    std::vector<int> data_;
    std::string name_;
    bool is_processed_;

public:
    DataProcessor(const std::string& name) : name_(name), is_processed_(false) {
        std::cout << "创建 DataProcessor: " << name_ << std::endl;
    }
    
    ~DataProcessor() {
        std::cout << "销毁 DataProcessor: " << name_ << std::endl;
    }
    
    // 基本异常安全保证 - 一般异常
    void processData(const std::vector<int>& new_data) {
        std::cout << "开始处理数据: " << name_ << std::endl;
        
        try {
            // 清空旧数据
            data_.clear();
            is_processed_ = false;
            
            // 处理新数据
            for (size_t i = 0; i < new_data.size(); ++i) {
                std::cout << "  处理数据 " << (i + 1) << ": " << new_data[i] << std::endl;
                
                // 模拟处理失败
                if (new_data[i] == 999) {
                    throw std::runtime_error("数据 999 处理失败");
                }
                
                data_.push_back(new_data[i] * 2);  // 简单的数据处理
            }
            
            is_processed_ = true;
            std::cout << "数据处理完成！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "处理失败: " << e.what() << std::endl;
            std::cout << "对象状态: 已清空，但处于有效状态" << std::endl;
            // 注意：这里不恢复原始数据，只是保证对象有效
        }
    }
    
    // 强异常安全保证版本
    void processDataStrongSafety(const std::vector<int>& new_data) {
        std::cout << "开始处理数据（强异常安全）: " << name_ << std::endl;
        
        // 保存原始状态
        auto old_data = data_;
        bool old_processed = is_processed_;
        
        try {
            // 清空旧数据
            data_.clear();
            is_processed_ = false;
            
            // 处理新数据
            for (size_t i = 0; i < new_data.size(); ++i) {
                std::cout << "  处理数据 " << (i + 1) << ": " << new_data[i] << std::endl;
                
                if (new_data[i] == 999) {
                    throw std::runtime_error("数据 999 处理失败");
                }
                
                data_.push_back(new_data[i] * 2);
            }
            
            is_processed_ = true;
            std::cout << "数据处理完成！" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "处理失败: " << e.what() << std::endl;
            // 恢复原始状态
            data_ = old_data;
            is_processed_ = old_processed;
            std::cout << "对象状态已恢复到原始状态" << std::endl;
        }
    }
    
    void printStatus() const {
        std::cout << "DataProcessor " << name_ << ": ";
        std::cout << "数据数量=" << data_.size() << ", ";
        std::cout << "已处理=" << (is_processed_ ? "是" : "否") << std::endl;
        
        if (!data_.empty()) {
            std::cout << "  数据: ";
            for (int val : data_) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 场景1：日志记录 - 基本异常安全就够了
class Logger {
private:
    std::vector<std::string> logs_;
    std::string filename_;

public:
    Logger(const std::string& filename) : filename_(filename) {
        std::cout << "创建 Logger: " << filename_ << std::endl;
    }
    
    // 基本异常安全 - 日志记录失败不影响程序
    void log(const std::string& message) {
        try {
            logs_.push_back(message);
            std::cout << "记录日志: " << message << std::endl;
            
            // 模拟写入文件失败
            if (message.find("ERROR") != std::string::npos) {
                throw std::runtime_error("文件写入失败");
            }
        } catch (const std::exception& e) {
            std::cout << "日志记录失败: " << e.what() << std::endl;
            std::cout << "但程序继续运行，不影响核心功能" << std::endl;
            // 基本异常安全：对象仍然有效，只是日志可能丢失
        }
    }
    
    void printLogs() const {
        std::cout << "日志数量: " << logs_.size() << std::endl;
        for (const auto& log : logs_) {
            std::cout << "  " << log << std::endl;
        }
    }
};

// 场景2：缓存更新 - 基本异常安全
class Cache {
private:
    std::vector<std::pair<std::string, int>> cache_;
    size_t max_size_;

public:
    Cache(size_t max_size) : max_size_(max_size) {
        std::cout << "创建 Cache，最大大小: " << max_size_ << std::endl;
    }
    
    // 基本异常安全 - 缓存更新失败不影响程序
    void updateCache(const std::string& key, int value) {
        try {
            // 查找并更新
            for (auto& item : cache_) {
                if (item.first == key) {
                    item.second = value;
                    std::cout << "更新缓存: " << key << " = " << value << std::endl;
                    return;
                }
            }
            
            // 添加新项
            if (cache_.size() >= max_size_) {
                throw std::runtime_error("缓存已满");
            }
            
            cache_.push_back({key, value});
            std::cout << "添加缓存: " << key << " = " << value << std::endl;
        } catch (const std::exception& e) {
            std::cout << "缓存更新失败: " << e.what() << std::endl;
            std::cout << "但程序继续运行，缓存状态有效" << std::endl;
        }
    }
    
    void printCache() const {
        std::cout << "缓存内容: ";
        for (const auto& item : cache_) {
            std::cout << item.first << "=" << item.second << " ";
        }
        std::cout << std::endl;
    }
};

// 场景3：统计信息 - 基本异常安全
class Statistics {
private:
    int count_;
    double sum_;
    std::string name_;

public:
    Statistics(const std::string& name) : count_(0), sum_(0.0), name_(name) {
        std::cout << "创建 Statistics: " << name_ << std::endl;
    }
    
    // 基本异常安全 - 统计失败不影响程序
    void addValue(double value) {
        try {
            std::cout << "添加统计值: " << value << std::endl;
            
            // 模拟计算失败
            if (value < 0) {
                throw std::runtime_error("负值不被接受");
            }
            
            sum_ += value;
            ++count_;
        } catch (const std::exception& e) {
            std::cout << "统计添加失败: " << e.what() << std::endl;
            std::cout << "但统计对象仍然有效" << std::endl;
        }
    }
    
    double getAverage() const {
        return count_ > 0 ? sum_ / count_ : 0.0;
    }
    
    void printStats() const {
        std::cout << "统计 " << name_ << ": 数量=" << count_ 
                  << ", 总和=" << sum_ << ", 平均值=" << getAverage() << std::endl;
    }
};

void demonstrate_basic_exception_safety() {
    std::cout << "=== 基本异常安全保证演示 ===" << std::endl;
    
    // 场景1：数据处理
    std::cout << "\n--- 场景1：数据处理 ---" << std::endl;
    DataProcessor processor("Processor1");
    
    // 先添加一些数据
    processor.processData({1, 2, 3});
    processor.printStatus();
    
    // 测试基本异常安全
    std::cout << "\n测试基本异常安全:" << std::endl;
    processor.processData({4, 5, 999, 6});  // 999 会失败
    processor.printStatus();
    
    // 测试强异常安全
    std::cout << "\n测试强异常安全:" << std::endl;
    processor.processDataStrongSafety({7, 8, 999, 9});  // 999 会失败
    processor.printStatus();
    
    // 场景2：日志记录
    std::cout << "\n--- 场景2：日志记录 ---" << std::endl;
    Logger logger("app.log");
    logger.log("INFO: 程序启动");
    logger.log("WARNING: 内存使用率高");
    logger.log("ERROR: 数据库连接失败");  // 这会失败
    logger.log("INFO: 程序继续运行");
    logger.printLogs();
    
    // 场景3：缓存更新
    std::cout << "\n--- 场景3：缓存更新 ---" << std::endl;
    Cache cache(3);
    cache.updateCache("key1", 100);
    cache.updateCache("key2", 200);
    cache.updateCache("key3", 300);
    cache.updateCache("key4", 400);  // 这会失败，缓存已满
    cache.printCache();
    
    // 场景4：统计信息
    std::cout << "\n--- 场景4：统计信息 ---" << std::endl;
    Statistics stats("温度统计");
    stats.addValue(25.5);
    stats.addValue(26.0);
    stats.addValue(-5.0);  // 这会失败，负值
    stats.addValue(27.0);
    stats.printStats();
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    demonstrate_basic_exception_safety();
    
    return 0;
}










