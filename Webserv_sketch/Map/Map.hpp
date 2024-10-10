/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:33:54 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 16:36:43 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP

# define MAP_HPP

#include <functional>
#include <memory>

template <
	typename Key,
	typename Value,
	typename Compare = std::less<Key>,
	typename Allocator = std::allocator<std::pair<const Key, Value> >
>
class Map;

# include "Map.tpp"

#endif
