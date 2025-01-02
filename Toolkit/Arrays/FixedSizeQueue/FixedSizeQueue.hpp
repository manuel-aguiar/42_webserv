/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedSizeQueue.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:04:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 13:37:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXEDSIZEQUEUE_HPP

# define FIXEDSIZEQUEUE_HPP

#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class FixedSizeQueue;

# include "FixedSizeQueue.tpp"

#endif