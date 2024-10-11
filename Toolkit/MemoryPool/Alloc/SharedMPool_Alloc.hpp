/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMPool_Alloc.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:44:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:22:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAREDMPOOL_ALLOC_HPP

# define SHAREDMPOOL_ALLOC_HPP

# include "MemoryPool_Alloc.hpp"

template <typename T, size_t BlockSize = 4096>
class SharedMPool_Alloc;

# include "SharedMPool_Alloc.tpp"

#endif