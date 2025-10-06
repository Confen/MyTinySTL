#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"
#include "iterator.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl {

/**
 * @brief 动态数组容器
 * @tparam T 元素类型
 * @tparam Alloc 分配器类型，默认为 mystl::allocator<T>
 * 
 * vector 是一个动态数组容器，支持随机访问，在末尾插入和删除元素的时间复杂度为 O(1)。
 * 当容量不足时会自动扩容，扩容策略为容量翻倍。
 */
template<typename T, typename Alloc = mystl::allocator<T>>
class vector {
private:
    T* begin_;           // 指向第一个元素
    T* end_;            // 指向最后一个元素的下一个位置
    T* cap_;            // 指向分配内存的末尾
    Alloc allocator_;   // 分配器对象

public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    
    typedef T                                           value_type;
    typedef T*                                          pointer;
    typedef const T*                                    const_pointer;
    typedef T&                                          reference;  
    typedef const T&                                    const_reference;
    typedef size_t                                      size_type;
    typedef ptrdiff_t                                   difference_type;
    typedef T*                                          iterator;
    typedef const T*                                    const_iterator;
    typedef mystl::reverse_iterator<iterator>           reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>     const_reverse_iterator;
    typedef Alloc                                       allocator_type;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================

    /**
     * @brief 默认构造函数
     * 创建一个空的 vector，不分配内存
     */
    vector() noexcept : begin_(nullptr), end_(nullptr), cap_(nullptr) {}

    /**
     * @brief 指定大小的构造函数
     * @param n 初始大小
     * 创建包含 n 个默认构造元素的 vector
     */
    explicit vector(size_type n) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
        if (n > 0) {
            begin_ = allocator_.allocate(n);
            end_ = mystl::uninitialized_fill_n(begin_, n, T{});
            cap_ = begin_ + n;
        }
    }

    /**
     * @brief 指定大小和值的构造函数
     * @param n 初始大小
     * @param value 初始值
     * 创建包含 n 个 value 值的 vector
     */
    vector(size_type n, const value_type& value) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
        if (n > 0) {
            begin_ = allocator_.allocate(n);
            end_ = mystl::uninitialized_fill_n(begin_, n, value);
            cap_ = begin_ + n;
        }
    }

    /**
     * @brief 拷贝构造函数
     * @param other 要拷贝的 vector
     * 创建 other 的副本
     */
    vector(const vector& other) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
        if (other.size() > 0) {
            begin_ = allocator_.allocate(other.capacity());
            end_ = cap_ = begin_ + other.size();
            mystl::uninitialized_copy(other.begin_, other.end_, begin_);
        }
    }

    /**
     * @brief 移动构造函数
     * @param other 要移动的 vector
     * 
     * 移动构造函数声明为 noexcept 的原因：
     * 1. 性能优化：标准库容器在选择拷贝vs移动时会检查noexcept
     * 2. 异常安全：移动操作本身不应该失败（只是指针赋值）
     * 3. 标准兼容：与std::vector保持一致
     * 4. 容器优化：作为其他容器的元素时性能更好
     */
    vector(vector&& other) noexcept 
        : begin_(other.begin_), end_(other.end_), cap_(other.cap_) {
        other.begin_ = other.end_ = other.cap_ = nullptr;
    }
    
    /**
     * @brief 初始化列表构造函数
     * @param ilist 初始化列表
     * 使用初始化列表创建 vector
     */
    vector(std::initializer_list<value_type> ilist) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
        if (ilist.size() > 0) {
            begin_ = allocator_.allocate(ilist.size());
            end_ = cap_ = begin_ + ilist.size();
            mystl::uninitialized_copy(ilist.begin(), ilist.end(), begin_);
        }
    }

    /**
     * @brief 析构函数
     * 销毁所有元素并释放内存
     */
    ~vector() {
        if (begin_) {
            mystl::destroy(begin_, end_);
            allocator_.deallocate(begin_, cap_ - begin_);
        }
    }

    // ============================================================================
    // 基础操作
    // ============================================================================

    /**
     * @brief 在末尾添加元素（拷贝版本）
     * @param value 要添加的值
     */
    void push_back(const value_type& value) {
        if (end_ < cap_) {
            // 容量足够，直接构造
            mystl::construct(end_, value);
            ++end_;
        } else {
            // 容量不足，需要扩容
            reallocate_and_insert(end_, value);
        }
    }

    /**
     * @brief 在末尾添加元素（移动版本）
     * @param value 要添加的值
     */
    void push_back(value_type&& value) {
        if (end_ < cap_) {
            // 容量足够，直接构造
            mystl::construct(end_, mystl::move(value));
            ++end_;
        } else {
            // 容量不足，需要扩容
            reallocate_and_insert(end_, mystl::move(value));
        }
    }

    /**
     * @brief 删除末尾元素
     * 注意：如果 vector 为空，行为未定义
     */
    void pop_back() {
        if (end_ > begin_) {
            --end_;
            mystl::destroy(end_);
        }
    }

    // ============================================================================
    // 基础容量查询
    // ============================================================================

    /**
     * @brief 返回当前元素数量
     * @return 元素数量
     */
    size_type size() const noexcept {
        return end_ - begin_;
    }

    /**
     * @brief 返回当前容量
     * @return 容量大小
     */
    size_type capacity() const noexcept {
        return cap_ - begin_;
    }

    /**
     * @brief 检查 vector 是否为空
     * @return 如果为空返回 true，否则返回 false
     */
    bool empty() const noexcept {
        return begin_ == end_;
    }

