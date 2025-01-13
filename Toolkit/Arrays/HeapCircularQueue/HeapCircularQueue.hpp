/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapCircularQueue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:04:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 22:33:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPCIRCULARQUEUE_HPP

# define HEAPCIRCULARQUEUE_HPP

#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class HeapCircularQueue;

# include "HeapCircularQueue.tpp"

#endif