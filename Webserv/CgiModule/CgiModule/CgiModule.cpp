/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:19:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 11:40:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

CgiModule::CgiModule(size_t workers, size_t backlog, Globals& globals) :
	m_numWorkers(workers),
	m_backlog(backlog),
	m_liveRequestCount(0),
	m_liveRequests(),
	m_allRequestData(),
	m_spareRequestData(MPool_FixedElem<InternalCgiRequestData*>(m_backlog)),
	m_pendingRequests(MPool_FixedElem<InternalCgiRequestData*>(m_backlog)),
	m_spareLiveRequests(MPool_FixedElem<InternalCgiWorker*>(m_numWorkers)),
	m_globals(globals)
{
	m_liveRequests.reserve(workers);
	
	for (size_t i = 0; i < m_numWorkers; i++)
	{
		m_liveRequests.emplace_back(*this, globals);
		m_spareLiveRequests.emplace_back(&m_liveRequests[i]);
	}
	
	m_allRequestData.reserve(backlog);
	for (size_t i = 0; i < m_backlog; i++)
	{
		m_allRequestData.emplace_back();
		m_spareRequestData.emplace_back(&m_allRequestData[i]);
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
	m_globals(copy.m_globals)
{

}

CgiModule& CgiModule::operator=(const CgiModule &assign)
{
	if (this == &assign)
		return (*this);
	m_numWorkers = assign.m_numWorkers;
	return (*this);
}


/*
	Deletes the CgiRequestData as it is no longer needed.
	Checks if there are more pending requests to execute, if so, execute.
	Else, save it for a later request.
*/
void	CgiModule::mf_returnLiveRequest(InternalCgiWorker& request)
{
	InternalCgiRequestData* requestData;

	requestData = static_cast<InternalCgiRequestData*>(request.accessCurRequestData());
	if (requestData)
		mf_deleteRequestData(*requestData);

	request.reset();

	if (m_pendingRequests.size() > 0)
	{
		request.execute(*m_pendingRequests.front());
		m_pendingRequests.pop_front();
	}
	else
	{
		m_spareLiveRequests.push_back(&request);
		m_liveRequestCount--;
	}
}

void	CgiModule::mf_deleteRequestData(InternalCgiRequestData& data)
{	
	List<InternalCgiRequestData>::iterator	dataIter;
	List<InternalCgiRequestData*>::iterator	pendingIter;
	
	dataIter = data.accessDataLocation();
	pendingIter = data.accessPendingLocation();

	if (pendingIter != NULL)
		m_pendingRequests.erase(pendingIter);
	if (dataIter != NULL)
		m_allRequestData.erase(dataIter);
}


void	CgiModule::forceStop()
{
	for (DynArray<InternalCgiWorker>::iterator it = m_liveRequests.begin(); it != m_liveRequests.end(); it++)
	{
		it->forcedClose();
		it->reset();
	}
}
