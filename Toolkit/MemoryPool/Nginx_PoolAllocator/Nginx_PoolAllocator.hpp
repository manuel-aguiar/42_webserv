/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_PoolAllocator.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:35:06 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:25:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NGINX_POOLALLOCATOR_HPP

# define NGINX_POOLALLOCATOR_HPP

# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"

template <typename T, typename MemoryPool = Nginx_MemoryPool>
class Nginx_PoolAllocator;

# include "Nginx_PoolAllocator.tpp"

#endif