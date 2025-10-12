#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H

#include<cstddef>
#include<utility>
// #include "util.h"
#include<new>
#include<memory>
// #include "iterator.h"
// #include "construct.h"
// #include "allocator.h"
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
        explicit list_node(T&& val) : prev(nullptr), next(nullptr), value(std::move(val)) {};
        
    };

    struct bidirectional_iterator_tag {};

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
      // using node_alloc     = mystl::allocator<node_type>;

      using iterator       = list_iterator<T>;
      using const_iterator = list_const_iterator<T>;

      private:
        node_type*  head_;
        node_type*  tail_;
        size_type   size_;
        // node_alloc  alloc_;

      public:
      //构造 / 析构
       list():head_(nullptr),tail_(nullptr),size_(0){}
      //  ~list(){}
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
       node_type* p = static_cast<node_type*>(::operator new(sizeof(node_type)));
       //强异常保证
       try {
        ::operator new(sizeof(node_type));
       new(p)node_type(std::forward<U>(value));
      p->prev =p->next= nullptr;
       }    //成功构造
       catch(...) {
       ::operator delete(p);
       throw;
       }
       return p;
       }

       void destroy_node(node_type* p) {
        // mystl::destroy(&p->value); //释放资源
        // alloc_.deallocate(p,1);  //释放内存
        p->~node_type();
        ::operator delete(p);
       }

       template <typename U>
       void emplace_front_impl(U&& value) {
         node_type* n = create_node(std::forward<U>(value));
         n->next = head_;
         n->prev = nullptr;
         if(tail_) head_->prev = n;
         else tail_ = n;
         head_ = n;
         ++size_;
       }
       
       template <typename U>
       void emplace_back_impl(U&& value) {
         node_type* n = create_node(std::forward<U>(value));
         n->prev = tail_;
         n->next = nullptr;
         if(head_) head_->next = n;
         else head_ = n;
         tail_ = n;
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
       reference front() const noexcept{return head_->value;}
       reference back() const noexcept{return tail_->value;}

       template<typename U>
       void push_front(const U& value) {
        emplace_front_impl(value);
       }

       template<typename U>
       void push_front(U&& value) {
        emplace_front_impl(std::forward<U>(value));
       } 

       template <typename U>
       void push_back(const U& value) {
        emplace_back_impl(value);
       }

       template <typename U>
       void push_back(U&& value) {
        emplace_back_impl(std::forward<U>(value));
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

       ~list(){clear();}


    }; //class list
} //namespace mystl
#endif