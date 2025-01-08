/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedBlock_MemoryPool.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:09:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef FIXEDBLOCK_MEMORYPOOL_H

# define FIXEDBLOCK_MEMORYPOOL_H

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>

template <typename T>
class FixedBlock_MemoryPool;

# include "FixedBlock_MemoryPool.tpp"

#endif