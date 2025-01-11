/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heap_ObjectPool.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:35:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef Heap_ObjectPool_H

# define Heap_ObjectPool_H

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>
# include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class Heap_ObjectPool;

# include "Heap_ObjectPool.tpp"

#endif