#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H

#include <cstddef>
#include <iterator>
#include <ostream>
#include <type_traits>

namespace mystl {

// 迭代器类型标签（使用标准库的标签）
using input_iterator_tag = std::input_iterator_tag;
using output_iterator_tag = std::output_iterator_tag;
using forward_iterator_tag = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;

// 迭代器基类模板
template<typename Category, typename T, typename Distance = ptrdiff_t,
         typename Pointer = T*, typename Reference = T&>
struct iterator {
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};

// 迭代器萃取器
template<typename Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type         value_type;
    typedef typename Iterator::difference_type    difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

// 指针特化版本
template<typename T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef ptrdiff_t                  difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

// const 指针特化版本
template<typename T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef ptrdiff_t                  difference_type;
    typedef const T*                   pointer;
    typedef const T&                   reference;
};

// 迭代器类型萃取
template<typename Iterator>
struct iterator_category {
    typedef typename iterator_traits<Iterator>::iterator_category type;
};

template<typename Iterator>
struct iterator_value_type {
    typedef typename iterator_traits<Iterator>::value_type type;
};

template<typename Iterator>
struct iterator_difference_type {
    typedef typename iterator_traits<Iterator>::difference_type type;
};

template<typename Iterator>
struct iterator_pointer {
    typedef typename iterator_traits<Iterator>::pointer type;
};

template<typename Iterator>
struct iterator_reference {
    typedef typename iterator_traits<Iterator>::reference type;
};

// 距离计算的分发函数（先声明）
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag);

template<typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
distance_dispatch(RandomAccessIterator first, RandomAccessIterator last, 
                  random_access_iterator_tag);

// 迭代器距离计算
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return distance_dispatch(first, last, category());
}

// 距离计算的分发函数实现
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template<typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
distance_dispatch(RandomAccessIterator first, RandomAccessIterator last, 
                  random_access_iterator_tag) {
    return last - first;
}

// 前进的分发函数（先声明）
template<typename InputIterator, typename Distance>
void advance_dispatch(InputIterator& it, Distance n, input_iterator_tag);

template<typename BidirectionalIterator, typename Distance>
void advance_dispatch(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag);

template<typename RandomAccessIterator, typename Distance>
void advance_dispatch(RandomAccessIterator& it, Distance n, random_access_iterator_tag);

// 迭代器前进
template<typename InputIterator, typename Distance>
void advance(InputIterator& it, Distance n) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    advance_dispatch(it, n, category());
}

// 前进的分发函数实现
template<typename InputIterator, typename Distance>
void advance_dispatch(InputIterator& it, Distance n, input_iterator_tag) {
    while (n--) {
        ++it;
    }
}

template<typename BidirectionalIterator, typename Distance>
void advance_dispatch(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--) {
            ++it;
        }
    } else {
        while (n++) {
            --it;
        }
    }
}

template<typename RandomAccessIterator, typename Distance>
void advance_dispatch(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
    it += n;
}

// 反向迭代器
template<typename Iterator>
class reverse_iterator {
public:
    typedef Iterator                                            iterator_type;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type      value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer         pointer;
    typedef typename iterator_traits<Iterator>::reference      reference;

private:
    Iterator current;

public:
    // 构造函数
    reverse_iterator() : current() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    reverse_iterator(const reverse_iterator& other) : current(other.current) {}

    // 赋值操作符
    reverse_iterator& operator=(const reverse_iterator& other) {
        current = other.current;
        return *this;
    }

    // 获取底层迭代器
    iterator_type base() const {
        return current;
    }

    // 解引用操作符
    reference operator*() const {
        Iterator tmp = current;
        return *--tmp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    // 前置递增
    reverse_iterator& operator++() {
        --current;
        return *this;
    }

    // 后置递增
    reverse_iterator operator++(int) {
        reverse_iterator tmp = *this;
        --current;
        return tmp;
    }

    // 前置递减
    reverse_iterator& operator--() {
        ++current;
        return *this;
    }

    // 后置递减
    reverse_iterator operator--(int) {
        reverse_iterator tmp = *this;
        ++current;
        return tmp;
    }

    // 随机访问操作
    reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current - n);
    }

    reverse_iterator& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current + n);
    }

    reverse_iterator& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

// 反向迭代器的比较操作
template<typename Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return lhs.base() == rhs.base();
}

