/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 21:41:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 22:44:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP

# include <memory>

typedef unsigned char t_byte;

template < typename T = t_byte, typename Allocator = std::allocator<t_byte> >
class Impl_Nginx_MemoryPool;

typedef Impl_Nginx_MemoryPool<> Nginx_MemoryPool;

# include "Nginx_MemoryPool.tpp"


#endif