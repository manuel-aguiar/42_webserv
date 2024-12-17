/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestData.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:36:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:36:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"

void	CgiRequestData::setEventHandler(const e_CgiEvents event, const t_ptr_event_data data, const t_func_event_handler handler)
{
	m_events[event].setFd_Data_Handler_Flags(0, data, handler, 0);
}

const std::map<e_CgiEnv, t_CgiEnvValue>& 		CgiRequestData::getEnvBase() const
{
	return (m_envBase);
}

const std::map<t_CgiEnvKey, t_CgiEnvValue>&		CgiRequestData::getEnvExtra() const
{
	return (m_envExtra);
}

const std::string&								CgiRequestData::getExtension() const
{
	return (m_extension);
}
