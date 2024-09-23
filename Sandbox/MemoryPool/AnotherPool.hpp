/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnotherPool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/23 12:14:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANOTHERPOOL_HPP

# define ANOTHERPOOL_HPP

#include <cstdlib>
#include <cstring>

template<typename T, size_t BlockSize>
class MemoryBlock
{
    public:

        MemoryBlock() : 
        {
            next = nullptr;
            prev = nullptr;
            freeSlotsCount = (BlockSize - sizeof(MemoryBlock*)) / sizeof(T);
        }
        MemoryBlock*    _next;
        MemoryBlock*    _prev;
        size_t          _freeSlotsCount;
        char            _data[BlockSize];
};



#endif