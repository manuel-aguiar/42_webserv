/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestDataGetSetAccess.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 11:13:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"

const t_CgiRequestEnv& 	CgiRequestData::getEnvVars() const
{
	return (m_env);
}

const std::string&		CgiRequestData::getExtension() const
{
	return (m_extension);
}

const std::string&		CgiRequestData::getScriptPath() const
{
	return (m_scriptPath);
}

unsigned int 	        CgiRequestData::getTimeoutMs() const
{
	return (m_timeoutMs);
}


void		CgiRequestData::setUser(t_ptr_callback_data user)
{
	m_user = user;
}


void		CgiRequestData::setHandler(const e_CgiCallback type, const t_func_CgiHandler handler)
{
	m_stateCallbacks[type] = handler;
}

void	CgiRequestData::setReadHandler(t_cgi_IO handler)
{
	m_readHandler = handler;
}

void	CgiRequestData::setWriteHandler(t_cgi_IO handler)
{
	m_writeHandler = handler;
}

void		CgiRequestData::setEnvBase(const e_CgiEnv env, const std::string& value)
{
	m_env.envBase.emplace_back(env, value);
}

void		CgiRequestData::setEnvExtra(const std::string& key, const std::string& value)
{
	m_env.envExtra.emplace_back(key, value);
}

void		CgiRequestData::setExtension(const std::string& extension)
{
	m_extension = extension;
}

void		CgiRequestData::setScriptPath(const std::string& path)
{
	m_scriptPath = path;
}


void	CgiRequestData::setTimeoutMs(const unsigned int timeoutMs)
{
	m_timeoutMs = timeoutMs;
}

