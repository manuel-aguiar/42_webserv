/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMemoryPoolDealloc.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:44:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 13:38:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_MEMORY_POOL_HPP

# define SHARED_MEMORY_POOL_HPP

# include "MemoryPoolDealloc.hpp"

template <typename T, size_t BlockSize = 16384>
class SharedMemoryPool;

# include "SharedMemoryPoolDealloc.tpp"

#endif