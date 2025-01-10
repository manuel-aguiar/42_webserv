/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:19:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 18:24:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

CgiModule::CgiModule(size_t workers, size_t backlog, Globals& globals) :
	m_numWorkers(workers),
	m_backlog(backlog),
	m_busyWorkerCount(0),
	m_allWorkers(workers),
	m_allRequestData(backlog),
	m_availableWorkers(workers),
	m_availableRequestData(backlog),
	m_executionQueue(backlog),
	m_globals(globals),
	m_timerTracker(backlog)
{
	// prepare workers
	for (size_t i = 0; i < m_numWorkers; i++)
	{
		m_allWorkers.emplace_back(*this, globals);
		m_availableWorkers.push_back(&m_allWorkers[i]);
	}
	
	// prepare RequestData
	for (size_t i = 0; i < m_backlog; i++)
	{
		m_allRequestData.emplace_back();
		m_availableRequestData.push_back(&m_allRequestData[i]);
	}
		
	
	m_baseEnv[E_CGI_AUTH_TYPE] = STR_CGI_AUTH_TYPE;
	m_baseEnv[E_CGI_CONTENT_LENGTH] = STR_CGI_CONTENT_LENGTH;
	m_baseEnv[E_CGI_CONTENT_TYPE] = STR_CGI_CONTENT_TYPE;
	m_baseEnv[E_CGI_GATEWAY_INTERFACE] = STR_CGI_GATEWAY_INTERFACE;
	m_baseEnv[E_CGI_PATH_INFO] = STR_CGI_PATH_INFO;
	m_baseEnv[E_CGI_PATH_TRANSLATED] = STR_CGI_PATH_TRANSLATED;
	m_baseEnv[E_CGI_QUERY_STRING] = STR_CGI_QUERY_STRING;
	m_baseEnv[E_CGI_REMOTE_ADDR] = STR_CGI_REMOTE_ADDR;
	m_baseEnv[E_CGI_REMOTE_HOST] = STR_CGI_REMOTE_HOST;
	m_baseEnv[E_CGI_REMOTE_IDENT] = STR_CGI_REMOTE_IDENT;
	m_baseEnv[E_CGI_REMOTE_USER] = STR_CGI_REMOTE_USER;
	m_baseEnv[E_CGI_REQUEST_METHOD] = STR_CGI_REQUEST_METHOD;
	m_baseEnv[E_CGI_SCRIPT_NAME] = STR_CGI_SCRIPT_NAME;
	m_baseEnv[E_CGI_SERVER_NAME] = STR_CGI_SERVER_NAME;
	m_baseEnv[E_CGI_SERVER_PORT] = STR_CGI_SERVER_PORT;
	m_baseEnv[E_CGI_SERVER_PROTOCOL] = STR_CGI_SERVER_PROTOCOL;
	m_baseEnv[E_CGI_SERVER_SOFTWARE] = STR_CGI_SERVER_SOFTWARE;
}

CgiModule::~CgiModule()
{
	forceStop();
}

//private as usual
CgiModule::CgiModule(const CgiModule &copy) :
	m_numWorkers(copy.m_numWorkers),
	m_globals(copy.m_globals),
	m_timerTracker(copy.m_backlog)
{

}

CgiModule& CgiModule::operator=(const CgiModule &assign)
{
	if (this == &assign)
		return (*this);
	m_numWorkers = assign.m_numWorkers;
	return (*this);
}





