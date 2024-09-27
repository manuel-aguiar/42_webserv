/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMPool_Dealloc.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:44:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:22:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAREDMPOOL_DEALLOC_HPP

# define SHAREDMPOOL_DEALLOC_HPP

# include "MemoryPool_Dealloc.hpp"

/*
    StartingBlocks -> when the pool opens, it will allocate this amount of blocks (each BlockSize bytes long)
    SpareBlocks -> the pool will only start deallocating empty blocks when the number of empty blocks exceeds this number
        -> allow the user to dealloc straight away or keep some memory for potential future allocs
*/


template <typename T, size_t BlockSize = 4096, size_t StartingBlocks = 0, size_t SpareBlocks = 0>
class SharedMPool_Dealloc;

# include "SharedMPool_Dealloc.tpp"

#endif