private:
    /**
     * @brief 重新分配内存并插入元素
     * @param pos 插入位置
     * @param value 要插入的值
     */
    template<typename U>
    void reallocate_and_insert(pointer pos, U&& value) {
        size_type old_size = size();
        size_type new_capacity = old_size == 0 ? 1 : old_size * 2;
        
        pointer new_begin = allocator_.allocate(new_capacity);
        pointer new_end = new_begin;
        
        try {
            // 拷贝前半部分
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            // 构造新元素
            mystl::construct(new_end, mystl::forward<U>(value));
            ++new_end;
            // 拷贝后半部分
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        } catch (...) {
            // 异常安全：清理已构造的元素
            mystl::destroy(new_begin, new_end);
            allocator_.deallocate(new_begin, new_capacity);
            throw;
        }
        
        // 销毁旧元素并释放旧内存
        mystl::destroy(begin_, end_);
        allocator_.deallocate(begin_, cap_ - begin_);
        
        // 更新指针
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_capacity;
    }

public:
/**
 * @brief 拷贝赋值操作符
 * @param other 要拷贝的vector
 * @return 当前vector的引用
 * 
 * 异常安全保证：强异常保证
 * 如果赋值失败，当前vector状态不变
 */
    vector& operator=(const vector&other)
    {
        if(this != &other)
        //
        {
            //如果容量足够，直接赋值
            if(other.size() <= capacity())
            {
                //销毁当前元素
                mystl::destroy(begin_,end_);
                //拷贝新元素
                mystl::uninitialized_copy(other.begin_,other.end_,begin_);
                end_= begin_ + other.size(); 
            }  else {
                //如果容量不足，需要扩容
                vector temp(other); //创建临时对象，强异常保证
                mystl::swap(*this, temp);
            }
        }
        return *this;
    }

    /**
 * @brief 移动赋值操作符
 * @param other 要移动的vector
 * @return 当前vector的引用
 * 
 * 异常安全保证：无异常保证（noexcept）
 * 移动操作不会失败
 */
    vector& operator=(vector&&other) noexcept {
        if(this != &other) {
            //销毁当前元素
            mystl::destroy(begin_,end_);
            allocator_.deallocate(begin_,cap_ - begin_);

            //移动other到当前对象
            begin_ = other.begin_;
            end_ = other.end_;
            cap_ = other.cap_;

            //将other置空
            other.begin_ = other.end_ = other.cap_ = nullptr;
        }
        return *this;
    }
