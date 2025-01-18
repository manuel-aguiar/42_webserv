/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest_onCallbacksCgi.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 10:19:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../Test_EventManager/Test_EventManager.hpp"
# include <unistd.h>


CgiRequestData::t_bytesRead	A_ProtoRequest::newCgiRead(int readFd)
{
	int bytesRead;

	bytesRead = ::read(readFd, &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
	m_TotalBytesRead += bytesRead;
	m_buffer[m_TotalBytesRead] = '\0';

	return (bytesRead);
}

CgiRequestData::t_bytesWritten	A_ProtoRequest::newCgiWrite(int writeFd)
{
	int bytesWritten;

	if (m_msgBody.size() == 0)
		return (0);

	bytesWritten = ::write(writeFd, m_msgBody.c_str(), m_msgBody.size());
	if ((size_t)bytesWritten != m_msgBody.size() && bytesWritten > 0)
		m_msgBody.erase(0, bytesWritten);
	return (bytesWritten);
}

/*
	Setup events to read/write from/to the cgi script,
	setup the callbacks to be triggered by the event manager
*/
void	A_ProtoRequest::SuccessCgi()
{
	m_CgiResultStatus = E_CGI_STATUS_SUCCESS;
}

/*
	The oposite of execute Cgi, delete events and finish request
*/
void	A_ProtoRequest::cancelCgi()
{	
	
	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_ERROR_RUNTIME;

}

/*
	Not much here, just mark the request as finished
*/
void	A_ProtoRequest::falseStartCgi()
{

	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_ERROR_STARTUP;
}


void	A_ProtoRequest::timeoutCgi()
{
	//std::cout << "proto " << m_id << " timeoutCgi" << std::endl;
	
	
	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_TIMEOUT;
}