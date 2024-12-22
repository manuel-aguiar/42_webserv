/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventGetSetAccess.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/22 10:26:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"

// getters

t_fd						Event::getFd() const
{
	return (m_fd);
}

const Callback&				Event::getCallback() const
{
	return (m_callback);
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

void	Event::setMonitorFlags(int flags)
{
	m_monitoredFlags = flags;
}

void	Event::setTriggeredFlags(int flags)
{
	m_triggeredFlags = flags;
}

void	Event::setFdFlags(const t_fd fd, int flags)
{
	m_fd = fd;
	m_monitoredFlags = flags;
}

void	Event::setCallback(const t_ptr_callback_data data, const t_func_callback_handler handler)
{
	m_callback.setData(data);
	m_callback.setHandler(handler);
}

void	Event::setFdFlagsCallback(const t_fd fd, int flags, const t_ptr_callback_data data, const t_func_callback_handler handler)
{
	m_fd = fd;
	m_monitoredFlags = flags;
	m_callback.setData(data);
	m_callback.setHandler(handler);
}


Callback&	Event::accessCallback()
{
	return (m_callback);
}