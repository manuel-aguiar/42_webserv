/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:44:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:41:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
#include "CgiLiveRequest.hpp"
#include "../CgiRequestData/CgiRequestData.hpp"
#include "../../Globals/Globals.hpp"
#include "../../ServerManager/EventManager/EventManager.hpp"

//#include "../python-cgi/pythonCgi.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

CgiLiveRequest::CgiLiveRequest(CgiModule& manager, Globals& globals) :
	m_curEventManager(NULL),
	m_curRequestData(NULL),
	m_pid(-1),
	m_manager(manager),
	m_globals(globals)
{
	m_ParentToChild[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
	m_ChildToParent[1] = -1;
	m_writeEvent.setFd_Data_Handler_Flags(0, this, &CgiLiveRequest::mf_CgiWrite, EPOLLIN);
	m_writeEvent.setFd_Data_Handler_Flags(0, this, &CgiLiveRequest::mf_CgiRead, EPOLLOUT);
}

CgiLiveRequest::~CgiLiveRequest()
{

}

void    CgiLiveRequest::reset()
{
	m_readEvent.reset();
	m_writeEvent.reset();
	m_curRequestData = NULL;
	m_curEventManager = NULL;

	if (m_writeEvent.getFd() != -1)
	{
		m_curEventManager->delEvent(m_writeEvent);
		m_writeEvent.setFd(-1);
	}
	if (m_readEvent.getFd() != -1)
	{
		m_curEventManager->delEvent(m_readEvent);
		m_readEvent.setFd(-1);
	}

	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ParentToChild[1]);
	mf_closeFd(m_ChildToParent[0]);
	mf_closeFd(m_ChildToParent[1]);
}




void	CgiLiveRequest::writeToChild()
{
	size_t bytesWritten;
	std::string& body = m_curRequestData->accessMsgBody();

	bytesWritten = ::write(m_ParentToChild[1], body.c_str(), body.size());

	if (bytesWritten != body.size())
	{
		if (bytesWritten > 0)
			body.erase(0, bytesWritten);
		return ;
	}
		
	m_curEventManager->delEvent(m_writeEvent);
	m_writeEvent.setFd(-1);
	mf_closeFd(m_ParentToChild[1]);
}

void	CgiLiveRequest::readFromChild()
{
	size_t	bytesRead;
	int		triggeredFlags;
	
	triggeredFlags = m_readEvent.getTriggeredFlags();
	bytesRead = ::read(m_ChildToParent[0], m_readBuf, sizeof(m_readBuf));

	if (bytesRead == 0 || (triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		m_curEventManager->delEvent(m_readEvent);
		mf_closeFd(m_ChildToParent[0]);
	}

	m_curRequestData->getEventHandler(CGI_ON_READ).handle();
}

void	CgiLiveRequest::forcedClose()
{
	if (m_pid != -1)
	{
		::kill(m_pid, SIGKILL);
		m_pid = -1;
		::waitpid(m_pid, NULL, 0);
	}
	m_curRequestData->getEventHandler(CGI_ON_ERROR).handle();
	reset();
}





CgiLiveRequest::CgiLiveRequest(const CgiLiveRequest &other) :
	m_curEventManager(other.m_curEventManager),
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_manager(other.m_manager),
	m_globals(other.m_globals)
{
    *this = other;
}

CgiLiveRequest &CgiLiveRequest::operator=(const CgiLiveRequest &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
