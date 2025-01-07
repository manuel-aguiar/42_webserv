/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_FixedElem.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 23:43:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MPOOL_FIXEDELEM_HPP

# define NGINX_MPOOL_FIXEDELEM_HPP

// C++ headers
# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>

# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"

// Project headers
template <typename T, typename MemoryPool = Nginx_MemoryPool>
class Nginx_MPool_FixedElem;

# include "Nginx_MPool_FixedElem.tpp"

#endif