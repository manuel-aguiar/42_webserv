/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTracker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:18:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 15:31:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMERTRACKER_HPP

# define TIMERTRACKER_HPP


# include <memory>

template <typename T, typename U, typename Allocator = std::allocator<std::pair<const T, U> > >
class TimerTracker;

# include "TimerTracker.tpp"

#endif
