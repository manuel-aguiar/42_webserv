/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventGetSetAccess.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 17:02:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"

// getters

t_fd						Event::getFd() const
{
	return (m_fd);
}

const t_ptr_event_data		Event::getData() const
{
	return (m_data);
}

const t_func_event_handler	Event::getHandler() const
{
	return (m_handler);
}

int							Event::getFlags() const
{
	return (m_flags);
}

// setters

void	Event::setFd(const t_fd fd)
{
	m_fd = fd;
}

void	Event::setData(const t_ptr_event_data data)
{
	m_data = data;
}

void	Event::setHandler(const t_func_event_handler handler)
{
	m_handler = handler;
}

void	Event::setFlags(int flags)
{
	m_flags = flags;
}

void	Event::setFd_Data_Handler_Flags(const t_fd fd, const t_ptr_event_data data, const t_func_event_handler function, int flags)
{
	m_fd = fd;
	m_data = data;
	m_handler = function;
	m_flags = flags;
}

// accessors
t_ptr_event_data    Event::accessData()
{
	return (m_data);
}

t_func_event_handler    Event::accessHandler()
{
	return (m_handler);
}
