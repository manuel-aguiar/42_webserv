/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SlabAllocator.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 23:43:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXEDBLOCK_POOLALLOCATOR_HPP

# define FIXEDBLOCK_POOLALLOCATOR_HPP

// C++ headers
# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>


// Project headers
template <typename T, typename MemoryPool>
class SlabAllocator;

# include "SlabAllocator.tpp"

#endif