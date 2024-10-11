/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:14:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/08 09:55:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_VECTOR_HPP

# define WEBSERVER_VECTOR_HPP

#include <memory>
template <typename T, typename Allocator = std::allocator<T> >
class DynArray;

# include "DynArray.tpp"

#endif