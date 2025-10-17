#ifndef MYTINYSTL_DEQUE_H
#define MYTINYSTL_DEQUE_H

#include<new>
#include <cstddef>
#include <type_traits>
#include "iterator.h" 
#include "util.h"
#include "construct.h"
#include "exceptdef.h"

namespace mystl {
    
    template <typename T,std::size_t BufS = 0>
    struct deque_buf_size {
        static constexpr size_t value =  BufS != 0 ? BufS : 
        //static_cast<size_t>(1)为1 * sizeof(T),表示至少有一个元素，为有效缓冲区
        (sizeof(T) < 512 ? static_cast<size_t>(512 / sizeof(T)) : static_cast<size_t>(1)); //至少一个缓冲区
    };

    //非类型模板参数（int,size_t）BufS为0时，即表示使用默认值
    template <typename T,std::size_t BufS = 0>
    struct deque_iterator {
        //基本类型定义
        using value_type        = T;
        using pointer           =T*;
        using reference         =T&;
        using difference_type   =std::ptrdiff_t;
        using iterator_category = mystl::random_access_iterator_tag;
        using nonconst_pointer  =typename std::remove_const<T>::type*;
        using map_pointer       =nonconst_pointer*;
        //缓冲区大小
        static constexpr std::size_t buffer_size = deque_buf_size<T,BufS>::value;
        nonconst_pointer cur;
        nonconst_pointer first;
        nonconst_pointer last;
        //指向的是map 中“当前块指针”的那个槽位本身（T** 指向 map[i]）
        map_pointer node;
        void set_node(map_pointer newnode) {
            node = newnode;
            first = *newnode;
            last = first + static_cast<difference_type>(buffer_size);
        }
        deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
        deque_iterator(nonconst_pointer cur_, nonconst_pointer first_, nonconst_pointer last_, map_pointer node_) : cur(cur_), first(first_)
        , last(last_), node(node_) {}
        reference operator*() const {return * cur;}
        pointer operator->() const {return cur;}

        bool operator==(const deque_iterator& rhs) const {return cur == rhs.cur && node == rhs.node;}
        bool operator!=(const deque_iterator& rhs) const {return !(*this == rhs);}

        deque_iterator& operator++() noexcept{++cur; if(cur == last){set_node(node + 1); cur = first;} return *this;}
        deque_iterator operator++(int) noexcept{deque_iterator tmp = *this;++(*this);return tmp;} //会修改*this,所以不能const

        deque_iterator& operator--() noexcept {
            if(cur == first){set_node(node-1);
            cur = last;} 
            --cur;
            return*this;
         }
        deque_iterator operator--(int)noexcept {deque_iterator tmp = *this; --(*this); return tmp;} //会修改*this,所以不能const

        deque_iterator& operator+=(difference_type n) noexcept {
            difference_type offset = n + (cur - first);
            const difference_type bs = static_cast<difference_type>(buffer_size);
            if (0 <= offset && offset < bs) {
                cur += n;
                return *this;
            } else {
                difference_type node_offset = (offset >= 0)
                    ? (offset / bs)
                    : -((-offset + bs - 1) / bs);
                set_node(node + node_offset);
                cur = first + (offset - node_offset * bs);
                return *this;
            }
        }
    deque_iterator& operator-=(difference_type n) noexcept {
        operator+=(-n);
        return *this;
    }
    deque_iterator operator+(difference_type n) const noexcept{
        auto tmp = *this;
        tmp += n;
        return tmp;
    }
    deque_iterator operator-(difference_type n) const noexcept{
        auto tmp = *this;
        tmp += -n;
        return tmp;
    }
    difference_type operator-(const deque_iterator& rhs) const noexcept{
        if(node == rhs.node) return cur - rhs.cur;
        else {
            difference_type node_diff = node - rhs.node;
            difference_type this_off = cur - first;
            difference_type rhs_off = rhs.cur - rhs.first;
            return node_diff * static_cast<difference_type>(buffer_size) + this_off - rhs_off;
        }
    }

    reference operator[](difference_type n) const noexcept{
        return *(*this + n);
    }

    bool operator<(const deque_iterator& rhs) const noexcept{
        return (node== rhs.node) ? (cur < rhs.cur) : (node < rhs.node);
    }

    bool operator>=(const deque_iterator& rhs) const noexcept
    {
        return rhs < *this || rhs == *this;
    }

    bool operator>(const deque_iterator& rhs) const noexcept
    {
        return rhs < *this;
    }
};

