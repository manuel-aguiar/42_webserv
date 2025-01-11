/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedBlock_MemoryPool.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 23:40:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef FIXEDBLOCK_MEMORYPOOL_H

# define FIXEDBLOCK_MEMORYPOOL_H

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>
# include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class FixedBlock_MemoryPool;

# include "FixedBlock_MemoryPool.tpp"

#endif