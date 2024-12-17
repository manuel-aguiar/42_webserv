/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestDataGetSetAccess.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 15:10:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"



//getters
const std::string&								CgiRequestData::getMsgBody() const
{
	return (m_msgBody);
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

const std::string&								CgiRequestData::getScriptPath() const
{
	return (m_scriptPath);
}


// setters
void	CgiRequestData::setEventHandler(const e_CgiEvents event, const t_ptr_event_data data, const t_func_event_handler handler)
{
	m_events[event].setFd_Data_Handler_Flags(0, data, handler, 0);
}

void		CgiRequestData::setMsgBody(const std::string& body)
{
	m_msgBody = body;
}

void		CgiRequestData::setEnvBase(const e_CgiEnv env, const std::string& value)
{
	m_envBase[env] = value;
}

void		CgiRequestData::setEnvExtra(const std::string& key, const std::string& value)
{
	m_envExtra[key] = value;
}

void		CgiRequestData::setExtension(const std::string& extension)
{
	m_extension = extension;
}

void		CgiRequestData::setScriptPath(const std::string& path)
{
	m_scriptPath = path;
}

void		CgiRequestData::setEventManager(EventManager& eventManager)
{
	m_eventManager = &eventManager;
}

//accessors

Event&				CgiRequestData::accessEventHandler(const e_CgiEvents eventType)
{
	return (m_events[eventType]);
}

std::string&		CgiRequestData::accessMsgBody()
{
	return (m_msgBody);
}