    template <typename T,std::size_t BufS = 0>
    class deque {
        public:
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using const_pointer     = const T*;
        using const_reference   = const T&;
        using size_type         = std::size_t;
        using difference_type   = std::ptrdiff_t;

        using iterator          =deque_iterator<T,BufS>;
        using const_iterator    =deque_iterator<const T,BufS>;

        private:
         // map 区：指向各缓冲块首地址的指针数组
        pointer* map_;//缓冲区首地址
        size_type map_size_;//缓冲区数量

         // 有效区：起止迭代器标识当前数据范围
        iterator start_;//起始迭代器    //start_.first 属于 start_ 当前所处的块（首部所在块，不会指向相邻备用空块）
        iterator finish_;//结束迭代器   //finish_.last 属于 finish_ 当前所处的块（尾后所在块，可能是备用空块）

        size_type size_;
        //分配器,释放节点
        pointer allocate_node(){
            const size_type n = deque_buf_size<T,BufS>::value;
        return static_cast<pointer>(::operator new(sizeof(T) * n));
        }
        //强异常保证的插入实现(待实现)
        iterator do_insert_strong(iterator pos,const_iterator first,const_iterator last)

        iterator do_insert_strong(iterator pos,iterator first,iterator last)

        void deallocate_node(pointer buf) noexcept{::operator delete(buf);}
        //创建map区和节点,释放节点
        void create_map_and_nodes(size_type num_nodes){
            map_ = static_cast<pointer*>(::operator new(sizeof(pointer) * num_nodes));
            map_size_ = num_nodes;
            for(size_type i = 0; i < num_nodes; ++i)
            {
                map_[i] = nullptr;
            }
        }
        void destroy_map_and_nodes() noexcept{
            if (!map_) return;
            // 释放所有已分配的块
            for (size_type i = 0; i < map_size_; ++i) {
                if (map_[i]) {
                    deallocate_node(map_[i]);
                    map_[i] = nullptr;
                }
            }
            ::operator delete(map_);
            map_ = nullptr;
            map_size_ = 0;
        }

        void initialize_empty()
        {
            map_size_ = 8;
            map_ =static_cast<pointer*>(::operator new(sizeof(pointer) * map_size_));
            const size_type center = map_size_ / 2;
            for (size_type i = 0; i < map_size_; ++i) {
                map_[i] = nullptr;
              }
            map_[center] = allocate_node();
            start_.set_node(map_ + center);
            finish_.set_node(map_ + center);
            start_.cur = start_.first;
            finish_.cur = start_.first;
        }

        //目标：当块内满时，若相邻槽位是空的，就分配新块并且将新块的指针赋值给相邻槽位，然后跳过去再插入,
        // 若相邻槽位不可用(下一步做 map 扩容）
        void require_back_slot(){
            if (finish_.cur != finish_.last) return; // 当前块未满，无需处理

            // 需要移动到尾部的下一个块
            if (finish_.node + 1 < map_ + map_size_) {
                // map 仍有槽位
                if (*(finish_.node + 1) == nullptr) {
                    *(finish_.node + 1) = allocate_node();
                }
            } else {
                // map 无槽位，需要扩容（在尾部预留）
                reallocate_map(1, false);
                if (*(finish_.node + 1) == nullptr) {
                    *(finish_.node + 1) = allocate_node();
                }
            }
            finish_.set_node(finish_.node + 1);
            finish_.cur = finish_.first;
        }

        void require_front_slot(){
            if (start_.cur != start_.first) return; // 当前块前部仍有空间

            if (start_.node > map_) {
                // map 左侧仍有槽位
                if (*(start_.node - 1) == nullptr) {
                    *(start_.node - 1) = allocate_node();
                }
            } else {
                // 已在最左端，无槽位可用，扩容到前部
                reallocate_map(1, true);
                if (*(start_.node - 1) == nullptr) {
                    *(start_.node - 1) = allocate_node();
                }
            }
            start_.set_node(start_.node - 1);
            start_.cur = start_.last;
        }
        
