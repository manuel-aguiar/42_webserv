/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPoolDealloc.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:16:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 08:35:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_POOL_DEALLOC_HPP

# define MEMORY_POOL_DEALLOC_HPP

#include <limits.h>
#include <stddef.h>
#include <cstring>
#include <iostream>

template <typename T, size_t BlockSize = 16384>
class MemoryPool;

# include "MemoryPoolDealloc.tpp"

#endif