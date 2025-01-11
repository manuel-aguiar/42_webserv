/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heap_MemoryPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:49:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Heap_MemoryPool_HPP

# define Heap_MemoryPool_HPP

# include <memory>
template <typename Allocator = std::allocator<unsigned char> >
class Heap_MemoryPool;

# include "Heap_MemoryPool.tpp"

#endif