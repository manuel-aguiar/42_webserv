/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackCircularQueue.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:04:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 12:51:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKCIRCULARQUEUE_HPP

# define STACKCIRCULARQUEUE_HPP

#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class StackCircularQueue;

# include "StackCircularQueue.tpp"

#endif