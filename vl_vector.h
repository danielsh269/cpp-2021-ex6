#ifndef CPP_2021_EX6_VL_VECTOR_H
#define CPP_2021_EX6_VL_VECTOR_H

#include <iostream>
#include <cmath>
#include <exception>
template <class T, size_t StaticCapacity = 16UL>
class vl_vector
{
private:

    int vec_size;
    size_t vec_capacity;

    template<class InputIterator1, class InputIterator2>
    int distance(InputIterator1 first, InputIterator2 last)
    {
        InputIterator1 temp = first;
        int k = 0;
        for(auto it = temp; it != last; ++it)
        {
            k++;
        }
        return k;
    }

public:
    T static_data[StaticCapacity];
    T* dynamic_data;

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef const T* const_pointer;
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

        if (this->vec_size > (int)StaticCapacity)
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

        for(auto i = temp; i != last; ++i)
        {
            this->vec_size++;
        }

        if (this->vec_size <= (int)StaticCapacity)
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
        this->vec_size = (int)count;

        if (this->vec_size <= (int)StaticCapacity)
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
        if (this->vec_size > (int)StaticCapacity)
        {
            delete[] this->dynamic_data;
        }

    }

    int size() const
    {
        return this->vec_size;
    }

    size_t capacity() const
    {
        return this->vec_capacity;
    }

    bool empty() const
    {
        return this->vec_size == 0;
    }

    T at(int index) const
    {
        if (index < 0 || index >= this->vec_size)
        {
            throw std::out_of_range("bad index");
        }

        if (this->vec_size > (int)StaticCapacity)
        {
            return this->dynamic_data[index];
        }

        return this->static_data[index];
    }

    void push_back(const T& v)
    {
        if (this->vec_size < (int)StaticCapacity)
        {
            std::fill(this->static_data + this->vec_size, this->static_data + this->vec_size + 1, v);
            this->vec_size++;
        }
        else if(this->vec_size == (int)StaticCapacity)
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
            if (this->vec_size > (int)this->vec_capacity)
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
        if (this->vec_size < (int)StaticCapacity)
        {
            std::move_backward(position, position + this->distance(position, this->data() + this->vec_size), this->static_data + this->vec_size + 1);
            int k = this->distance(this->data(), position);
            std::fill(this->data() + k, this->data() + k + 1, v);
            this->vec_size++;
        }
        else if(this->vec_size == (int)StaticCapacity)
        {
            this->vec_size++;
            this->vec_capacity = ceil((3 * this->vec_size) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::move(this->static_data, this->static_data + StaticCapacity, this->dynamic_data);
            int pos = this->distance(this->static_data, position);
            std::move_backward(this->dynamic_data + pos, this->dynamic_data + this->vec_size - 1, this->dynamic_data +
            this->vec_size);
            std::fill(this->dynamic_data + pos, this->dynamic_data + pos + 1, v);
            return this->dynamic_data + pos;
        }
        else
        {
            this->vec_size++;
            if (this->vec_size > (int)this->vec_capacity)
            {
                this->vec_capacity = ceil((3 * this->vec_size) / 2);
                T* temp = this->dynamic_data;
                this->dynamic_data = new T[this->vec_capacity];
                std::copy(temp, temp + this->vec_size - 1, this->dynamic_data);
                std::move_backward(position, position + this->distance(position,temp + this->vec_size - 1), this->dynamic_data + this->vec_size);
                int k = this->distance(temp, position);
                std::fill(this->data() + k, this->data() + k + 1, v);
                delete[] temp;
            }
            else
            {
                std::move_backward(position, position + this->distance(position, this->dynamic_data + this->vec_size - 1), this->dynamic_data + this->vec_size);
                int k = this->distance(this->data(), position);
                std::fill(this->data() + k, this->data() + k + 1, v);
            }
        }

        return position;
    }

    template <class InputIterator1, class InputIterator2> InputIterator1 insert(InputIterator1 position, InputIterator2
            first, InputIterator2 last)
    {
        int k = 0;
        InputIterator2 temp = first;
        for(auto i = temp; i != last; i++)
        {
            k++;
        }


        if (this->vec_size + k <= (int)StaticCapacity)
        {
            std::move_backward(position, position + this->distance(position, this->static_data + this->vec_size), this->static_data + this->vec_size + k);
            int pos = this->distance(this->data(), position);
            std::copy(first, last, this->data() + pos);
            this->vec_size += k;
        }
        else if (this->vec_size <= (int)StaticCapacity && this->vec_size + k > (int)StaticCapacity)
        {
            this->vec_capacity = ceil((3 * (this->vec_size + k)) / 2);
            this->dynamic_data = new T[this->vec_capacity];
            std::move(this->static_data, this->static_data + this->vec_size, this->dynamic_data);
            int pos = this->distance(this->static_data, position);
            std::move_backward(this->dynamic_data + pos, this->dynamic_data + this->vec_size, this->dynamic_data +
            this->vec_size + k);
            std::copy(first, last, this->dynamic_data + pos);
            this->vec_size += k;
            return this->dynamic_data + pos;

        }
        else
        {
            if (this->vec_size + k > (int)this->vec_capacity)
            {
                this->vec_capacity = ceil((3 * (this->vec_size + k)) / 2);
                T* temp2 = this->dynamic_data;
                this->dynamic_data = new T[this->vec_capacity];
                std::copy(temp2, temp2 + this->vec_size, this->dynamic_data);
                std::move_backward(position, position + this->distance(position,this->dynamic_data + this->vec_size), this->dynamic_data + this->vec_size + k);

                int pos = this->distance(this->data(), position);
                std::copy(first, last, this->data() + pos);
                this->vec_size += k;
            }
            else
            {
                std::move_backward(position, position + this->distance(position, this->dynamic_data + this->vec_size), this->dynamic_data + this->vec_size + k);
                int pos = this->distance(this->data(), position);
                std::copy(first, last, this->data() + pos);
                this->vec_size += k;
            }
        }
        return position;
    }

    void pop_back() {
        if (this->vec_size == 0) {
            return;
        }

        if (this->vec_size - 1 == (int)StaticCapacity) {
            std::move(this->dynamic_data, this->dynamic_data + this->vec_size - 1, this->static_data);
            this->vec_capacity = StaticCapacity;
        }

        this->vec_size--;
    }

    template <class InputIterator> InputIterator erase(InputIterator position)
    {

        int pos = 0;
        if (this->vec_size - 1 == (int)StaticCapacity)
        {
            pos = this->distance(this->dynamic_data, position);
            std::move(this->dynamic_data, position, this->static_data);
            std::move(position + 1, this->dynamic_data + this->vec_size, this->static_data + pos);
            this->vec_capacity = StaticCapacity;
            this->vec_size--;
            return this->static_data + pos;
        }

        else if(this->vec_size <= (int)StaticCapacity)
        {
            std::move_backward(position + 1, position + this->distance(position, this->static_data + this->vec_size), this->static_data + this->vec_size -
            1);
            this->vec_size--;
            return position;
        }
        else
        {
            std::move_backward(position + 1, position + this->distance(position, this->dynamic_data + this->vec_size), this->dynamic_data + this->vec_size
            - 1);
            this->vec_size--;
            return position;
        }
    }

    template <class InputIterator> InputIterator erase(InputIterator first, InputIterator last)
    {

        int k = this->distance(first, last);
        if (this->vec_size <= (int)StaticCapacity)
        {
            int pos = this->distance(this->static_data, first);
            std::move_backward(last, this->static_data + this->vec_size, this->static_data + this->vec_size -
            k);
            this->vec_size -= k;
            return this->static_data + pos;
        }
        if (this->vec_size - k <= (int)StaticCapacity)
        {
            int pos = this->distance(this->dynamic_data, first);
            std::move(this->dynamic_data, first, this->static_data);
            std::move(last, this->dynamic_data + this->vec_size, this->static_data + pos);
            this->vec_capacity = StaticCapacity;
            this->vec_size -= k;
            return this->static_data + pos;
        }

        else
        {
            int pos = this->distance(this->dynamic_data, first);
            std::move_backward(last , this->dynamic_data + this->vec_size, this->dynamic_data + this->vec_size - k);
            this->vec_size -= k;
            return this->dynamic_data + pos;
        }
    }

    void clear()
    {
        if (this->vec_size <= (int)StaticCapacity)
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

    T* data()
    {
        if (this->vec_size <= (int)StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    const T* data() const
    {
        if (this->vec_size <= (int)StaticCapacity)
        {
            return this->static_data;
        }

        return this->dynamic_data;
    }

    bool contains(const T& v) const
    {

        for(int i = 0; i < this->vec_size; i++)
        {
            if (this->operator[](i) == v)
                return true;
        }
        return false;
    }

    vl_vector& operator=(const vl_vector& vec)
    {
        if (this == &vec)
        {
            return *this;
        }

        if (vec.vec_size > (int)StaticCapacity)
        {
            if (this->vec_size > (int)StaticCapacity)
            {
                delete[] this->dynamic_data;
            }
            this->dynamic_data = new T[vec.capacity()];
            std::copy(vec.data(), vec.data() + vec.size(), this->dynamic_data);
        }
        else
        {
            if (this->vec_size > (int)StaticCapacity)
            {
                delete[] this->dynamic_data;
            }
            std::copy(vec.data(), vec.data() + vec.size(), this->static_data);
        }
        this->vec_capacity = vec.capacity();
        this->vec_size = vec.size();
        return *this;
    }

    const T& operator[](int index) const
    {
        const T* temp;
        if (this->vec_size <= (int)StaticCapacity)
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
        if (this->vec_size <= (int)StaticCapacity)
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
            if (this->operator[](i) != other[i])
                return false;
        }

        return true;
    }

    bool operator==(const vl_vector& other) const
    {
        if (this->vec_size != other.size())
            return false;

        for (int i = 0; i < this->vec_size; i++)
        {
            if (this->data()[i] != other.data()[i])
                return false;
        }

        return true;
    }

    bool operator!=(const vl_vector& other)
    {
        return !(this->operator==(other));
    }

    bool operator!=(const vl_vector& other) const
    {
        return !(this->operator==(other));
    }

    iterator begin()
    {
        return this->data();
    }

    iterator end()
    {
        return this->data() + this->vec_size;
    }

    const_iterator begin() const
    {
        return this->data();

    }

    const_iterator end() const
    {
        return this->data() + this->vec_size;
    }
    const_iterator cbegin() const
    {
        return this->data();
    }

    const_iterator cend() const
    {
        return this->data() + this->vec_size;
    }

    std::reverse_iterator<pointer> rbegin()
    {
        return std::make_reverse_iterator(this->end());
    }

    std::reverse_iterator<pointer> rend()
    {
       return std::make_reverse_iterator(this->begin());
    }

    const std::reverse_iterator<const_pointer> rbegin() const
    {
        return std::make_reverse_iterator(this->end());
    }

    const std::reverse_iterator<const_pointer> rend() const
    {
        return std::make_reverse_iterator(this->begin());

    }

    const std::reverse_iterator<const_pointer> crbegin() const
    {
        return std::make_reverse_iterator(this->cend());

    }

    const std::reverse_iterator<const_pointer> crend() const
    {
        return std::make_reverse_iterator(this->cbegin());
    }


};



#endif //CPP_2021_EX6_VL_VECTOR_H
