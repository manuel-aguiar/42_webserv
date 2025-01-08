/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapSlotArray.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:27:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 16:36:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPSLOTARRAY_HPP

# define HEAPSLOTARRAY_HPP

# include <cstddef>
# include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class HeapSlotArray;

# include "HeapSlotArray.tpp"

#endif