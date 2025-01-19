/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestExternal.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:33:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 19:33:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequest.hpp"


void
CgiModule::Request::setUser(CgiModule::User user)
{
	assert(m_state == STATE_ACQUIRED);
	m_user = user;
}

void
CgiModule::Request::setUserCallback(const CgiModule::CallbackType type, const CgiModule::Callback handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_userCallbacks[type] = handler;
}

void
CgiModule::Request::setReadHandler(CgiModule::IO_Handler handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_readHandler = handler;
}

void
CgiModule::Request::setWriteHandler(CgiModule::IO_Handler handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_writeHandler = handler;
}

void
CgiModule::Request::setEnvBase(const Cgi::EnvType env, const CgiModule::EnvValue& value)
{
	assert(m_state == STATE_ACQUIRED);
	m_env.envBase.emplace_back(env, value);
}

void
CgiModule::Request::setEnvExtra(const CgiModule::EnvKey& key, const CgiModule::EnvValue& value)
{
	assert(m_state == STATE_ACQUIRED);
	m_env.envExtra.emplace_back(key, value);
}

void
CgiModule::Request::setExtension(const CgiModule::InterpExtension& extension)
{
	assert(m_state == STATE_ACQUIRED);
	m_extension = extension;
}

void
CgiModule::Request::setScriptPath(const std::string& path)
{
	assert(m_state == STATE_ACQUIRED);
	m_scriptPath = path;
}


void
CgiModule::Request::setTimeoutMs(const unsigned int timeoutMs)
{
	assert(m_state == STATE_ACQUIRED);
	m_timeoutMs = timeoutMs;
}

void
CgiModule::Request::setRuntimeOptions(CgiModule::RuntimeOptions options)
{
	assert(m_state == STATE_ACQUIRED);
	m_options = options;
}

const CgiModule::EnvVariables& 
CgiModule::Request::getEnvVars() const
{
	return (m_env);
}

const CgiModule::InterpExtension&
CgiModule::Request::getExtension() const
{
	return (m_extension);
}

const std::string&
CgiModule::Request::getScriptPath() const
{
	return (m_scriptPath);
}

unsigned int
CgiModule::Request::getTimeoutMs() const
{
	return (m_timeoutMs);
}
