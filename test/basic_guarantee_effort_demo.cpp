#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

// 演示类：模拟可能失败的操作
class ResourceClass {
private:
    int value_;
    std::string* resource_;
    static int count_;
    
public:
    ResourceClass(int v) : value_(v) {
        ++count_;
        std::cout << "构造 ResourceClass(" << value_ << "), 总数: " << count_ << std::endl;
        
        resource_ = new std::string("Resource_" + std::to_string(value_));
        
        // 在某些情况下抛出异常
        if (count_ == 3) {
            std::cout << "  构造失败，抛出异常！" << std::endl;
            throw std::runtime_error("资源分配失败");
        }
    }
    
    ~ResourceClass() {
        --count_;
        std::cout << "析构 ResourceClass(" << value_ << "), 剩余: " << count_ << std::endl;
        delete resource_;
    }
    
    int getValue() const { return value_; }
    const std::string& getResource() const { return *resource_; }
    
    static int getCount() { return count_; }
    static void resetCount() { count_ = 0; }
};

int ResourceClass::count_ = 0;

// 演示1：基本异常保证的"尽力原则"
void demo_basic_guarantee_effort() {
    std::cout << "=== 演示1：基本异常保证的"尽力原则" ===" << std::endl;
    
    // 基本异常保证的容器
    class BasicGuaranteeContainer {
    private:
        std::vector<ResourceClass*> data_;
        
    public:
        ~BasicGuaranteeContainer() {
            for (auto* ptr : data_) {
                delete ptr;
            }
        }
        
        // 基本异常保证：尽力添加元素
        void addElement(int value) {
            std::cout << "尝试添加元素 " << value << "..." << std::endl;
            
            try {
                ResourceClass* new_obj = new ResourceClass(value);
                data_.push_back(new_obj);
                std::cout << "添加成功，当前大小: " << data_.size() << std::endl;
            } catch (const std::exception& e) {
                std::cout << "添加失败: " << e.what() << std::endl;
                std::cout << "基本异常保证：容器仍然有效，大小: " << data_.size() << std::endl;
                // 不重新抛出异常，而是尽力保持容器有效
            }
        }
        
        // 基本异常保证：尽力批量添加
        void addElements(const std::vector<int>& values) {
            std::cout << "尝试批量添加 " << values.size() << " 个元素..." << std::endl;
            
            size_t original_size = data_.size();
            
            for (int value : values) {
                try {
                    ResourceClass* new_obj = new ResourceClass(value);
                    data_.push_back(new_obj);
                    std::cout << "添加元素 " << value << " 成功" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "添加元素 " << value << " 失败: " << e.what() << std::endl;
                    std::cout << "基本异常保证：容器仍然有效，已添加: " << (data_.size() - original_size) << " 个" << std::endl;
                    // 继续尝试添加下一个元素
                }
            }
        }
        
        size_t size() const { return data_.size(); }
        
        void printContents() const {
            std::cout << "容器内容: ";
            for (size_t i = 0; i < data_.size(); ++i) {
                std::cout << data_[i]->getValue() << " ";
            }
            std::cout << std::endl;
        }
    };
    
    ResourceClass::resetCount();
    
    BasicGuaranteeContainer container;
    
    // 添加一些元素
    container.addElement(1);
    container.addElement(2);
    container.addElement(3);  // 这个会失败
    
    std::cout << "当前容器大小: " << container.size() << std::endl;
    container.printContents();
    
    // 批量添加
    std::vector<int> batch_values = {4, 5, 6, 7, 8};
    container.addElements(batch_values);
    
    std::cout << "最终容器大小: " << container.size() << std::endl;
    container.printContents();
}

// 演示2：基本异常保证 vs 强异常保证
void demo_basic_vs_strong_guarantee() {
    std::cout << "\n=== 演示2：基本异常保证 vs 强异常保证 ===" << std::endl;
    
    // 基本异常保证的类
    class BasicGuaranteeClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            std::cout << "基本异常保证：尽力添加元素" << std::endl;
            
            for (int elem : elements) {
                try {
                    if (elem > 100) {
                        throw std::runtime_error("元素过大");
                    }
                    data_.push_back(elem);
                    std::cout << "添加元素 " << elem << " 成功" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "添加元素 " << elem << " 失败: " << e.what() << std::endl;
                    std::cout << "基本异常保证：容器仍然有效，当前大小: " << data_.size() << std::endl;
                    // 继续尝试添加下一个元素
                }
            }
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    // 强异常保证的类
    class StrongGuaranteeClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            std::cout << "强异常保证：要么全部成功，要么全部失败" << std::endl;
            
            std::vector<int> temp = data_;
            
            try {
                for (int elem : elements) {
                    if (elem > 100) {
                        throw std::runtime_error("元素过大");
                    }
                    temp.push_back(elem);
                    std::cout << "添加元素 " << elem << " 成功" << std::endl;
                }
                data_ = std::move(temp);
                std::cout << "所有元素添加成功" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "添加失败: " << e.what() << std::endl;
                std::cout << "强异常保证：对象状态完全不变，大小: " << data_.size() << std::endl;
                throw;  // 重新抛出异常
            }
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    std::vector<int> test_data = {1, 2, 150, 4, 5};  // 第3个元素会失败
    
    // 测试基本异常保证
    std::cout << "测试基本异常保证版本:" << std::endl;
    BasicGuaranteeClass basic_obj;
    basic_obj.addElements(test_data);
    std::cout << "基本异常保证结果：大小 " << basic_obj.size() << std::endl;
    
    // 测试强异常保证
    std::cout << "\n测试强异常保证版本:" << std::endl;
    StrongGuaranteeClass strong_obj;
    try {
        strong_obj.addElements(test_data);
    } catch (const std::exception& e) {
        std::cout << "强异常保证：操作失败，对象状态不变" << std::endl;
    }
    std::cout << "强异常保证结果：大小 " << strong_obj.size() << std::endl;
}

