/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestProtoRequest_onCallbacksCgi.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 14:11:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TestProtoRequest.hpp"
# include <unistd.h>


CgiModule::BytesRead
TestProtoRequest::CgiRead(int readFd)
{
	int bytesRead;

	bytesRead = ::read(readFd, &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
	m_TotalBytesRead += bytesRead;
	m_buffer[m_TotalBytesRead] = '\0';
	
	return (bytesRead);
}

CgiModule::BytesWritten
TestProtoRequest::CgiWrite(int writeFd)
{
	int bytesWritten;

	if (m_msgBody.size() == 0)
		return (0);
		
	bytesWritten = ::write(writeFd, m_msgBody.c_str(), m_msgBody.size());

	if ((size_t)bytesWritten != m_msgBody.size() && bytesWritten > 0)
		m_msgBody.erase(0, bytesWritten);
	return (bytesWritten);
}

void
TestProtoRequest::SuccessCgi()
{
	m_CgiResultStatus = E_CGI_STATUS_SUCCESS;
}

/*
	The oposite of execute Cgi, delete events and finish request
*/
void
TestProtoRequest::cancelCgi()
{	
	m_CgiResultStatus = E_CGI_STATUS_ERROR_RUNTIME;
}

void
TestProtoRequest::falseStartCgi()
{
	
	m_CgiResultStatus = E_CGI_STATUS_ERROR_STARTUP;
}


void	
TestProtoRequest::timeoutCgi()
{
	m_CgiResultStatus = E_CGI_STATUS_TIMEOUT;
}