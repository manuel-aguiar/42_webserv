/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Impl_Stack_MemoryPool.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 22:49:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:50:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Impl_Stack_MemoryPool_HPP

# define Impl_Stack_MemoryPool_HPP

# include <cstddef>

typedef unsigned char t_byte;

template <size_t BlockSize, typename T = t_byte>
class Impl_Stack_MemoryPool;

template <size_t BlockSize>
class Stack_MemoryPool : public Impl_Stack_MemoryPool<BlockSize, t_byte> {};

# include "Stack_MemoryPool.tpp"

#endif