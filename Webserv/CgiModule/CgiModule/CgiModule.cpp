/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:19:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:37:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiLiveRequest/CgiLiveRequest.hpp"
# include "CgiModule.hpp"

CgiModule::CgiModule(size_t maxConnections, Globals& globals) :
	m_maxConnections(maxConnections),
	m_liveRequests(),
	m_allRequestData(),
	m_pendingRequests(),
	m_spareLiveRequests(),
	m_globals(globals)
{
	m_liveRequests.reserve(maxConnections);
	for (size_t i = 0; i < maxConnections; i++)
	{
		m_liveRequests.emplace_back(*this, globals);
		m_spareLiveRequests.emplace_back(&m_liveRequests[i]);
	}
		
	
	m_baseEnvLeftEqual[E_CGI_AUTH_TYPE] = STR_CGI_AUTH_TYPE;
	m_baseEnvLeftEqual[E_CGI_CONTENT_LENGTH] = STR_CGI_CONTENT_LENGTH;
	m_baseEnvLeftEqual[E_CGI_CONTENT_TYPE] = STR_CGI_CONTENT_TYPE;
	m_baseEnvLeftEqual[E_CGI_GATEWAY_INTERFACE] = STR_CGI_GATEWAY_INTERFACE;
	m_baseEnvLeftEqual[E_CGI_PATH_INFO] = STR_CGI_PATH_INFO;
	m_baseEnvLeftEqual[E_CGI_PATH_TRANSLATED] = STR_CGI_PATH_TRANSLATED;
	m_baseEnvLeftEqual[E_CGI_QUERY_STRING] = STR_CGI_QUERY_STRING;
	m_baseEnvLeftEqual[E_CGI_REMOTE_ADDR] = STR_CGI_REMOTE_ADDR;
	m_baseEnvLeftEqual[E_CGI_REMOTE_HOST] = STR_CGI_REMOTE_HOST;
	m_baseEnvLeftEqual[E_CGI_REMOTE_IDENT] = STR_CGI_REMOTE_IDENT;
	m_baseEnvLeftEqual[E_CGI_REMOTE_USER] = STR_CGI_REMOTE_USER;
	m_baseEnvLeftEqual[E_CGI_REQUEST_METHOD] = STR_CGI_REQUEST_METHOD;
	m_baseEnvLeftEqual[E_CGI_SCRIPT_NAME] = STR_CGI_SCRIPT_NAME;
	m_baseEnvLeftEqual[E_CGI_SERVER_NAME] = STR_CGI_SERVER_NAME;
	m_baseEnvLeftEqual[E_CGI_SERVER_PORT] = STR_CGI_SERVER_PORT;
	m_baseEnvLeftEqual[E_CGI_SERVER_PROTOCOL] = STR_CGI_SERVER_PROTOCOL;
	m_baseEnvLeftEqual[E_CGI_SERVER_SOFTWARE] = STR_CGI_SERVER_SOFTWARE;
}

CgiModule::~CgiModule()
{

}

/*

	Deletes the CgiRequestData as it is no longer needed.
	Checks if there are more pending requests to execute, if so, execute.
	Else, save it for a later request.

*/
void	CgiModule::mf_returnLiveRequest(CgiLiveRequest& request)
{
	ManagedRequestData* requestData;

	requestData = static_cast<ManagedRequestData*>(request.accessCurRequestData());
	if (requestData)
	{
		for (List<ManagedRequestData>::iterator it = m_allRequestData.begin(); it != m_allRequestData.end(); it++)
		{
			if (&(*it) == requestData)
			{
				m_allRequestData.erase(it);
				break ;
			}
		}
	}

	request.reset();

	if (m_pendingRequests.size() > 0)
	{
		request.execute(*m_pendingRequests.front());
		m_pendingRequests.pop_front();
	}
	else
		m_spareLiveRequests.push_back(&request);
}