        void reallocate_map(std::size_t nodes_to_add,bool add_at_front)
        {
            const size_type old_nodes = static_cast<size_type>(finish_.node - start_.node + 1);
            size_type new_map_size = mystl::max(map_size_ * 2, map_size_ + nodes_to_add + 2);

            pointer* new_map = static_cast<pointer*>(::operator new(sizeof(pointer) * new_map_size));
            for(size_type i = 0;i < new_map_size;++i) new_map[i] = nullptr;

            const size_type old_start_index = static_cast<size_type>(start_.node - map_);
            const size_type bias = add_at_front ? nodes_to_add : 0;
            const size_type new_start_index = (new_map_size - old_nodes) /  2 + bias;

            //拷贝旧的map到新的map
            for(size_type i = 0;i <old_nodes;++i)
            {
                new_map[new_start_index + i] = map_[old_start_index + i];
            }
            //保留块内偏移
            const difference_type start_off = start_.cur - start_.first;
            const difference_type finish_off = finish_.cur - finish_.first;

            const size_type new_finish_index = new_start_index + old_nodes - 1;
            start_.set_node(new_map + new_start_index);
            finish_.set_node(new_map + new_finish_index);

            start_.cur = start_.first + start_off;
            finish_.cur = finish_.first + finish_off;

            ::operator delete(map_);
            map_ = new_map;
            map_size_ = new_map_size;
        }
       public:
       deque() noexcept : map_(nullptr),map_size_(0),start_(),finish_(),size_(0){initialize_empty();}

       bool      empty() const noexcept {return start_ == finish_;}
       size_type size()  const noexcept {return size_;}  //等待实现

       reference front(){return *start_.cur;}
       const_reference front() const{return *start_.cur;}
       reference back(){iterator tmp = finish_;--tmp;return *tmp;}
       const_reference back() const{iterator tmp = finish_;--tmp;return *tmp;}

       iterator begin() noexcept{iterator tmp = start_;return tmp;}
       const_iterator begin() const noexcept{return const_iterator(start_.cur,start_.first,start_.last,start_.node);}
       const_iterator cbegin() const noexcept{return const_iterator(start_.cur,start_.first,start_.last,start_.node);}
       iterator end() noexcept{iterator tmp = finish_;return tmp;}
       const_iterator end() const noexcept{return const_iterator(finish_.cur,finish_.first,finish_.last,finish_.node);}
       const_iterator cend() const noexcept{return const_iterator(finish_.cur,finish_.first,finish_.last,finish_.node);}

       void push_back(const value_type& value){
        require_back_slot();
        if(finish_.cur != finish_.last){
           mystl::construct(finish_.cur,value);
            ++finish_.cur;
        }     
        ++size_;
    }
       void push_back(value_type&& value){
        require_back_slot();
        if(finish_.cur != finish_.last){
          mystl::construct(finish_.cur,mystl::forward<value_type>(value));
            ++finish_.cur;
        }
        ++size_;
       }

       void push_front(const value_type& value){
        require_front_slot();
        if(start_.cur != start_.first){
          mystl::construct(start_.cur - 1,value);
            --start_.cur;
        }
        ++size_;
       }
       void push_front(value_type&& value){
        require_front_slot();
        if(start_.cur != start_.first){
            mystl::construct(start_.cur - 1,mystl::forward<value_type>(value));
            --start_.cur;
        }
        ++size_;
        }
       void pop_back(){
        if(empty())return;
        if(finish_.cur != finish_.first){
            --finish_.cur;           
           mystl::destroy(finish_.cur);
        }else{
            pointer old_buf = finish_.first;
            size_type index = static_cast<size_type>(finish_.node - map_);
            *(map_ + index) = nullptr;
            deallocate_node(old_buf);

            finish_.set_node(finish_.node - 1);
            finish_.cur = finish_.last - 1;
            mystl::destroy(finish_.cur);
        }
        --size_;
       }
       void pop_front(){
        if(empty()) return;
        if(start_.cur != start_.last - 1){
           mystl::destroy(start_.cur);
            ++start_.cur;
        } else{
            mystl::destroy(start_.cur);

            pointer old_buf = start_.first;
            size_type index = static_cast<size_type>(start_.node - map_);
            *(map_ + index) = nullptr;  
            deallocate_node(old_buf);

           start_.set_node(start_.node + 1);
           start_.cur = start_.first;
        }
        --size_;
       }

       reference operator[](size_type n)
       {
        iterator it  = start_;
        it += static_cast<difference_type>(n);
        return *it;
       }
       const_reference operator[](size_type n) const
       {
        const_iterator it = start_;
        it += static_cast<difference_type>(n);
        return *it;
       }

       reference at(size_type n)
       {
        if(n >= size_) throw mystl::mystl_out_of_range("deque::at: n is out of range");
        return (*this)[n];
       }

       const_reference at(size_type n) const
       {
        if(n >= size_) throw mystl::mystl_out_of_range("deque::at: n is out of range");
        return (*this)[n];
       }

