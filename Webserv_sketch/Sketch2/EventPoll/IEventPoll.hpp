/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventPoll.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:17:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 11:17:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTPOLL_HPP

# define IEVENTPOLL_HPP

class IEventPoll {
public:
    virtual void poll() = 0;
};




#endif