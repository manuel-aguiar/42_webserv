/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 11:06:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

#include <cstring>
#include <cassert>


template <typename T, size_t ElemCount>
class StackArray
{
    public:
        StackArray() {};
        ~StackArray() {};
        StackArray(const StackArray &other)
        {
            std::memcpy(_array, other._array, sizeof(T) * ElemCount);
        };

        StackArray &operator=(const StackArray &other)
        {
            if (this == &other)
                return (*this);
            std::memcpy(_array, other._array, sizeof(T) * ElemCount);
            return (*this);
        };

        T& operator[](const size_t index)
        {
            assert(index < ElemCount);
            return *(reinterpret_cast<T*>(&_array[index * sizeof(T)]));
        }

        
        size_t size() const
        {
            return (ElemCount);
        }
        
    private:
        typedef unsigned char t_byte;
        t_byte  _array[sizeof(T) * ElemCount];
};




#endif