// 演示3：基本异常保证的实际应用
void demo_basic_guarantee_practical() {
    std::cout << "\n=== 演示3：基本异常保证的实际应用 ===" << std::endl;
    
    // 文件操作类（基本异常保证）
    class FileManager {
    private:
        std::vector<std::string> file_list_;
        std::vector<bool> file_status_;
        
    public:
        // 基本异常保证：尽力打开文件
        void openFiles(const std::vector<std::string>& filenames) {
            std::cout << "尝试打开 " << filenames.size() << " 个文件..." << std::endl;
            
            for (const std::string& filename : filenames) {
                try {
                    // 模拟文件打开操作
                    if (filename == "error.txt") {
                        throw std::runtime_error("文件不存在");
                    }
                    
                    file_list_.push_back(filename);
                    file_status_.push_back(true);
                    std::cout << "打开文件 " << filename << " 成功" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "打开文件 " << filename << " 失败: " << e.what() << std::endl;
                    std::cout << "基本异常保证：已打开的文件仍然有效" << std::endl;
                    // 继续尝试打开下一个文件
                }
            }
        }
        
        // 基本异常保证：尽力保存文件
        void saveFiles() {
            std::cout << "尝试保存 " << file_list_.size() << " 个文件..." << std::endl;
            
            for (size_t i = 0; i < file_list_.size(); ++i) {
                try {
                    // 模拟文件保存操作
                    if (file_list_[i] == "readonly.txt") {
                        throw std::runtime_error("文件只读");
                    }
                    
                    file_status_[i] = true;
                    std::cout << "保存文件 " << file_list_[i] << " 成功" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "保存文件 " << file_list_[i] << " 失败: " << e.what() << std::endl;
                    std::cout << "基本异常保证：其他文件仍然有效" << std::endl;
                    // 继续尝试保存下一个文件
                }
            }
        }
        
        size_t getFileCount() const { return file_list_.size(); }
        size_t getSavedCount() const {
            size_t count = 0;
            for (bool status : file_status_) {
                if (status) ++count;
            }
            return count;
        }
    };
    
    FileManager manager;
    
    // 打开文件
    std::vector<std::string> filenames = {"file1.txt", "file2.txt", "error.txt", "file3.txt"};
    manager.openFiles(filenames);
    
    std::cout << "成功打开 " << manager.getFileCount() << " 个文件" << std::endl;
    
    // 保存文件
    manager.saveFiles();
    
    std::cout << "成功保存 " << manager.getSavedCount() << " 个文件" << std::endl;
}

// 演示4：基本异常保证的局限性
void demo_basic_guarantee_limitations() {
    std::cout << "\n=== 演示4：基本异常保证的局限性 ===" << std::endl;
    
    // 基本异常保证的局限性
    class LimitedClass {
    private:
        std::vector<int> data_;
        
    public:
        void addElements(const std::vector<int>& elements) {
            std::cout << "基本异常保证：尽力添加元素" << std::endl;
            
            for (int elem : elements) {
                try {
                    if (elem > 100) {
                        throw std::runtime_error("元素过大");
                    }
                    data_.push_back(elem);
                    std::cout << "添加元素 " << elem << " 成功" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "添加元素 " << elem << " 失败: " << e.what() << std::endl;
                    std::cout << "基本异常保证：容器仍然有效，但状态已改变" << std::endl;
                    std::cout << "当前大小: " << data_.size() << "（与原始状态不同）" << std::endl;
                    // 继续尝试添加下一个元素
                }
            }
        }
        
        size_t size() const { return data_.size(); }
        const std::vector<int>& getData() const { return data_; }
    };
    
    LimitedClass obj;
    
    std::vector<int> test_data = {1, 2, 150, 4, 5};
    
    std::cout << "原始状态：大小 " << obj.size() << std::endl;
    
    obj.addElements(test_data);
    
    std::cout << "最终状态：大小 " << obj.size() << std::endl;
    std::cout << "基本异常保证：对象有效，但状态已改变" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    ResourceClass::resetCount();
    
    demo_basic_guarantee_effort();
    demo_basic_vs_strong_guarantee();
    demo_basic_guarantee_practical();
    demo_basic_guarantee_limitations();
    
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "基本异常保证的"尽力原则"特点：" << std::endl;
    std::cout << "1. 尽力而为：尽可能完成操作，即使部分失败" << std::endl;
    std::cout << "2. 容错性：单个元素失败不影响其他元素" << std::endl;
    std::cout << "3. 实用性：适合批量操作和容错场景" << std::endl;
    std::cout << "4. 局限性：对象状态可能已改变" << std::endl;
    std::cout << "5. 适用场景：文件操作、网络操作、批量处理等" << std::endl;
    
    return 0;
}
















