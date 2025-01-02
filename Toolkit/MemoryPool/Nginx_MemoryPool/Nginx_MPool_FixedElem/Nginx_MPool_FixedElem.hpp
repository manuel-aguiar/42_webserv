/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_FixedElem.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/03 11:55:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef NGINX_MPOOL_FIXEDELEM_HPP
# define NGINX_MPOOL_FIXEDELEM_HPP

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>

template <typename T>
class Nginx_MPool_FixedElem;

# include "Nginx_MPool_FixedElem.tpp"

#endif