       void clear() noexcept {
        // moved-from 保护：map_ 为空时不做任何指针运算
        //防止反复释放内存
        if (!map_) {
            size_ = 0;
            start_ = iterator();
            finish_ = iterator();
            return;
        }
        // 析构所有元素
        while (!empty()) { pop_back(); }

        // 释放除当前槽位外的所有块
        const size_type start_index = static_cast<size_type>(start_.node - map_);
        for (pointer* p = map_; p != map_ + map_size_; ++p) {
            if (p != start_.node && *p) {
                deallocate_node(*p);
                *p = nullptr;
            }
        }

        // 确保当前槽位有一个有效块
        if (map_[start_index] == nullptr) {
            map_[start_index] = allocate_node();
        }

        start_.set_node(map_ + start_index);
        finish_.set_node(map_ + start_index);
        start_.cur = start_.first;
        finish_.cur = finish_.first;
        size_ = 0;
       }

       ~deque() {
        if (!map_) return; // moved-from 安全
        clear();
        const size_type idx = static_cast<size_type>(start_.node - map_);
        if (map_[idx]) {
            deallocate_node(map_[idx]);
            map_[idx] = nullptr;
        }
        ::operator delete(map_);
        map_ = nullptr;
        map_size_ = 0;
       }

       // 收缩到合适大小
       void shrink_to_fit() noexcept{
            if(!map_) return;
            const size_type old_nodes = static_cast<size_type>(finish_ - start_ + 1);
            size_type new_map_size = old_nodes + 2;
            if(new_map_size < static_cast<size_type>(8)){
                new_map_size = static_cast<size_type>(8);
            }
            if(new_map_size > map_size_) {new_map_size = map_size_;}

            const difference_type start_off = start_.cur - start_.first;
            const difference_type finish_off = finish_.cur - finish_.first;

            pointer*new_map = static_cast<pointer*>(::operator new(sizeof(pointer) * new_map_size));
            for(size_type i = 0;i < new_map_size;++i) new_map[i] = nullptr;

            const size_type old_start_index = static_cast<size_type>(start_.node - map_);
            const size_type new_start_index = (new_map_size - old_nodes) / 2;
            for(size_type i =0;i < old_nodes;++i)
            {
                new_map[new_start_index + i] = map_[old_start_index + i];
            }

            start_.set_node(new_map + new_start_index);
            finish_.set_node(new_map + new_start_index + old_nodes - 1);
            start_.cur = start_.first + start_off;
            finish_.cur = finish_.first + finish_off;

            ::operator delete(map_);
            map_ = new_map;
            map_size_ = new_map_size;
       }

       void resize(size_type count) {
        if(count < size_) {
            while (size_ > count) pop_back();
        }else if(count > size_) {
                while(size_ < count) push_back(value_type());
            }
        }

        void resize(size_type count,const value_type& value)
        {
            if(count < size_) {
                while (size_ > count) pop_back();
            }else if(count > size_) {
                while(size_ < count) push_back(value);
            }
        }

