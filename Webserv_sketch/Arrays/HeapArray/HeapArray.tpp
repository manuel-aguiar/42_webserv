/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 09:35:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPARRAY_TPP

# define HEAPARRAY_TPP

#include <cstring>
#include <cassert>

template <typename T>
class HeapArrayProxy
{
    public:
        HeapArrayProxy(T* element) : element(element) {}

        HeapArrayProxy& operator=(const T& value)
        {
            new (element) T(value);
            return (*this);
        }

        operator T&() const
        {
            return (*element);
        }

        T* operator&()
        {
            return element;
        }

    private:
        T* element;
};


template <typename T>
class HeapArray
{
    public:
        HeapArray(size_t size = 0) : _array(reinterpret_cast<T*>(new t_byte [sizeof(T) * size])), _size(size)
        {
        };

        ~HeapArray()
        {
            delete [] (_array);
        };

        HeapArray(const HeapArray &other) : _array(reinterpret_cast<T*>(new t_byte [sizeof(T) * other._size])), _size(other._size)
        {
            std::memcpy(_array, other.array, sizeof(T) * size);
        };

        HeapArray &operator=(const HeapArray &other)
        {
            if (this == &other)
                return (*this);
            if (_size != other.size())
            {
                delete [] (_array);
                _size = other.size();
                _array = reinterpret_cast<T*>(new t_byte [sizeof(T) * _size]);
            }
            std::memcpy(_array, other.array, sizeof(T) * size);
            return (*this);
        };

        HeapArrayProxy<T> operator[](const size_t index)
        {
            assert(index < _size);
            return HeapArrayProxy<T>(reinterpret_cast<T*>(&_array[index]));
        }

        
        size_t size() const
        {
            return (_size);
        }
        
    private:

        typedef unsigned char   t_byte;
        T*          _array;
        size_t      _size;
};




#endif