/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequestReadWrite.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:48:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 13:45:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"

void	A_ProtoRequest::writeCgi()
{
	int					triggeredFlags;
	int					bytesWritten;
	const Event&		writeEvent = m_curRequestData->getWriteEvent();

	triggeredFlags = writeEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_manager.delEvent(writeEvent);
		m_cgi.finishedWriting(*m_curRequestData);
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		
		bytesWritten = ::send(writeEvent.getFd(), m_msgBody.c_str(), m_msgBody.size(), MSG_NOSIGNAL);

		if (bytesWritten == -1)
		{
			m_manager.delEvent(writeEvent);
			m_cgi.finishedWriting(*m_curRequestData);
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
			m_cgi.finishedWriting(*m_curRequestData);
		}
	}
}

void	A_ProtoRequest::readCgi()
{
	size_t				bytesRead;
	int					triggeredFlags;
	const Event&		readEvent = m_curRequestData->getReadEvent();
	
	triggeredFlags = readEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(readEvent.getFd(), m_buffer, sizeof(m_buffer) - 1);
		m_buffer[bytesRead] = '\0';
		if (bytesRead == 0)
		{
			m_manager.delEvent(readEvent);
			m_cgi.finishedReading(*m_curRequestData);
			m_cgi.finishedRequest(*m_curRequestData);
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
		m_cgi.finishedReading(*m_curRequestData);
		m_cgi.finishedRequest(*m_curRequestData);
		printBufStdout();
	}
}

void	A_ProtoRequest::executeCgi()
{
	m_manager.addEvent(m_curRequestData->getReadEvent());
	m_manager.addEvent(m_curRequestData->getWriteEvent());
}

void	A_ProtoRequest::cancelCgi()
{
	m_manager.delEvent(m_curRequestData->getReadEvent());
	m_manager.delEvent(m_curRequestData->getWriteEvent());
}