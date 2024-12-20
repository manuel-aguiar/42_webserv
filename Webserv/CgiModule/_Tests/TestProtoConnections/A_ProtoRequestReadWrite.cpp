/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequestReadWrite.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 14:19:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"

void	A_ProtoRequest::writeCgi()
{
	int					triggeredFlags;
	int					bytesWritten;
	const Event&		writeEvent = m_CgiRequestData->getWriteEvent();

	triggeredFlags = writeEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_manager.delEvent(writeEvent);
		m_cgi.finishedWriting(*m_CgiRequestData);
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = ::send(writeEvent.getFd(), m_msgBody.c_str(), m_msgBody.size(), MSG_NOSIGNAL);

		if (bytesWritten == -1)
		{
			m_manager.delEvent(writeEvent);
			m_cgi.finishedWriting(*m_CgiRequestData);
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
			m_manager.delEvent(writeEvent);
			m_cgi.finishedWriting(*m_CgiRequestData);
		}
	}
}

void	A_ProtoRequest::readCgi()
{
	size_t				bytesRead;
	int					triggeredFlags;
	const Event&		readEvent = m_CgiRequestData->getReadEvent();
	
	triggeredFlags = readEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(readEvent.getFd(), m_buffer, sizeof(m_buffer) - 1);
		m_TotalBytesRead += bytesRead;
		if (bytesRead == 0)
		{
			m_manager.delEvent(readEvent);
			m_cgi.finishedReading(*m_CgiRequestData);
			m_cgi.finishedRequest(*m_CgiRequestData);
			printBufStdout();
		}
	}

	/*
		There must be some distinction between finishing reading because there is
		no more and finish reading because there was an error..........
	*/

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		m_manager.delEvent(readEvent);
		m_cgi.finishedReading(*m_CgiRequestData);
		m_cgi.finishedRequest(*m_CgiRequestData);
		printBufStdout();
	}
}

void	A_ProtoRequest::executeCgi()
{
	m_CgiRequestData->accessReadEvent().setCallback(this, &A_ProtoRequest::EventCallbackOnRead);
	m_CgiRequestData->accessReadEvent().setMonitorFlags(EPOLLIN);
	m_CgiRequestData->accessWriteEvent().setCallback(this, &A_ProtoRequest::EventCallbackOnWrite);
	m_CgiRequestData->accessWriteEvent().setMonitorFlags(EPOLLOUT);
	m_manager.addEvent(m_CgiRequestData->getReadEvent());
	m_manager.addEvent(m_CgiRequestData->getWriteEvent());
}

void	A_ProtoRequest::cancelCgi()
{
	m_manager.delEvent(m_CgiRequestData->getReadEvent());
	m_manager.delEvent(m_CgiRequestData->getWriteEvent());
}

void A_ProtoRequest::EventCallbackOnRead(Callback& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	request.readCgi();
}

void A_ProtoRequest::EventCallbackOnWrite(Callback& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	request.writeCgi();
}