/**
 * @brief 初始化列表赋值操作符
 * @param ilist 初始化列表
 * @return 当前vector的引用
 */
    vector& operator=(std::initializer_list<value_type> ilist) {
        assign(ilist.begin(),ilist.end());
        return *this;
    }

    /**
 * @brief 下标访问操作符（非const版本）
 * @param pos 位置索引
 * @return 元素的引用
 * 
 * 注意：不进行边界检查，访问越界行为未定义
 */
    reference operator[](size_type pos) {
        return begin_[pos];
    }

/**
 * @brief 下标访问操作符（const版本）
 * @param pos 位置索引
 * @return 元素的const引用
 */
    const_reference operator[](size_type pos) const {
        return begin_[pos];
    }

   /**
 * @brief 边界检查访问方法（非const版本）
 * @param pos 位置索引
 * @return 元素的引用
 * @throws std::out_of_range 如果pos超出范围
 */
 reference at(size_type pos) {
    if(pos >= size()) {
        throw std::out_of_range("vector::at: pos out of range");
    }
    return begin_[pos];
 }

 /**
 * @brief 边界检查访问方法（const版本）
 * @param pos 位置索引
 * @return 元素的const引用
 * @throws std::out_of_range 如果pos超出范围
 */
const_reference at(size_type pos) const {
    if (pos >= size()) {
        throw std::out_of_range("vector::at : out of range");
    }
    return begin_[pos];
}

reference front() {
    return *begin_;
};

const_reference front() const{
    return *begin_;
}

reference back() {
    return *(end_ - 1);  //end_指向最后一个元素的下一个位置
}

const_reference back() const{
    return *(end_ - 1);
}

pointer data() noexcept {
    return begin_;
}

const_pointer data() const noexcept {
    return begin_;
}

void reserve(size_type new_cap) {
    if(new_cap > capacity()) {
        reallocate(new_cap);
    }
}

void resize(size_type count) {
    resize(count,value_type{});
}

/**
 * @brief 调整vector大小（带默认值）
 * @param count 新的元素数量
 * @param value 新元素的默认值
 */
void resize(size_type count, const value_type& value){
    if(count < size()) {
        mystl::destroy(begin_ + count , end_);
        end_ = begin_ + count;
    }else if (count > size()) {
        if(count > capacity()) {
            reallocate(count);
        }
        mystl::uninitialized_fill_n(end_, count - size(), value);
        end_ = begin_ + count;
    }
}

/**
 * @brief 清空vector
 * 销毁所有元素，但保留容量
 */
void clear() noexcept {
    mystl::destroy(begin_, end_);
    end_ = begin_;
}

void shrink_to_fit() {
    if(size() < capacity()) {
        reallocate(size());
    }
}

size_type max_size() const noexcept{
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}
/**
 * @brief 重新分配内存到指定容量
 * @param new_capacity 新的容量
 */

void reallocate(size_type new_capacity) {
    if(new_capacity == 0)
    {
        if(begin_){
        mystl::destroy(begin_,end_);
        allocator_.deallocate(begin_,cap_ - begin_);
        begin_ = end_ = cap_ = nullptr;
        }
        return ;
    }
     pointer new_begin = allocator_.allocate(new_capacity);
     pointer new_end = new_begin;

     try {
        new_end = mystl::uninitialized_move(begin_,end_,new_begin);
     }
     catch(...) {
        mystl::destroy(new_begin,new_end);
        allocator_.deallocate(new_begin,new_capacity);
        throw;
     }
//如果try抛出异常，以下代码不会执行
     mystl::destroy(begin_,end_);
     allocator_.deallocate(begin_,cap_ - begin_);

     begin_ = new_begin;
     end_ = new_end;
     cap_ = new_begin + new_capacity;
}
};

} // namespace mystl

#endif // MYTINYSTL_VECTOR_H