/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapFixedBlock.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:17:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPFIXEDBLOCK_HPP

# define HEAPFIXEDBLOCK_HPP

# include <memory>
template <typename Allocator = std::allocator<unsigned char> >
class HeapFixedBlock;

# include "HeapFixedBlock.tpp"

#endif