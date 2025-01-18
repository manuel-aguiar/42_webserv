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

//protected

void	CgiRequestData::mf_reset()
{
	m_extension.clear();
	m_scriptPath.clear();
	m_env.envBase.clear();
	m_env.envExtra.clear();

	for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
		m_stateCallbacks[i] = NULL;
	
	m_timeoutMs = 0;
	m_user = NULL;
	m_readHandler = NULL;
	m_writeHandler = NULL;
}

CgiRequestData::CgiRequestData() :
	m_user(NULL),
	m_readHandler(NULL),
	m_writeHandler(NULL),
	m_extension(""),
	m_scriptPath(""),
	m_timeoutMs(0),
	m_state(E_CGI_STATE_IDLE)
{
	m_env.envBase.reserve(E_CGI_ENV_COUNT);
	for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
		m_stateCallbacks[i] = NULL;
}

CgiRequestData::~CgiRequestData()
{

}

CgiRequestData::CgiRequestData(const CgiRequestData &copy) :
	m_extension(copy.m_extension),
	m_scriptPath(copy.m_scriptPath)
{
	//more needed here
	m_env.envBase = copy.m_env.envBase;
	m_env.envExtra = copy.m_env.envExtra;
}

CgiRequestData &CgiRequestData::operator=(const CgiRequestData &assign)
{
	//more needed here
	if (this == &assign)
		return (*this);
	m_scriptPath = assign.m_scriptPath;
	m_env.envBase = assign.m_env.envBase;
	m_env.envExtra = assign.m_env.envExtra;
	m_extension = assign.m_extension;
	m_state = assign.m_state;
	return (*this);
}