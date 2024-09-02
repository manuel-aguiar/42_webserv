/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollEventManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:28:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/02 09:46:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLEVENTMANAGER_HPP

# define EPOLLEVENTMANAGER_HPP

# include "IEventManager.hpp"

# include <sys/epoll.h>

class EPoll : public IEventManager
{
    
};


#endif