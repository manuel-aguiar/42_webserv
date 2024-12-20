/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorker.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:44:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:41:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
#include "InternalCgiWorker.hpp"
#include "../CgiRequestData/CgiRequestData.hpp"
#include "../../Globals/Globals.hpp"
#include "../../ServerManager/EventManager/EventManager.hpp"

//#include "../python-cgi/pythonCgi.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

CgiModule::InternalCgiWorker::InternalCgiWorker(CgiModule& manager, Globals& globals) :
	m_curRequestData(NULL),
	m_pid(-1),
	m_CgiModule(manager),
	m_globals(globals)
{
	m_ParentToChild[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
	m_ChildToParent[1] = -1;
}

CgiModule::InternalCgiWorker::~InternalCgiWorker()
{

}

void    CgiModule::InternalCgiWorker::reset()
{
	m_curRequestData = NULL;
	m_pid = -1;

	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ParentToChild[1]);
	mf_closeFd(m_ChildToParent[0]);
	mf_closeFd(m_ChildToParent[1]);

	m_argPtr.clear();
	m_envPtr.clear();
	m_envStr.clear();
}




void	CgiModule::InternalCgiWorker::writeToChild()
{
	int					triggeredFlags;
	int					bytesWritten;
	std::string& body = m_curRequestData->accessMsgBody();

	triggeredFlags = m_writeEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLERR || triggeredFlags & EPOLLHUP)
	{
		m_curEventManager->delEvent(m_writeEvent);
		mf_closeFd(m_ParentToChild[1]);
		m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle();
		return ;
	}

	if (triggeredFlags & EPOLLOUT)
	{
		
		bytesWritten = ::send(m_ParentToChild[1], body.c_str(), body.size(), MSG_NOSIGNAL);

		if (bytesWritten == -1)
		{
			m_curEventManager->delEvent(m_writeEvent);
			mf_closeFd(m_ParentToChild[1]);
			return ;
		}
		
		if ((size_t)bytesWritten != body.size())
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
}

void	CgiModule::InternalCgiWorker::readFromChild()
{
	size_t	bytesRead;
	int		triggeredFlags;
	
	triggeredFlags = m_readEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(m_ChildToParent[0], m_readBuf, sizeof(m_readBuf) - 1);
		m_readBuf[bytesRead] = '\0';
		if (bytesRead == 0)
		{
			m_curEventManager->delEvent(m_readEvent);
			mf_closeFd(m_ChildToParent[0]);
		}
		m_curRequestData->accessEventHandler(E_CGI_ON_READ).handle();
		if (bytesRead == 0)
			cleanClose();
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		m_curEventManager->delEvent(m_readEvent);
		mf_closeFd(m_ChildToParent[0]);
		m_curRequestData->accessEventHandler(E_CGI_ON_CLOSE).handle();
	}
}

void	CgiModule::InternalCgiWorker::cleanClose()
{
	int status;

	if (m_pid != -1)
	{
		::waitpid(m_pid, &status, 0);
		m_pid = -1;
		m_curRequestData->accessEventHandler(E_CGI_ON_CLOSE).handle();
	}
	m_CgiModule.mf_returnWorker(*this);
}

void	CgiModule::InternalCgiWorker::forcedClose()
{
	if (m_pid != -1)
	{
		::kill(m_pid, SIGKILL);
		::waitpid(m_pid, NULL, 0);
		m_pid = -1;	
	}
	m_CgiModule.mf_returnWorker(*this);
}

CgiRequestData*	CgiModule::InternalCgiWorker::accessCurRequestData()
{
	return (m_curRequestData);
}

void 	CgiModule::InternalCgiWorker::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}


CgiModule::InternalCgiWorker::InternalCgiWorker(const InternalCgiWorker &other) :
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_CgiModule(other.m_CgiModule),
	m_globals(other.m_globals)
{
    *this = other;
}

CgiModule::InternalCgiWorker& CgiModule::InternalCgiWorker::operator=(const InternalCgiWorker &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
