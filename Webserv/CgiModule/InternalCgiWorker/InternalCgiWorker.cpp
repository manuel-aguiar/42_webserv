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
#include "../../Globals/Globals.hpp"
#include "../../ServerManager/EventManager/EventManager.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

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
	m_EmergencyEvent.setMonitoredFlags(EPOLLIN);
	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
}

CgiModule::InternalCgiWorker::~InternalCgiWorker()
{

}

void    CgiModule::InternalCgiWorker::reset()
{
	m_pid = -1;

	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ParentToChild[1]);
	mf_closeFd(m_ChildToParent[0]);
	mf_closeFd(m_ChildToParent[1]);

	m_argPtr.clear();
	m_envPtr.clear();
	m_envStr.clear();

	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
	
	if (m_curRequestData)
	{
		mf_disableEmergencyEvent();
		m_CgiModule.mf_returnRequestData(*m_curRequestData);
	}
	
	mf_closeFd(m_EmergencyPhone[0]);
	mf_closeFd(m_EmergencyPhone[1]);
}


void	CgiModule::InternalCgiWorker::KillExecution()
{
	mf_KillWaitChild();
	m_CgiModule.mf_returnWorker(*this);
}

CgiModule::InternalCgiRequestData*	CgiModule::InternalCgiWorker::accessCurRequestData()
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
