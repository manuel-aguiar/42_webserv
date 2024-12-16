/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventGetSetAccess.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 15:59:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"

// getters

t_fd						Event::getFd() const
{
	return (m_fd);
}

t_ptr_event_data		Event::getData() const
{
	return (m_data);
}

t_func_event_handler	Event::getHandler() const
{
	return (m_handler);
}

int							Event::getMonitoredFlags() const
{
	return (m_monitoredFlags);
}

int							Event::getTriggeredFlags() const
{
	return (m_monitoredFlags);
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

void	Event::setMonitorFlags(int flags)
{
	m_monitoredFlags = flags;
}

void	Event::setTriggeredFlags(int flags)
{
	m_triggeredFlags = flags;
}

void	Event::setFd_Data_Handler_Flags(const t_fd fd, const t_ptr_event_data data, const t_func_event_handler function, int flags)
{
	m_fd = fd;
	m_data = data;
	m_handler = function;
	m_monitoredFlags = flags;
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
