#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H

#include<cstddef>
#include "iterator.h"
#include "type_traits.h"
 #include "util.h"
#include<new>
#include<memory>
#include<initializer_list>

// #include "construct.h"
#include "allocator.h"
// #include "exceptdef.h"
// #include "type_traits.h"
// #include "functional.h"

namespace mystl {

  template <typename T>
    struct list_node{
        list_node*prev;
        list_node*next;
        T        value;
        //拷贝构造函数
        explicit list_node(const T& val) : prev(nullptr), next(nullptr), value(val) {};
        //移动构造函数
        explicit list_node(T&& val) : prev(nullptr), next(nullptr), value(mystl::move(val)) {};
        
    };

    // struct bidirectional_iterator_tag {};

    template<typename T>
    struct list_iterator {
      using self          = list_iterator<T>;
      using value_type    = T;
      using reference     = T&;
      using pointer       = T*;
      using difference_type = std::ptrdiff_t;
      using iterator_category = mystl::bidirectional_iterator_tag;

      list_node<T>*node;

      list_iterator() : node(nullptr) {};
      explicit list_iterator(list_node<T>*p) : node(p){};

      reference operator*() const {return node->value;}
      pointer operator->() const {return &(node->value);}

      //当node为nullptr(即end),此操作未完成
      self& operator++() {node = node ? node->next : nullptr;return *this;}
      self  operator++(int) {self tmp(*this); ++(*this); return tmp; }
      
      self& operator--() {node = node ? node->prev : nullptr; return *this;}
      self  operator--(int) {self tmp(*this); --(*this); return tmp;}

      bool operator==(const self& rhs) const {return node == rhs.node;}
      bool operator!=(const self& rhs) const {return node != rhs.node;}
    };

    template<typename T>
    struct list_const_iterator {
      using self              = list_const_iterator<T>;
      using reference         = const T&;
      using pointer           = const T*;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = mystl::bidirectional_iterator_tag;

      const list_node<T>*node;

      list_const_iterator() : node(nullptr) {};
      explicit list_const_iterator(const list_node<T>*p) :node(p) {};
      // 允许从非常量迭代器隐式转换为常量迭代器
      list_const_iterator(const list_iterator<T>& it) : node(it.node) {};

      reference operator*()  const{return node->value;}
      pointer   operator->() const{return &(node->value);}

      self& operator++() {node = node ? node->next : nullptr; return *this;}
      self  operator++(int) {self tmp(*this); ++(*this); return tmp;}

      self& operator--() {node = node ? node->prev : nullptr; return *this;}
      self  operator--(int) {self tmp(*this); --(*this); return tmp;}

      bool operator ==(const self& rhs) const {return node == rhs.node;}
      bool operator !=(const self& rhs) const {return node != rhs.node; }
    };//private
    //定义最小 list 主体（接口骨架）
    template <typename T>
    class list {
      public:
      using value_type     = T;
      // using allocator_type = Alloc;
      using size_type      = std::size_t;
      using reference      = value_type&;
      using const_reference = const value_type&;

      using node_type      = list_node<T>;
      using node_alloc     = mystl::allocator<node_type>;

      using iterator       = list_iterator<T>;
      using const_iterator = list_const_iterator<T>;

      private:
        node_type*  head_;
        node_type*  tail_;
        size_type   size_;
        node_alloc  alloc_;

      public:
      //构造 / 析构
       list():head_(nullptr),tail_(nullptr),size_(0){}

       //复制构造：用other的内容初始化*this
       list(const list& other) : head_(nullptr),tail_(nullptr),size_(0){
        insert(cend(),other.begin(),other.end());
       }

       list(list&& other) noexcept : head_(other.head_),tail_(other.tail_),size_(other.size_){
        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
      }

      //复制赋值：用rhs的内容赋值给*this
      list& operator=(const list& rhs) {
        if(this != &rhs) {
          list tmp(rhs);
          swap(tmp);
        }
        return *this; //类型为const list&
      }

      //移动赋值：用rhs的内容赋值给*this
      list& operator=(list&& rhs) noexcept {
        if(this != &rhs) {  //this为指向对象的指针
          clear();
          head_ = rhs.head_;
          tail_ = rhs.tail_;
          size_ = rhs.size_;
          rhs.head_ = rhs.tail_ = nullptr;
          rhs.size_ = 0;
        }
        return *this;   //类型为list&
      }
       //迭代器
       iterator begin() noexcept{return iterator(head_);}
       iterator end()   noexcept{return iterator(nullptr);}

