#ifndef CPP_2021_EX6_VL_VECTOR_H
#define CPP_2021_EX6_VL_VECTOR_H

#include <iostream>
#include <cmath>
#include <exception>
template <class T, size_t StaticCapacity>
class vl_vector
{
private:

    size_t vec_size;
    size_t vec_capacity;
public:
    T static_data[StaticCapacity];
    T* dynamic_data;

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef int difference_type;
    typedef std::random_access_iterator_tag iterator_category;
    /**
     * default constructor
     */
    vl_vector()
    {
        this->vec_size= 0;
        this->vec_capacity = StaticCapacity;
    }
    /**
     * copy constructor
     * @param other
     */
    vl_vector(vl_vector const &other)
    {
        this->vec_size = other.vec_size;
        this->vec_capacity = other.vec_capacity;

        if (this->vec_size > StaticCapacity)
        {
            this->dynamic_data = new T[this->vec_capacity];
            std::copy(other.dynamic_data, other.dynamic_data + other.vec_size, this->dynamic_data);
        }
        else
        {
            std::copy(other.static_data, other.static_data + other.vec_size, this->static_data);
        }
    }

    template <class InputIterator> vl_vector(InputIterator first, InputIterator last)
    {
        this->vec_size= 0;
        InputIterator temp = first;

        for(auto i = first; i != last; ++i)
        {
            this->vec_size++;
        }

        if (this->vec_size <= StaticCapacity)
        {
            this->vec_capacity = StaticCapacity;
            std::copy(first, last, this->static_data);
        }
        else
        {
            this->vec_capacity = ceil((3 * this->vec_size) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::copy(first, last, this->dynamic_data);
        }
    }

    vl_vector(size_t count, const T &v)
    {
        this->vec_size = count;

        if (this->vec_size <= StaticCapacity)
        {
            this->vec_capacity = StaticCapacity;
            std::fill(this->static_data, this->static_data + count, v);
        }
        else
        {
            this->vec_capacity = ceil((3 * this->vec_size) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::fill(this->dynamic_data, this->dynamic_data + count, v);
        }
    }

    ~vl_vector()
    {
        if (this->vec_size > StaticCapacity)
        {
            delete[] this->dynamic_data;
        }

    }

    size_t size()
    {
        return this->vec_size;
    }

    size_t capacity()
    {
        return this->vec_capacity;
    }

    bool empty()
    {
        return this->vec_size == 0;
    }

    T at(int index)
    {
        if (index < 0 || index >= this->vec_size)
        {
            throw "Wrong index";
        }

        if (this->vec_size > StaticCapacity)
        {
            return this->dynamic_data[index];
        }

        return this->static_data[index];
    }

    void push_back(const T& v)
    {
        if (this->vec_size < StaticCapacity)
        {
            std::fill(this->static_data + this->vec_size, this->static_data + this->vec_size + 1, v);
            this->vec_size++;
        }
        else if(this->vec_size == StaticCapacity)
        {
            this->vec_size++;
            this->vec_capacity = ceil((3 * this->vec_size) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::move(this->static_data, this->static_data + StaticCapacity, this->dynamic_data);
            std::fill(this->dynamic_data + this->vec_size - 1, this->dynamic_data + this->vec_size, v);
        }
        else
        {
            this->vec_size++;
            if (this->vec_size > this->vec_capacity)
            {
                this->vec_capacity = ceil((3 * this->vec_size) / 2);
                T* temp = this->dynamic_data;
                this->dynamic_data = new T[this->vec_capacity];
                std::copy(temp, temp + this->vec_size - 1, this->dynamic_data);
                std::fill(this->dynamic_data + this->vec_size - 1, this->dynamic_data + this->vec_size, v);
            }
            else
            {
                std::fill(this->dynamic_data + this->vec_size - 1, this->dynamic_data + this->vec_size, v);
            }
        }
    }

    template <class InputIterator> InputIterator insert(InputIterator position, const T& v)
    {
        if (this->vec_size < StaticCapacity)
        {
            std::move_backward(position, this->static_data + this->vec_size, this->static_data + this->vec_size + 1);
            std::fill(position, position + 1, v);
            this->vec_size++;
        }
        else if(this->vec_size == StaticCapacity)
        {
            this->vec_size++;
            this->vec_capacity = ceil((3 * this->vec_size) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::move(this->static_data, this->static_data + StaticCapacity, this->dynamic_data);
            int pos = std::distance(this->static_data, position);
            std::move_backward(this->dynamic_data + pos, this->dynamic_data + this->vec_size - 1, this->dynamic_data +
            this->vec_size);
            std::fill(this->dynamic_data + pos, this->dynamic_data + pos + 1, v);
            return this->dynamic_data + pos;
        }
        else
        {
            this->vec_size++;
            if (this->vec_size > this->vec_capacity)
            {
                this->vec_capacity = ceil((3 * this->vec_size) / 2);
                T* temp = this->dynamic_data;
                this->dynamic_data = new T[this->vec_capacity];
                std::copy(temp, temp + this->vec_size - 1, this->dynamic_data);
                std::move_backward(position, this->dynamic_data + this->vec_size - 1, this->dynamic_data + this->vec_size);
                std::fill(position, position + 1, v);
            }
            else
            {
                std::move_backward(position, this->dynamic_data + this->vec_size - 1, this->dynamic_data + this->vec_size);
                std::fill(position, position + 1, v);
            }
        }

        return position;
    }

    template <class InputIterator> InputIterator insert(InputIterator position, InputIterator first, InputIterator last)
    {
        int k = 0;
        InputIterator temp = first;
        for(auto i = temp; i != last; i++)
        {
            k++;
        }


        if (this->vec_size + k <= StaticCapacity)
        {
            std::move_backward(position, this->static_data + this->vec_size, this->static_data + this->vec_size + k);
            std::copy(first, last, position);
            this->vec_size += k;
        }
        else if (this->vec_size <= StaticCapacity && this->vec_size + k > StaticCapacity)
        {
            this->vec_capacity = ceil((3 * (this->vec_size + k)) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::move(this->static_data, this->static_data + this->vec_size, this->dynamic_data);
            int pos = std::distance(this->static_data, position);
            std::move_backward(this->dynamic_data + pos, this->dynamic_data + this->vec_size, this->dynamic_data +
            this->vec_size + k);
            std::copy(first, last, this->dynamic_data + pos);
            this->vec_size += k;
            return this->dynamic_data + pos;

        }
        else
        {
            if (this->vec_size + k > this->vec_capacity)
            {
                this->vec_capacity = ceil((3 * (this->vec_size + k)) / 2);
                T* temp = this->dynamic_data;
                this->dynamic_data = new T[this->vec_capacity];
                std::copy(temp, temp + this->vec_size, this->dynamic_data);
                std::move_backward(position, this->dynamic_data + this->vec_size, this->dynamic_data + this->vec_size + k);
                std::copy(first, last, position);
                this->vec_size += k;
            }
            else
            {
                std::move_backward(position, this->dynamic_data + this->vec_size, this->dynamic_data + this->vec_size + k);
                std::copy(first, last, position);
                this->vec_size += k;
            }
        }
        return position;
    }

    void pop_back() {
        if (this->vec_size == 0) {
            return;
        }

        if (this->vec_size - 1 == StaticCapacity) {
            std::move(this->dynamic_data, this->dynamic_data + this->vec_size - 1, this->static_data);
            this->vec_capacity = StaticCapacity;
        }

        this->vec_size--;
    }

    template <class InputIterator> InputIterator erase(InputIterator position)
    {

        int pos = 0;
        if (this->vec_size - 1 == StaticCapacity)
        {
            pos = std::distance(this->dynamic_data, position);
            std::move(this->dynamic_data, position, this->static_data);
            std::move(position + 1, this->dynamic_data + this->vec_size, this->static_data + pos);
            this->vec_capacity = StaticCapacity;
            this->vec_size--;
            return this->static_data + pos;
        }

        else if(this->vec_size <= StaticCapacity)
        {
            std::move_backward(position + 1, this->static_data + this->vec_size, this->static_data + this->vec_size -
            1);
            this->vec_size--;
            return position;
        }
        else
        {
            std::move_backward(position + 1, this->dynamic_data + this->vec_size, this->dynamic_data + this->vec_size
            - 1);
            this->vec_size--;
            return position;
        }
    }

    template <class InputIterator> InputIterator erase(InputIterator first, InputIterator last)
    {

        int k = std::distance(first, last);
        if (this->vec_size <= StaticCapacity)
        {
            int pos = std::distance(this->static_data, first);
            std::move_backward(last, this->static_data + this->vec_size, this->static_data + this->vec_size -
            k);
            this->vec_size -= k;
            return this->static_data + pos;
        }
        if (this->vec_size - k <= StaticCapacity)
        {
            int pos = std::distance(this->dynamic_data, first);
            std::move(this->dynamic_data, first, this->static_data);
            std::move(last, this->dynamic_data + this->vec_size, this->static_data + pos);
            this->vec_capacity = StaticCapacity;
            this->vec_size -= k;
            return this->static_data + pos;
        }

        else
        {
            int pos = std::distance(this->dynamic_data, first);
            std::move_backward(last , this->dynamic_data + this->vec_size, this->dynamic_data + this->vec_size - k);
            this->vec_size -= k;
            return this->dynamic_data + pos;
        }
    }

    void clear()
    {
        if (this->vec_size <= StaticCapacity)
        {
            this->vec_size = 0;
        }
        else
        {
            delete[] this->dynamic_data;
            this->vec_size = 0;
            this->vec_capacity = StaticCapacity;
        }
    }

    T* data() const
    {
        if (this->vec_size <= StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    bool contains(const T& v)
    {

        for(int i = 0; i < this->vec_size; i++)
        {
            if (this[i] == v)
                return true;
        }
        return false;
    }

    const T& operator[](int index) const
    {
        T* temp;
        if (this->vec_size <= StaticCapacity)
        {
            temp = this->static_data;
        }
        else
        {
            temp = dynamic_data;
        }

        return temp[index];
    }

    T& operator[](int index)
    {

        T* temp;
        if (this->vec_size <= StaticCapacity)
        {
            temp = this->static_data;
        }
        else
        {
            temp = dynamic_data;
        }

        return temp[index];
    }

    bool operator==(const vl_vector& other)
    {
        if (this->vec_size != other.size())
            return false;

        for (int i = 0; i < this->vec_size; i++)
        {
            if (this[i] != other[i])
                return false;
        }

        return true;
    }

    const bool operator==(const vl_vector& other) const
    {
        if (this->vec_size != other.size())
            return false;

        for (int i = 0; i < this->vec_size; i++)
        {
            if (this[i] != other[i])
                return false;
        }

        return true;
    }

    bool operator!=(const vl_vector& other)
    {
        return !(this == other);
    }

    const bool operator!=(const vl_vector& other) const
    {
        return !(this == other);
    }

    iterator begin()
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    iterator end()
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data + this->vec_size;
        }

        return this->dynamic_data + this->vec_size;
    }

    const_iterator begin() const
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    const_iterator end() const
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data + this->vec_size;
        }

        return this->dynamic_data + this->vec_size;
    }
    const_iterator cbegin() const
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    const_iterator cend() const
    {
        if (this->vec_size > StaticCapacity)
        {
            return this->static_data + this->vec_size;
        }

        return this->dynamic_data + this->vec_size;
    }


    class reverse_iterator
    {
    private:
        vl_vector* vec;
        pointer curr;
    public:
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        reverse_iterator(vl_vector* vec)
        {
            this->vec = vec;
            this->curr = vec[vec->vec_size - 1];
        }

        reference operator*()
        {
            return *curr;
        }

        reverse_iterator &operator++()
        {
            this->curr--;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator temp = *this;
            this->curr--;
            return temp;
        }

        reverse_iterator &operator--()
        {
            this->curr++;
            return *this;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator temp = *this;
            this->curr++;
            return temp;
        }

        pointer operator->()
        {
            return &(*curr);
        }

        reverse_iterator& operator=(const reverse_iterator& rhs)
        {
            *this = rhs;
            return *this;
        }

        reverse_iterator base() const
        {
            return *this;
        }

        reverse_iterator& operator[]( difference_type n ) const
        {
            this->curr = this->curr - n;
            return *this;
        }

        reverse_iterator operator+( difference_type n ) const
        {
            reverse_iterator temp = *this;
            temp.curr = temp.curr - n;
            return temp;
        }

        reverse_iterator& operator+=( difference_type n )
        {
            this->curr = this->curr - n;
            return *this;
        }

        reverse_iterator operator-( difference_type n ) const
        {
            reverse_iterator temp = *this;
            temp.curr = temp.curr + n;
            return temp;
        }

        reverse_iterator& operator-=( difference_type n )
        {
            this->curr = this->curr + n;
            return *this;
        }

        bool operator==(const reverse_iterator &rhs)
        {
            return this->curr == rhs.curr;
        }

        bool operator!=(const reverse_iterator &rhs)
        {
            return this->curr != rhs.curr;
        }

        bool operator<(const reverse_iterator &rhs)
        {
            return this->curr > rhs.curr;
        }

        bool operator<=(const reverse_iterator &rhs)
        {
            return this->curr >= rhs.curr;
        }

        bool operator>(const reverse_iterator &rhs)
        {
            return this->curr < rhs.curr;
        }

        bool operator>=(const reverse_iterator &rhs)
        {
            return this->curr <= rhs.curr;
        }

    };

    reverse_iterator rbegin()
    {
        return reverse_iterator(this);
    }

    reverse_iterator rend()
    {
        reverse_iterator it = reverse_iterator(this);
        it += this->vec_size;
        return it;
    }

    const reverse_iterator rbegin() const
    {
        return reverse_iterator(this);
    }

    const reverse_iterator rend() const
    {
        reverse_iterator it = reverse_iterator(this);
        it += this->vec_size;
        return it;
    }

    const reverse_iterator crbegin() const
    {
        return reverse_iterator(this);
    }

    const reverse_iterator crend() const
    {
        reverse_iterator it = reverse_iterator(this);
        it += this->vec_size;
        return it;
    }


};



#endif //CPP_2021_EX6_VL_VECTOR_H
