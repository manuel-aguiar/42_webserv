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
#include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
#include "../Test_EventManager/Test_EventManager.hpp"
#include "../../../Globals/Globals.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"

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
	m_EmergencyPhone[0] = -1;
	m_EmergencyPhone[1] = -1;
	
	m_EmergencyEvent.setCallback(this, mf_EventCallback_OnEmergency);
	m_EmergencyEvent.setMonitoredFlags(EPOLLIN | EPOLLERR | EPOLLHUP);

	m_readEvent.setCallback(this, mf_EventCallback_onRead);
	m_readEvent.setMonitoredFlags(EPOLLIN | EPOLLERR | EPOLLHUP);

	m_writeEvent.setCallback(this, mf_EventCallback_onWrite);
	m_writeEvent.setMonitoredFlags(EPOLLOUT | EPOLLET | EPOLLERR | EPOLLHUP);

	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
}

CgiModule::InternalCgiWorker::~InternalCgiWorker()
{

}

void    CgiModule::InternalCgiWorker::reset()
{
	m_pid = -1;

	// close all associated fds
	mf_disableMyEvent(m_EmergencyEvent);
	mf_disableMyEvent(m_readEvent);
	mf_disableMyEvent(m_writeEvent);
	
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ParentToChild[1]);
	mf_closeFd(m_ChildToParent[0]);
	mf_closeFd(m_ChildToParent[1]);
	mf_closeFd(m_EmergencyPhone[0]);
	mf_closeFd(m_EmergencyPhone[1]);

	// clear args for execve, keep size for reuse
	m_argPtr.clear();
	m_envPtr.clear();
	m_envStr.clear();

	// clear emergency buffer
	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
	
	m_curRequestData = NULL;
}


void	CgiModule::InternalCgiWorker::stop()
{
	mf_KillWaitChild();
}

void	CgiModule::InternalCgiWorker::assignRequestData(InternalCgiRequestData& data)
{
	m_curRequestData = &data;
}

CgiModule::InternalCgiRequestData*	CgiModule::InternalCgiWorker::accessRequestData()
{
	return (m_curRequestData);
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