       const_iterator begin() const noexcept{return const_iterator(head_);}
       const_iterator end()   const noexcept{return const_iterator(nullptr);}

       const_iterator cbegin() const noexcept{return const_iterator(head_);}
       const_iterator cend()   const noexcept{return const_iterator(nullptr);}

       bool empty() const noexcept{return size_ == 0;}
       size_type size() const noexcept{return size_;}

   private:
       template<typename U>
       node_type* create_node(U&& value) {
        //返回 void*，需要显式转换为 node_type*，
        //否则报 "invalid conversion from 'void' to 'node_type'"
      //  node_type* p = static_cast<node_type*>(::operator new(sizeof(node_type)));
      node_type* p = alloc_.allocate(1);
       //强异常保证
       try {
       new(p)node_type(mystl::forward<U>(value));
       p->prev =p->next= nullptr;
       }    //成功构造
       catch(...) {
       alloc_.deallocate(p,1);
       throw;
       }
       return p;
       }

       void destroy_node(node_type* p) {
        // mystl::destroy(&p->value); //释放资源
        // alloc_.deallocate(p,1);  //释放内存
        p->~node_type();
        alloc_.deallocate(p,1);
       }
       //维护为头节点
       void link_as_first(node_type* p) {
        p->prev = nullptr;
        p->next = head_;
        if(head_) head_->prev = p;
        else tail_ = p;
        head_ = p;
       }
       //维护为尾节点
       void link_as_last(node_type* node) {
        node->next = nullptr;
        node->prev = tail_;
        if(tail_) tail_->next = node;
        else      head_ = node;
        tail_ = node; 
       }
       // 在 pos 之前插入 n；若 pos==nullptr 则等价于尾插
       //不改变size_
       void splice_before(node_type* pos, node_type *node) {
        if(pos == nullptr) {link_as_last(node); return;}
        node->next =pos;
        node->prev = pos->prev;
        if(pos->prev) pos->prev->next = node;
        else
        head_ = node;
        pos->prev = node;
       }

       // 将 n 从链中断开（不删除内存），并修正首尾；不改动 size_
       void unlink(node_type* node) noexcept {
       node_type* p = node -> prev;
       node_type* q = node -> next;
       if(p) p->next = q; else head_ = q;
       if(q) q->prev = p; else tail_ = p;
       //不要把相邻节点断掉，清空的是自身指针
       node->next = node ->prev = nullptr;
      }

       template <typename U>
       void emplace_front_impl(U&& value) {
         node_type* n = create_node(mystl::forward<U>(value));
        //  n->next = head_;
        //  n->prev = nullptr;
        //  if(tail_) head_->prev = n;
        //  else tail_ = n;
        //  head_ = n;
        link_as_first(n);
         ++size_;
       }
       
       template <typename U>
       void emplace_back_impl(U&& value) {
         node_type* n = create_node(mystl::forward<U>(value));
        //  n->prev = tail_;
        //  n->next = nullptr;
        //  if(head_) head_->next = n;
        //  else head_ = n;
        //  tail_ = n;
        link_as_last(n);
         ++size_;
       }
       public:
       void swap(list& other) noexcept {
        node_type* t;
        t = head_; head_ = other.head_; other.head_ = t;
        t = tail_; tail_ = other.tail_; other.tail_ = t;

        size_type s = size_;
        size_ = other.size_;
        other.size_ = s;
       }
       public:
       reference front()  noexcept{return head_->value;}
       reference back()   noexcept{return tail_->value;}

       //const版本
      const_reference front() const noexcept{return head_->value;}
      const_reference back() const noexcept{return tail_->value;}

       template<typename U>
       void push_front(const U& value) {
        emplace_front_impl(value);
       }

       template<typename U>
       void push_front(U&& value) {
        emplace_front_impl(mystl::forward<U>(value));
       } 

       template <typename U>
       void push_back(const U& value) {
        emplace_back_impl(value);
       }

       template <typename U>
       void push_back(U&& value) {
        emplace_back_impl(mystl::forward<U>(value));
       }

       template <typename U>
       void pop_front() {
        node_type* n = head_;
        head_ = n -> next;
        if(head_) head_->prev = nullptr;
        else tail_ = nullptr;
       destroy_node(n);
        --size_;
       }

       template <typename U>
       iterator insert(const_iterator pos,U&& val) {
        node_type * n = create_node(mystl::forward<U>(val));
        splice_before(const_cast<node_type*>(pos.node),n);
        ++size_;
        return iterator(n);
       }

