/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:41:30 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 11:12:58 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_HPP

# define LIST_HPP

#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class List;

# include "List.tpp"

#endif
