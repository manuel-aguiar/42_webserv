/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 09:35:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

#include <cstring>
#include <cassert>

template <typename T>
class StackArrayProxy
{
    public:
        StackArrayProxy(T* element) : element(element) {}

        StackArrayProxy& operator=(const T& value)
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

template <typename T, size_t ElemCount>
class StackArray
{
    public:
        StackArray() {};
        ~StackArray() {};
        StackArray(const StackArray &other)
        {
            std::memcpy(array, other.array, sizeof(T) * ElemCount);
        };

        StackArray &operator=(const StackArray &other)
        {
            if (this == &other)
                return (*this);
            std::memcpy(array, other.array, sizeof(T) * ElemCount);
            return (*this);
        };

        StackArrayProxy<T> operator[](const size_t index)
        {
            assert(index < ElemCount);
            return StackArrayProxy<T>(reinterpret_cast<T*>(&array[index * sizeof(T)]));
        }

        
        size_t size() const
        {
            return (ElemCount);
        }
        
    private:
        typedef unsigned char t_byte;
        t_byte  array[sizeof(T) * ElemCount];
};




#endif