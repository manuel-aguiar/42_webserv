/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMemoryPoolDeallocMemAlign.hpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:44:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 08:11:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_MEMORY_POOL_HPP

# define SHARED_MEMORY_POOL_HPP

# include "MemoryPoolDeallocMemAlign.hpp"

template <typename T, size_t BlockSize = 32768>
class SharedMemoryPool;

# include "SharedMemoryPoolDeallocMemAlign.tpp"

#endif