/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest_onCallbacksCgi.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 15:15:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"
# include <unistd.h>

//event callbacks
void A_ProtoRequest::EventCallbackOnRead(Callback& event)
{
	A_ProtoRequest* request = static_cast<A_ProtoRequest*>(event.getData());
	request->OnRead();
}

void A_ProtoRequest::EventCallbackOnWrite(Callback& event)
{
	A_ProtoRequest* request = static_cast<A_ProtoRequest*>(event.getData());
	request->onWrite();
}

/*
	WriteCgi, to be triggered by the event manager via the Callback
*/
void	A_ProtoRequest::onWrite()
{
	int					triggeredFlags;
	int					bytesWritten;

	triggeredFlags = m_CgiWriteEvent.getTriggeredFlags();
	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_eventManager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = ::write(m_CgiWriteEvent.getFd(), m_msgBody.c_str(), m_msgBody.size());

		//outdated event, ignore, wait for epoll to trigger for the new file attached to this fd
		if (bytesWritten == -1)
			return ;

		if ((size_t)bytesWritten != m_msgBody.size())
		{
			
			if (bytesWritten > 0)
				m_msgBody.erase(0, bytesWritten);
			return ;
		}
		else
		{
			m_eventManager.delEvent(m_CgiWriteEvent);
			m_CgiWriteEvent.reset();
		}
	}
}

/*
	ReadCgi, to be triggered by the event manager via the Callback
*/
void	A_ProtoRequest::OnRead()
{
	int					bytesRead;
	int					triggeredFlags;

	triggeredFlags = m_CgiReadEvent.getTriggeredFlags();
	
	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(m_CgiReadEvent.getFd(), &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
		
		//outdated event, ignore, wait for epoll to trigger for the new file attached to this fd
		if (bytesRead == -1)
			return ;
		m_TotalBytesRead += bytesRead;
		if (bytesRead == 0 || m_TotalBytesRead == sizeof(m_buffer) - 1)
		{
			m_eventManager.delEvent(m_CgiReadEvent);
			m_CgiReadEvent.reset();
			m_cgi.finishRequest(*m_CgiRequestData);
			printBufStdout();

			//internal test
			if (m_CgiResultStatus == E_CGI_STATUS_WORKING)
				m_CgiResultStatus = E_CGI_STATUS_SUCCESS;
				
		}
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		//std::cout << "proto " << m_id << " disabling read event after disconnection" << std::endl;
		m_eventManager.delEvent(m_CgiReadEvent);
		m_CgiReadEvent.reset();
		m_cgi.finishRequest(*m_CgiRequestData);
		printBufStdout();

		//internal test
		if (m_CgiResultStatus == E_CGI_STATUS_WORKING)
			m_CgiResultStatus = E_CGI_STATUS_SUCCESS;
			
	}
}

/*
	Setup events to read/write from/to the cgi script,
	setup the callbacks to be triggered by the event manager
*/
void	A_ProtoRequest::executeCgi()
{
	m_CgiReadEvent.setFd(m_CgiRequestData->getReadFd());
	m_CgiReadEvent.setMonitoredFlags(EPOLLIN);
	m_CgiReadEvent.setCallback(this, &A_ProtoRequest::EventCallbackOnRead);

	m_CgiWriteEvent.setFd(m_CgiRequestData->getWriteFd());
	m_CgiWriteEvent.setMonitoredFlags(EPOLLOUT);
	m_CgiWriteEvent.setCallback(this, &A_ProtoRequest::EventCallbackOnWrite);
	
	m_eventManager.addEvent(m_CgiReadEvent);
	m_eventManager.addEvent(m_CgiWriteEvent);
}

/*
	The oposite of execute Cgi, delete events and finish request
*/
void	A_ProtoRequest::cancelCgi()
{	
	if (m_CgiReadEvent.getFd() != -1)
	{
		m_eventManager.delEvent(m_CgiReadEvent);
		m_CgiReadEvent.reset();
	}
	if (m_CgiWriteEvent.getFd() != -1)
	{
		m_eventManager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
	}
	m_cgi.finishRequest(*m_CgiRequestData);
	
	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_ERROR_RUNTIME;

}

/*
	Not much here, just mark the request as finished
*/
void	A_ProtoRequest::falseStartCgi()
{
	m_cgi.finishRequest(*m_CgiRequestData);

	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_ERROR_STARTUP;
}


void	A_ProtoRequest::timeoutCgi()
{
	//std::cout << "proto " << m_id << " timeoutCgi" << std::endl;
	if (m_CgiReadEvent.getFd() != -1)
	{
		m_eventManager.delEvent(m_CgiReadEvent);
		m_CgiReadEvent.reset();
	}
	if (m_CgiWriteEvent.getFd() != -1)
	{
		m_eventManager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
	}
	m_cgi.finishRequest(*m_CgiRequestData);
	
	//inform your client something bad happened

	//internal test
	m_CgiResultStatus = E_CGI_STATUS_TIMEOUT;
}