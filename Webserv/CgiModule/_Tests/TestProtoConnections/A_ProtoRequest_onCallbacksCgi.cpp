/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest_onCallbacksCgi.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 17:40:35 by mmaria-d         ###   ########.fr       */
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
	//std::cout << "proto " << m_id << " writingCgi" << std::endl;
	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_eventManager.delEvent(m_CgiWriteEvent);
		m_CgiWriteEvent.reset();
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = ::send(m_CgiWriteEvent.getFd(), m_msgBody.c_str(), m_msgBody.size(), MSG_NOSIGNAL);

		if (bytesWritten == -1)
		{
			//std::cout << "proto " << m_id << " disabling write event" << std::endl;
			m_eventManager.delEvent(m_CgiWriteEvent);
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
	size_t				bytesRead;
	int					triggeredFlags;
	//std::cout << "proto " << m_id << " readingCgi" << std::endl;
	triggeredFlags = m_CgiReadEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(m_CgiReadEvent.getFd(), &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
		m_TotalBytesRead += bytesRead;
		if (bytesRead == 0 || m_TotalBytesRead == sizeof(m_buffer) - 1)
		{
			//std::cout << "proto " << m_id << " disabling read event after reading" << std::endl;
			m_eventManager.delEvent(m_CgiReadEvent);
			m_CgiReadEvent.reset();
			m_cgi.finishRequest(*m_CgiRequestData);
			printBufStdout();
		}
			//std::cout << "proto " << m_id << " read " << bytesRead << " bytes" << std::endl;
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		//std::cout << "proto " << m_id << " disabling read event after disconnection" << std::endl;
		m_eventManager.delEvent(m_CgiReadEvent);
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
	//std::cout << "proto " << m_id << " executinCgi" << std::endl;
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
	//std::cout << "proto " << m_id << " cancelingCgi" << std::endl;
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

	m_CancelCount++;
}

/*
	Not much here, just mark the request as finished
*/
void	A_ProtoRequest::falseStartCgi()
{
	//std::cout << "proto " << m_id << " falseStartCgi" << std::endl;
	m_cgi.finishRequest(*m_CgiRequestData);

	//inform your client something bad happened
}