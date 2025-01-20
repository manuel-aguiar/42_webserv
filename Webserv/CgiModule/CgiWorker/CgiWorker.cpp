

// Project Headers
#include "CgiWorker.hpp"
#include "../CgiInternalRequest/CgiInternalRequest.hpp"
#include "../../Globals/Globals.hpp"
#include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

namespace Cgi
{

Module::Worker::Worker(Module& cgi) :
	m_curRequestData	(NULL),
	m_pid				(-1),
	m_CgiModule			(cgi)
{
	m_ParentToChild[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
	m_ChildToParent[1] = -1;
	m_EmergencyPhone[0] = -1;
	m_EmergencyPhone[1] = -1;
	
	m_EmergencyEvent.setFd(-1);
	m_EmergencyEvent.setCallback(this, mf_EventCallback_OnEmergency);
	m_EmergencyEvent.setMonitoredFlags(Ws::Epoll::READ | Ws::Epoll::ERROR | Ws::Epoll::HANGUP);

	m_readEvent.setFd(-1);
	m_readEvent.setCallback(this, mf_EventCallback_onRead);
	m_readEvent.setMonitoredFlags(Ws::Epoll::READ | Ws::Epoll::ERROR | Ws::Epoll::HANGUP);

	m_writeEvent.setFd(-1);
	m_writeEvent.setCallback(this, mf_EventCallback_onWrite);
	m_writeEvent.setMonitoredFlags(Ws::Epoll::WRITE | Ws::Epoll::ERROR | Ws::Epoll::HANGUP);

	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
}

Module::Worker::~Worker()
{

}

void    Module::Worker::reset()
{
	m_pid = -1;
	
	// close all pipes
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

	enableAllHandlers();
}


void	Module::Worker::stop()
{
	mf_KillWaitChild();
}

void	Module::Worker::assignRequestData(InternalRequest& data)
{
	m_curRequestData = &data;
}

Module::InternalRequest*	Module::Worker::accessRequestData()
{
	return (m_curRequestData);
}

void
Module::Worker::disableReadEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_readEvent, markAsStale);
}

void
Module::Worker::disableWriteEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_writeEvent, markAsStale);
}

void
Module::Worker::disableEmergencyEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_EmergencyEvent, markAsStale);
}

void
Module::Worker::disableReadHandler()
{
	m_readEvent.setCallback(NULL, NULL);
}

void
Module::Worker::disableWriteHandler()
{
	m_writeEvent.setCallback(NULL, NULL);
}

void
Module::Worker::disableEmergencyHandler()
{
	m_EmergencyEvent.setCallback(NULL, NULL);
}

void
Module::Worker::disableAllHandlers()
{
	disableReadHandler();
	disableWriteHandler();
	disableEmergencyHandler();
}


void
Module::Worker::enableReadHandler()
{
	m_readEvent.setCallback(this, mf_EventCallback_onRead);
}

void
Module::Worker::enableWriteHandler()
{
	m_writeEvent.setCallback(this, mf_EventCallback_onWrite);
}

void
Module::Worker::enableEmergencyHandler()
{
	m_EmergencyEvent.setCallback(this, mf_EventCallback_OnEmergency);
}

void
Module::Worker::enableAllHandlers()
{
	enableReadHandler();
	enableWriteHandler();
	enableEmergencyHandler();
}

// private, bare minimum to compile
Module::Worker::Worker(const Worker &other) :
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_CgiModule(other.m_CgiModule) {}

Module::Worker& Module::Worker::operator=(const Worker &assign){(void)assign; return (*this);}

}; // namespace Cgi