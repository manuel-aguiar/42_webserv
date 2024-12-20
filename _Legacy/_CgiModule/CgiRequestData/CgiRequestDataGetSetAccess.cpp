/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestDataGetSetAccess.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 12:08:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"



//getters
const std::string&								CgiRequestData::getMsgBody() const
{
	return (m_msgBody);
}

const t_CgiRequestEnv& 							CgiRequestData::getEnvVars() const
{
	return (m_env);
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
void	CgiRequestData::setCallback(const e_CgiCallbacks event, const t_ptr_event_data data, const t_func_event_handler handler)
{
	m_events[event].setFd_Data_Handler_Flags(0, data, handler, 0);
}

void		CgiRequestData::setMsgBody(const std::string& body)
{
	m_msgBody = body;
}

void		CgiRequestData::setEnvBase(const e_CgiEnv env, const std::string& value)
{
	m_env.envBase.emplace_back(env, value);
}

void		CgiRequestData::setEnvExtra(const std::string& key, const std::string& value)
{
	m_env.envExtra[key] = value;
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

Event&				CgiRequestData::accessCallbacks(const e_CgiCallbacks eventType)
{
	return (m_events[eventType]);
}

std::string&		CgiRequestData::accessMsgBody()
{
	return (m_msgBody);
}

EventManager&		CgiRequestData::accessEventManager()
{
	return (*m_eventManager);
}
