/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTracker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 18:49:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMERTRACKER_HPP

# define TIMERTRACKER_HPP


# include <memory>

template <typename T, typename U, typename Allocator = std::allocator<T> >
class TimerTracker;

# include "TimerTracker.tpp"

#endif