        void assign(size_type count,const value_type& value){
           clear();
           while(count--) push_back(value);
        }
        //如果InputIterator是整数类型，则不进行赋值,通过SFINAE机制,使用enable_if来选择合适的函数重载
        template <typename InputIterator,typename = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
        void assign(InputIterator first,InputIterator last){
            clear();
            for(;first != last;++first) push_back(*first);}
            //或者指针哑元类型
            // template <class InputIt>
            // typename std::enable_if<!std::is_integral<InputIt>::value, void>::type
            // assign(InputIt first, InputIt last) {
            // clear();
            // for (; first != last; ++first) push_back(*first);
        //初始化列表赋值
       void assign(std::initializer_list<value_type> ilist){
        clear();
        assign(ilist.begin(),ilist.end());
       }
        //返回插入位置的迭代器
        //单点插入，左半段右移，右半段左移
        iterator insert(iterator pos,const value_type& value){
            difference_type index = pos - start_;

            if(index < static_cast<difference_type>(size_ / 2)) {
                push_front(front());
                pos = start_ + 1;
                iterator it_src = start_ + 1;
                iterator it_dst = start_;
                for(;it_src != pos;++it_src,++it_dst)
                {
                    *it_dst = mystl::move(*it_src);
                }
                *(start_ + index) = value;
                return start_ + index;
            }else{
                push_back(back());
                pos = start_ + index;
                iterator it_src =  finish_ - 2;
                iterator it_dst = finish_ - 1;
                while(it_src >= pos) {
                    *it_dst = mystl::move(*it_src);
                    --it_dst;
                    --it_src;
                }
                *pos = value;
                return pos;
            }
        }
        //右值插入，左半段右移，右半段左移
        iterator insert(iterator pos,value_type&& value)
        {
            difference_type index = pos - start_;

            if(index < static_cast<difference_type>(size_ / 2)) {
                push_front(front());
                pos = start_ + 1;
                iterator it_src = start_ + 1;
                iterator it_dst = start_;
                for(;it_src != pos;++it_src,++it_dst)
                {
                    *it_dst = mystl::move(*it_src);
                }
                *(start_ + index) = mystl::move(value);
                return start_ + index;
            }else{
                push_back(back());
                pos = start_ + index;
                iterator it_src =  finish_ - 2;
                iterator it_dst = finish_ - 1;
                while(it_src >= pos) {
                    *it_dst = mystl::move(*it_src);
                    --it_dst;
                    --it_src;
                }
                *pos = mystl::move(value);
                return start_ + index;
            }
        }
        //单点插入，左半段右移，右半段左移
        iterator insert(iterator pos,size_type count,const value_type& value)
        {
            if(count == 0) return pos;
            difference_type index = pos - start_;
            for(size_type i = 0;i < count;++i)
            {
                pos = insert(start_ + index + static_cast<difference_type>(i),value);
            }
            return start_ + index;
        }
        //区间插入，左半段右移，右半段左移
        //SFINAE机制，如果InputIterator是整数类型，则不进行插入
        template<typename InputIterator,typename 
        = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
        iterator insert(iterator pos,InputIterator first,InputIterator last)
        {
            if(first == last) return pos;
            difference_type index = pos - start_;
            size_type i = 0;
            for(;first != last;++first,++i)
            {
                pos = insert(start_ + index + static_cast<difference_type>(i),*first);
            }
            return start_ + index;
        }
        //初始化列表插入，左半段右移，右半段左移
        iterator insert(iterator pos,std::initializer_list<value_type> ilist)
        {
            difference_type index = pos - start_;
            size_type i = 0;
            for(const auto& it : ilist)
            {
                pos = insert(start_ + index + static_cast<difference_type>(i),it);
                ++i;
            }
            return start_ + index;
        }

        //区间插入，左半段右移，右半段左移
        iterator insert(iterator pos,const_iterator first,const_iterator last)
        {
            difference_type index = pos - start_;
            size_type n = static_cast<size_type>(last - first);
            if(n == 0) return start_ + index;
            pointer tmp = static_cast<pointer>(::operator new(sizeof(value_type)*n));
            for(int i = 0;first != last;++first,++i)
            {
                mystl::construct(tmp + i,*first);
            }
            for(int i = 0;i< n;i++)
            {
                insert(start_ + index + static_cast<difference_type>(i),*(tmp + i));
            }
            mystl::destroy(tmp,tmp + n);
            ::operator delete(tmp);
            return start_ + index;
        }
        //单点erase，左半段右移，右半段左移
        iterator erase(iterator pos) {
           difference_type index = pos - start_;
           if(index < static_cast<difference_type>(size_ / 2)) {
            if (pos != start_) {
                iterator it_src = pos - 1;
                iterator it_dst = pos;
                while(it_src >= start_) {
                    *it_dst = mystl::move(*it_src);
                    --it_src;
                    --it_dst;
                }
            }
            pop_front();
          //返回删除元素的下一个位置
            return start_ + index;
           }
           else {
                iterator it_src = pos + 1;
                iterator it_dst = pos;
               for(;it_src != finish_;++it_src,++it_dst)
                { 
                    *it_dst = *it_src;
                }
                pop_back();
                return start_ + index;
           }
        }

