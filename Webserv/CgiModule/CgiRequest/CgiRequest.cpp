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

#include "CgiRequest.hpp"

CgiModule::Request::Request() :
	m_user			(NULL),
	m_readHandler	(NULL),
	m_writeHandler	(NULL),
	m_timeoutMs		(0),
	m_extension		(""),
	m_scriptPath	(""),
	m_state			(STATE_IDLE),
	m_options		(RUNTIME_BASE)
{
	m_env.envBase.reserve(Cgi::ENV_COUNT);
	for (size_t i = 0; i < CALLBACK_COUNT; i++)
		m_userCallbacks[i] = NULL;
}

CgiModule::Request::~Request()
{

}



void	CgiModule::Request::mf_reset()
{
	m_extension.clear();
	m_scriptPath.clear();
	m_env.envBase.clear();
	m_env.envExtra.clear();

	for (size_t i = 0; i < CALLBACK_COUNT; i++)
		m_userCallbacks[i] = NULL;
	
	m_timeoutMs = 0;
	m_user = NULL;
	m_readHandler = NULL;
	m_writeHandler = NULL;
	m_state = STATE_IDLE;
	m_options = RUNTIME_BASE;
}

CgiModule::Request::Request(const Request &copy) :
	m_user(copy.m_user),
	m_readHandler(copy.m_readHandler),
	m_writeHandler(copy.m_writeHandler),
	m_timeoutMs(copy.m_timeoutMs),
	m_extension(copy.m_extension),
	m_scriptPath(copy.m_scriptPath)
{
	m_env.envBase = copy.m_env.envBase;
	m_env.envExtra = copy.m_env.envExtra;
}

CgiModule::Request&	CgiModule::Request::operator=(const Request &assign)
{
	if (this == &assign)
		return (*this);

	m_user = assign.m_user;
	m_readHandler = assign.m_readHandler;
	m_writeHandler = assign.m_writeHandler;
	m_timeoutMs = assign.m_timeoutMs;
	m_extension = assign.m_extension;
	m_scriptPath = assign.m_scriptPath;
	m_env.envBase = assign.m_env.envBase;
	m_env.envExtra = assign.m_env.envExtra;
	
	return (*this);
}