template<typename Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(lhs == rhs);
}

template<typename Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
    return rhs.base() < lhs.base();
}

template<typename Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(rhs < lhs);
}

template<typename Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
    return rhs < lhs;
}

template<typename Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(lhs < rhs);
}

template<typename Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
          const reverse_iterator<Iterator>& rhs) {
    return rhs.base() - lhs.base();
}

template<typename Iterator>
reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n,
          const reverse_iterator<Iterator>& it) {
    return it + n;
}

// 插入迭代器基类
template<typename Container>
class insert_iterator {
protected:
    Container* container;
    typename Container::iterator iter;

public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    insert_iterator(Container& c, typename Container::iterator i)
        : container(&c), iter(i) {}

    insert_iterator& operator=(const typename Container::value_type& value) {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }

    insert_iterator& operator*() {
        return *this;
    }

    insert_iterator& operator++() {
        return *this;
    }

    insert_iterator& operator++(int) {
        return *this;
    }
};

// back_insert_iterator
template<typename Container>
class back_insert_iterator {
protected:
    Container* container;

public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    explicit back_insert_iterator(Container& c) : container(&c) {}

    back_insert_iterator& operator=(const typename Container::value_type& value) {
        container->push_back(value);
        return *this;
    }

    back_insert_iterator& operator*() {
        return *this;
    }

    back_insert_iterator& operator++() {
        return *this;
    }

    back_insert_iterator& operator++(int) {
        return *this;
    }
};

// front_insert_iterator
template<typename Container>
class front_insert_iterator {
protected:
    Container* container;

public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    explicit front_insert_iterator(Container& c) : container(&c) {}

    front_insert_iterator& operator=(const typename Container::value_type& value) {
        container->push_front(value);
        return *this;
    }

    front_insert_iterator& operator*() {
        return *this;
    }

    front_insert_iterator& operator++() {
        return *this;
    }

    front_insert_iterator& operator++(int) {
        return *this;
    }
};

// 辅助函数
template<typename Container>
back_insert_iterator<Container> back_inserter(Container& c) {
    return back_insert_iterator<Container>(c);
}

template<typename Container>
front_insert_iterator<Container> front_inserter(Container& c) {
    return front_insert_iterator<Container>(c);
}

template<typename Container>
insert_iterator<Container> inserter(Container& c, typename Container::iterator i) {
    return insert_iterator<Container>(c, i);
}

// 流迭代器
template<typename T>
class istream_iterator {
private:
    std::istream* stream;
    T value;
    bool end_marker;

public:
    typedef input_iterator_tag iterator_category;
    typedef T                  value_type;
    typedef ptrdiff_t          difference_type;
    typedef const T*           pointer;
    typedef const T&           reference;

    istream_iterator() : stream(nullptr), end_marker(true) {}

    istream_iterator(std::istream& s) : stream(&s), end_marker(false) {
        read();
    }

    istream_iterator(const istream_iterator& other)
        : stream(other.stream), value(other.value), end_marker(other.end_marker) {}

    reference operator*() const {
        return value;
    }

    pointer operator->() const {
        return &value;
    }

    istream_iterator& operator++() {
        read();
        return *this;
    }

    istream_iterator operator++(int) {
        istream_iterator tmp = *this;
        read();
        return tmp;
    }

    bool operator==(const istream_iterator& other) const {
        return (end_marker == other.end_marker) && 
               (end_marker || stream == other.stream);
    }

    bool operator!=(const istream_iterator& other) const {
        return !(*this == other);
    }

private:
    void read() {
        if (stream && !(*stream >> value)) {
            end_marker = true;
            stream = nullptr;
        }
    }
};

template<typename T>
class ostream_iterator {
private:
    std::ostream* stream;
    const char* separator;

public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    ostream_iterator(std::ostream& s) : stream(&s), separator(nullptr) {}

    ostream_iterator(std::ostream& s, const char* sep)
        : stream(&s), separator(sep) {}

    ostream_iterator& operator=(const T& value) {
        *stream << value;
        if (separator) {
            *stream << separator;
        }
        return *this;
    }

    ostream_iterator& operator*() {
        return *this;
    }

    ostream_iterator& operator++() {
        return *this;
    }

    ostream_iterator& operator++(int) {
        return *this;
    }
};

} // namespace mystl

#endif // MYTINYSTL_ITERATOR_H