        //区间erase，左半段右移，右半段左移
        iterator erase(iterator first,iterator last) {
            if(first ==  last) return first;
            difference_type index_first = first - start_;
            difference_type index_last = last - start_;
            size_type n = static_cast<size_type>(index_last - index_first);  

            if(index_first < static_cast<difference_type>(size_ - index_last))
            {
                if (first != start_) {
                    iterator it_src = first - 1;
                    iterator it_dst = last - 1;
                    while(it_src >= start_) {
                        *it_dst = *it_src;
                        --it_dst;
                        --it_src;
                    }
                }
              for(size_type i = 0;i < n;i++) pop_front();
              return start_ + index_first;
            } else {
                iterator it_src = last;
                iterator it_dst = first;
                for(;it_src != finish_;++it_src,++it_dst)
                {
                    *it_dst= *it_src;
                }
                for(size_type i = 0;i < n;i++) pop_back();
                return start_ + index_first;
            }       
        }
//emplace_back返回最后一个元素的迭代器
        template <class... Args>
        iterator emplace_back(Args&&... args) {
            require_back_slot();
            mystl::construct(finish_.cur,mystl::forward<Args>(args)...);
            ++finish_.cur;
            ++size_;
            iterator it  = finish_;
            --it;
            return it;
        }
//emplace_front返回第一个元素的迭代器
        template <class... Args>
        iterator emplace_front(Args&&... args){
            require_front_slot();
            //修正完美转发，避免value_type(mystl::forward<Args>(args)...)这种写法，
            // 因为value_type可能没有构造函数
            mystl::construct(start_.cur - 1,mystl::forward<Args>(args)...);
            --start_.cur;
            ++size_;
            return start_;
        }
//emplace返回插入位置的迭代器
        template <class... Args>
        iterator emplace(iterator pos,Args&&... args){
        return insert(pos,value_type(mystl::forward<Args>(args)...));
        }

        void swap(deque& other) noexcept{
            mystl::swap(map_,other.map_);
            mystl::swap(map_size_,other.map_size_);
            mystl::swap(start_,other.start_);
            mystl::swap(finish_,other.finish_);
            mystl::swap(size_,other.size_);
        }

        //ADL机制，找到swap函数,不用找命名空间
        friend void swap(deque& a,deque& b) noexcept{
            a.swap(b);
        }

        friend bool operator==(const deque& a,const deque& b)noexcept{
            if(a.size_ != b.size_) return false;
            auto it = a.begin();
            auto it2 = b.begin();
           for(;it != a.end() && it2 != b.end();++it,++it2)
            {
                if(!(*it == *it2)) return false;
            }
            return true;
        }
        // 
        friend bool operator!=(const deque& a,const deque& b)noexcept{
            {
               return !(a == b);
            }
        }
        //字典序比较,从头开始比较,出现第一组不相等，如果a小于b,返回true,否则返回false
        friend bool operator < (const deque& a,const deque& b)noexcept{
            auto it = a.begin();
            auto it2 = b.begin();
            for(;it != a.end()&&it2 != b.end();++it,++it2)
            {
                if(*it < *it2) return true;
                if(*it2 < *it) return false;
            }
            return it == a.end() && it2 != b.end();
        }
        friend bool operator<=(const deque& a,const deque& b)noexcept{
            return !(b < a);
        }
        friend bool operator>(const deque& a,const deque& b)noexcept{
            return b < a;
        }
        friend bool operator>=(const deque& a,const deque& b)noexcept{
            return !(a < b);
        }

        //拷贝构造
        deque(const deque& other) : map_(nullptr),map_size_(0),start_(),finish_(),size_(0)
        {
            initialize_empty();
            assign(other.begin(),other.end());
        }
        deque(deque&& other) noexcept:map_(other.map_),map_size_(other.map_size_),start_(other.start_),finish_(other.finish_),size_(other.size_)
        {
            other.map_ = nullptr;
            other.map_size_ = 0;
            other.start_ = iterator();
            other.finish_ = iterator();
            other.size_ = 0;
        }

        //拷贝赋值
        deque& operator=(const deque& other){
            if(&other == this) return *this;
            clear();
            assign(other.begin(),other.end());
            return *this;
        }
        //移动赋值
        deque& operator=(deque&& other) noexcept{
            if(this == &other) return *this;
            clear();
            destroy_map_and_nodes();
            map_ = other.map_;
            map_size_ = other.map_size_;
            start_ = other.start_;
            finish_ = other.finish_;
            size_ = other.size_;
            other.map_ = nullptr;
            other.map_size_ = 0;
            other.start_ = iterator();
            other.finish_ = iterator();
            other.size_ = 0;
            return *this;
        }
        //初始化列表赋值
        deque(std::initializer_list<value_type> ilist) : map_(nullptr),map_size_(0),start_(),finish_(),size_(0)
        {
            initialize_empty();
            assign(ilist.begin(),ilist.end());
        }
        deque& operator=(std::initializer_list<value_type> ilist){
            clear();
            assign(ilist.begin(),ilist.end());
            return *this;
        }

        };//class deque
    }//namespace mystl
    
    #endif