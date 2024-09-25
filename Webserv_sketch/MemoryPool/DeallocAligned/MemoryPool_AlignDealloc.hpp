/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPool_AlignDealloc.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:16:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 11:58:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_POOL_DEALLOC_HPP

# define MEMORY_POOL_DEALLOC_HPP

#include <limits.h>
#include <stddef.h>
#include <cstring>
#include <iostream>

template <typename T, size_t BlockSize = 4096, size_t StartingBlocks = 0, size_t SpareBlocks = 0>
class MemoryPool_AlignDealloc;

# include "MemoryPool_AlignDealloc.tpp"

#endif