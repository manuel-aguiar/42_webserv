/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMemoryPool.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:44:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 13:35:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_MEMORY_POOL_HPP

# define SHARED_MEMORY_POOL_HPP

# include "MemoryPool.hpp"
# include "MemoryPoolDealloc.hpp"
# include "MemoryPoolDeallocMemAlign.hpp"

template <typename T, size_t BlockSize = 16384>
class SharedMemoryPool;

# include "SharedMemoryPool.tpp"

#endif