       template <typename U>
       iterator insert(const_iterator pos,size_type count,const U& value)
       {
          if(count == 0) return iterator(const_cast<node_type*>(pos.node));
          node_type* before = const_cast<node_type*>(pos.node);
          node_type* first_new = nullptr;
          for(size_type i = 0; i < count; ++i)
          {
            node_type* new_node = create_node(value);
            if(!first_new) first_new = new_node;
            splice_before(before,new_node);
            ++size_;
          }
          return iterator(first_new);
       }

       template <typename InputIterator,typename = 
       typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
       iterator insert(const_iterator pos,InputIterator first,InputIterator last) {
        node_type* before = const_cast<node_type*>(pos.node);
        node_type* first_new = nullptr;
        for(;first != last;++first)
        {
          node_type*new_node = create_node(*first);
          if(!first_new) first_new = new_node;
          splice_before(before,new_node);
          ++size_;
        }
        return iterator(first_new ? first_new : const_cast<node_type*>(pos.node)) ;
       }
       template <typename... Args>
       iterator emplace(const_iterator pos,Args&&... args) {
        T tmp(mystl::forward<Args>(args)...);
        return insert(pos,mystl::move(tmp));
      }

      iterator erase(const_iterator pos){
        node_type* n = const_cast<node_type*>(pos.node);
        node_type* next = n ? n->next :  nullptr;
        unlink(n);
        destroy_node(n);
        --size_;
        return iterator(next);
      }

      iterator erase(const_iterator first,const_iterator last) {
        node_type* cur = const_cast<node_type*>(first.node);
        node_type* to = const_cast<node_type*>(last.node);
        while(cur != to) {
          node_type* next = cur->next;
          unlink(cur);
          destroy_node(cur);
          --size_;
          cur = next;
        }
        return iterator(to);
      }

       void pop_back() {
        node_type* n = tail_;
        tail_ = n -> prev;
        if(tail_) tail_->next = nullptr;
        else head_ = nullptr;
       destroy_node(n);
        --size_;
       }
        
