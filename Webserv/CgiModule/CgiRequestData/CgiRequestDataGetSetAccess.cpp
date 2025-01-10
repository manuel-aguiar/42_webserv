/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestDataGetSetAccess.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 10:14:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"


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

t_fd										CgiRequestData::getReadFd() const
{
	return (m_readFd);
}

t_fd										CgiRequestData::getWriteFd() const
{
	return (m_writeFd);
}

// setters
void	CgiRequestData::setCallback(const e_CgiCallback type, const t_ptr_callback_data data, const t_func_callback_handler handler)
{
	m_callbacks[type].setData_Handler(data, handler);
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

void		CgiRequestData::setEventManager(EventManager& manager)
{
	m_eventManager = &manager;
}

//accessors

Callback&				CgiRequestData::accessCallback(const e_CgiCallback eventType)
{
	return (m_callbacks[eventType]);
}

EventManager*			CgiRequestData::accessEventManager()
{
	return (m_eventManager);
}
