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
	m_CgiModule(manager),
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
	m_pid = -1;

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

	m_argPtr.clear();
	m_envPtr.clear();
	m_envStr.clear();
}




void	CgiLiveRequest::writeToChild()
{
	int					triggeredFlags;
	size_t				bytesWritten;
	std::string& body = m_curRequestData->accessMsgBody();

	triggeredFlags = m_writeEvent.getTriggeredFlags();
	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = ::write(m_ParentToChild[1], body.c_str(), body.size());

		if (bytesWritten != body.size())
		{
			if (bytesWritten > 0)
				body.erase(0, bytesWritten);
			return ;
		}
		else
		{
			m_curEventManager->delEvent(m_writeEvent);
			mf_closeFd(m_ParentToChild[1]);
		}
		m_curRequestData->accessEventHandler(E_CGI_ON_WRITE).handle();
	}
	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_curEventManager->delEvent(m_writeEvent);
		mf_closeFd(m_ParentToChild[1]);
		m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle();
	}
}

void	CgiLiveRequest::readFromChild()
{
	size_t	bytesRead;
	int		triggeredFlags;
	
	triggeredFlags = m_readEvent.getTriggeredFlags();
	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(m_ChildToParent[0], m_readBuf, sizeof(m_readBuf) - 1);
		m_readBuf[bytesRead] = '\0';
		std::cout << "Read from child: " << m_readBuf << std::endl;
		m_curRequestData->accessEventHandler(E_CGI_ON_READ).handle();
	}
		
	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		m_curEventManager->delEvent(m_readEvent);
		mf_closeFd(m_ChildToParent[0]);
		m_curRequestData->accessEventHandler(E_CGI_ON_CLOSE).handle();
	}
}

void	CgiLiveRequest::cleanClose()
{
	int status;

	if (m_pid != -1)
	{
		::waitpid(m_pid, &status, 0);
		m_pid = -1;
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			m_curRequestData->accessEventHandler(E_CGI_ON_CLOSE).handle();
		else
			m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle();
	}
}

void	CgiLiveRequest::forcedClose()
{
	if (m_pid != -1)
	{
		::kill(m_pid, SIGKILL);
		::waitpid(m_pid, NULL, 0);
		m_pid = -1;
	}
	m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle();
}

CgiRequestData*	CgiLiveRequest::accessCurRequestData()
{
	return (m_curRequestData);
}



CgiLiveRequest::CgiLiveRequest(const CgiLiveRequest &other) :
	m_curEventManager(other.m_curEventManager),
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_CgiModule(other.m_CgiModule),
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