       void clear() noexcept{
        node_type* cur = head_;
        while(cur) {
          node_type* next = cur->next;
         destroy_node(cur);
          cur = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
       }

       void assign(size_type count,const T& value) {
        //clear后cend 即为begin
        clear();
        if(count == 0) return;
        insert(cend(),count,value);
       }

       template <typename InputIterator>
       void assign(InputIterator first,InputIterator last) {
       //clear后cend 即为begin
        clear();
        if(first == last) return;
        insert(cend(),first,last);
       }

       void resize(size_type count) {
        if(count == size_) return;
        if(count < size_) {
          iterator it = begin();
          for(size_type i = 0;i < count;++i) ++it;
          erase(it,cend());
          }
          else {
            insert(cend(),count - size_,T());
          }
        }

        void resize(size_type count,const T&value) 
        {
          if(count == size_) return;
          if(count < size_) {
            iterator it = begin();
            for(size_type i = 0;i < count;++i) ++it;
            erase(it,cend());
            }
            else {
              insert(cend(),count - size_,value);
            }
          }
       ~list(){clear();}

       //不动辅助函数；线性一次扫描，命中即擦除，并正确前移迭代器
        size_type remove(const T& value){
          size_type count = 0;
          for(auto it = begin(),last = end();it != last;)
          {
            if(*it == value) {
              it = erase(it);
              ++count;
            }
            else{
              ++it;
            }
          }
            return count;
          
        }
        template <typename UnaryPredicate>
        size_type remove_if(UnaryPredicate pred) {
          size_type count = 0;
          for(auto it = begin(),last = end();it != last;)
          {
            if(pred(*it)) {
              it = erase(it);
              ++count;
            }
            else {
              ++it;
            }
          }
          return count;
        }
//去重
        size_type unique() {
          size_type removed = 0;
          if(size_ < 2) return 0;
          auto prev = begin();
          auto it = prev; ++it;
          for(auto last = end();it != last;) {
            if(*prev == *it) {
              it = erase(it);
              ++removed;
            } else{
              prev = it; ++it;
            }
          }
          return removed;
        }

        template <typename BinaryPredicate>
        size_type unique(BinaryPredicate pred) {
          size_type removed = 0;
          if(size_ < 2) return 0;
          auto prev = begin();
          auto it = prev; ++it;
          for(auto last = end();it != last;)
          {
            if(pred(*prev,*it)){
              it = erase(it);
              ++removed;
            }
            else {prev = it; it++;}
          }
          return removed;
        }

        void reverse() {
          node_type* cur = head_;
          while(cur) {
            node_type *next_backup = cur->next;
            cur->next = cur->prev;
            cur->prev = next_backup;
            cur = next_backup;
          }
          node_type* tmp = head_;
          head_ = tail_;
          tail_ = tmp;
        }

        void splice(const_iterator pos,list& other) {
          if(this == &other || other.size_ == 0) return;

          size_type moved = other.size_;

          node_type* first = other.head_;
          node_type* last = other.tail_;

          other.head_ = other.tail_ = nullptr;

          node_type* before = const_cast<node_type*>(pos.node);
          if(before == nullptr) {
            // link_as_first(first);
            // link_as_last(last);
            if(tail_) 
              {tail_->next = first; first->prev = tail_;}
            else {head_ = first;}
            tail_ = last;
        }else {
            node_type* prev = before->prev;
            if(prev) prev->next = first;
            else head_ = first;
            before->prev = last;
            last->next = before;
            before->prev = last;
          }
          size_ += moved;
          other.size_ = 0;
        }

      void splice(const_iterator pos,list& other,const_iterator it) {
        if(it == other.cend()) return;
        node_type* n = const_cast<node_type*>(it.node);
//如果self == other，则不能在pos之前插入n
        if(this == &other) {   //相同对象，不能在pos之前插入n
          if(pos.node == n || pos.node == n->next) return;
        }
//从other中移除n
        other.unlink(n);
        --other.size_;
 //pos之前插入n
        splice_before(const_cast<node_type*>(pos.node),n);
        ++size_;
      }

      void splice(const_iterator pos,list& other,const_iterator first,const_iterator last) {
        if(first == last) return;

        node_type* start = const_cast<node_type*>(first.node);
        node_type* finish = const_cast<node_type*>(last.node);

        node_type* last_incl = finish ? finish->prev : other.tail_;

        if(this == &other) {
          for(node_type* cur = start;cur != finish;cur = cur->next) {
            if(cur == pos.node) return;
          }
        }

        // 从 other 脱链整段 [start, finish)
        node_type* before_start = start->prev;
        if(before_start) before_start->next = finish;
        else other.head_ = finish;

        if(finish) finish->prev = before_start;
        else other.tail_ = before_start;

        // 统计移动个数（线性计数）
        size_type moved = 0;
        for(node_type* cur = start;cur != finish;cur = cur->next) ++moved;

        // 插入到 *this 的 pos 之前（整段拼接，O(1)）
        node_type* before = const_cast<node_type*>(pos.node);
        if(before == nullptr) {
          if(tail_) {tail_->next = start;start->prev = tail_;}
          else {head_ = start;}
          tail_ = last_incl;
        } else {
          node_type* prev = before->prev;
          if(prev) prev->next = finish;
          else other.head_ = finish;
          before->prev = last_incl;
        }
        size_ += moved;
        other.size_ -= moved;
      }

      //初始化列表构造
      list(std::initializer_list<T> ilist)
      :head_(nullptr),tail_(nullptr),size_(0) {
      insert(cend(),ilist.begin(),ilist.end());
      }

      //初始化列表赋值
      list& operator=(std::initializer_list<T> ilist) {
        clear();
        insert(cend(),ilist.begin(),ilist.end());
        return *this;
      }

      bool operator==(const list& rhs) const {
        if(size_ != rhs.size_) return false;
        for(auto it1 = begin(),it2 = rhs.begin(),e1 = end();it1 != e1;++it1,++it2)
        {
          if(!(*it1 == *it2)) return false;
        }
        return true;
      }
      bool operator!=(const list& rhs) const {return !(*this == rhs);}

      bool operator<(const list& rhs) const {
        auto it1 = begin(),e1 = end();
        auto it2 = rhs.begin(),e2 = rhs.end();
        for(;it1 != e1 && it2 != e2;++it1,++it2) {
          if(*it1 < *it2) return true;
          if(*it2 < *it1) return false;
        }
        return it1 == e1 && it2 != e2;
      }
      bool operator<=(const list& rhs) const {return !(rhs < *this);}
      bool operator>(const list& rhs) const {return rhs < *this;}
      bool operator>=(const list& rhs) const {return !(*this < rhs);}
    }; //class list
    template <typename T>
    inline void swap(list<T>& a, list<T>& b) noexcept { a.swap(b); }
} //namespace mystl
#endif