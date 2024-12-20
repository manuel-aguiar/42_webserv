/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestDataGetSetAccess.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 13:18:10 by mmaria-d         ###   ########.fr       */
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

const Event&									CgiRequestData::getReadEvent() const
{
	return (m_CgiReadEvent);
}

const Event&									CgiRequestData::getWriteEvent() const
{
	return (m_CgiWriteEvent);
}

// setters
void	CgiRequestData::setCallback(const e_CgiCallbacks event, const t_ptr_callback_data data, const t_func_callback_handler handler)
{
	m_callbacks[event].setData_Handler(data, handler);
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

//accessors

Callback&				CgiRequestData::accessCallback(const e_CgiCallbacks eventType)
{
	return (m_callbacks[eventType]);
}

Event& 					CgiRequestData::accessReadEvent()
{
	return (m_CgiReadEvent);
}

Event& 					CgiRequestData::accessWriteEvent()
{
	return (m_CgiWriteEvent);
}

