/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 09:07:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

#include <cstring>
#include <cassert>

template <typename T, typename ElemCount>
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

        T& operator[]  (const size_t index)
        {
            assert(index < ElemCount);

            return (reinterpret_cast<T*>(array)[index]);
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