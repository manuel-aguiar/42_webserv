/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest_onCallbacksCgi.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 15:53:13 by mmaria-d         ###   ########.fr       */
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
		m_manager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = ::send(m_CgiWriteEvent.getFd(), m_msgBody.c_str(), m_msgBody.size(), MSG_NOSIGNAL);

		if (bytesWritten == -1)
		{
			m_manager.delEvent(m_CgiWriteEvent);
			m_CgiWriteEvent.reset();
			return ;
		}
		
		if ((size_t)bytesWritten != m_msgBody.size())
		{
			
			if (bytesWritten > 0)
				m_msgBody.erase(0, bytesWritten);
			return ;
		}
		else
		{
			m_manager.delEvent(m_CgiWriteEvent);
			m_CgiWriteEvent.reset();
		}
	}
}

/*
	ReadCgi, to be triggered by the event manager via the Callback
*/
void	A_ProtoRequest::OnRead()
{
	size_t				bytesRead;
	int					triggeredFlags;
	
	triggeredFlags = m_CgiReadEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(m_CgiReadEvent.getFd(), &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
		m_TotalBytesRead += bytesRead;
		if (bytesRead == 0 || m_TotalBytesRead == sizeof(m_buffer) - 1)
		{
			m_manager.delEvent(m_CgiReadEvent);
			m_CgiReadEvent.reset();
			m_cgi.finishRequest(*m_CgiRequestData);
			printBufStdout();
		}
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		m_manager.delEvent(m_CgiReadEvent);
		m_CgiReadEvent.reset();
		m_cgi.finishRequest(*m_CgiRequestData);
		printBufStdout();
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
	
	m_manager.addEvent(m_CgiReadEvent);
	m_manager.addEvent(m_CgiWriteEvent);
}

/*
	The oposite of execute Cgi, delete events and finish request
*/
void	A_ProtoRequest::cancelCgi()
{	
	if (m_CgiReadEvent.getFd() != -1)
	{
		m_manager.delEvent(m_CgiReadEvent);
		m_CgiReadEvent.reset();
	}
	if (m_CgiWriteEvent.getFd() != -1)
	{
		m_manager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
	}
	m_cgi.finishRequest(*m_CgiRequestData);

	//inform client something bad happened

	m_CancelCount++;
}

/*
	Not much here, just mark the request as finished
*/
void	A_ProtoRequest::falseStartCgi()
{
	m_cgi.finishRequest(*m_CgiRequestData);

	//inform client something bad happened
}