/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapSlab.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:27:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 16:36:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPSLAB_HPP

# define HEAPSLAB_HPP

# include <cstddef>
# include <memory>

template <size_t nodeSize, typename Allocator = std::allocator<unsigned char> >
class HeapSlab;

# include "HeapSlab.tpp"

#endif