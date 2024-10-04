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
class HeapArray
{
    public:

        HeapArray() : _array(NULL), _size(0)
        {
        };

        HeapArray(size_t size) : _array(reinterpret_cast<T*>(new t_byte [sizeof(T) * size])), _size(size)
        {
        };

        ~HeapArray()
        {
            delete [] (reinterpret_cast<t_byte*>(_array));
        };

        HeapArray(const HeapArray &other) : _array(reinterpret_cast<T*>(new t_byte [sizeof(T) * other._size])), _size(other._size)
        {
            std::memcpy(reinterpret_cast<t_byte*>(_array), 
                        reinterpret_cast<t_byte*>(other._array), 
                        sizeof(T) * _size);
        };

        HeapArray &operator=(const HeapArray &other)
        {
            if (this == &other)
                return (*this);
            if (_size != other._size)
            {
                if (_array)
                    delete [] (reinterpret_cast<t_byte*>(_array));
                _size = other._size;
                _array = reinterpret_cast<T*>(new t_byte [sizeof(T) * _size]);
            }
            std::memcpy(reinterpret_cast<t_byte*>(_array), 
                        reinterpret_cast<t_byte*>(other._array), 
                        sizeof(T) * _size);
            return (*this);
        };

        T& operator[](const size_t index)
        {
            assert(index < _size);
            return *(reinterpret_cast<T*>(&_array[index]));
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