

// Project Headers
#include "CgiWorker.hpp"
#include "../CgiInternalRequest/CgiInternalRequest.hpp"
#include "../../Globals/Globals.hpp"
#include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

//#include "../python-cgi/pythonCgi.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

CgiModule::Worker::Worker(CgiModule& cgi) :
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
	m_EmergencyEvent.setMonitoredFlags(EPOLLIN | EPOLLERR | EPOLLHUP);

	m_readEvent.setFd(-1);
	m_readEvent.setCallback(this, mf_EventCallback_onRead);
	m_readEvent.setMonitoredFlags(EPOLLIN | EPOLLERR | EPOLLHUP);

	m_writeEvent.setFd(-1);
	m_writeEvent.setCallback(this, mf_EventCallback_onWrite);
	m_writeEvent.setMonitoredFlags(EPOLLOUT | EPOLLERR | EPOLLHUP);

	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
}

CgiModule::Worker::~Worker()
{

}

void    CgiModule::Worker::reset()
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


void	CgiModule::Worker::stop()
{
	mf_KillWaitChild();
}

void	CgiModule::Worker::assignRequestData(InternalRequest& data)
{
	m_curRequestData = &data;
}

CgiModule::InternalRequest*	CgiModule::Worker::accessRequestData()
{
	return (m_curRequestData);
}

void
CgiModule::Worker::disableReadEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_readEvent, markAsStale);
}

void
CgiModule::Worker::disableWriteEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_writeEvent, markAsStale);
}

void
CgiModule::Worker::disableEmergencyEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(m_EmergencyEvent, markAsStale);
}

void
CgiModule::Worker::disableReadHandler()
{
	m_readEvent.setCallback(NULL, NULL);
}

void
CgiModule::Worker::disableWriteHandler()
{
	m_writeEvent.setCallback(NULL, NULL);
}

void
CgiModule::Worker::disableEmergencyHandler()
{
	m_EmergencyEvent.setCallback(NULL, NULL);
}

void
CgiModule::Worker::disableAllHandlers()
{
	disableReadHandler();
	disableWriteHandler();
	disableEmergencyHandler();
}


void
CgiModule::Worker::enableReadHandler()
{
	m_readEvent.setCallback(this, mf_EventCallback_onRead);
}

void
CgiModule::Worker::enableWriteHandler()
{
	m_writeEvent.setCallback(this, mf_EventCallback_onWrite);
}

void
CgiModule::Worker::enableEmergencyHandler()
{
	m_EmergencyEvent.setCallback(this, mf_EventCallback_OnEmergency);
}

void
CgiModule::Worker::enableAllHandlers()
{
	enableReadHandler();
	enableWriteHandler();
	enableEmergencyHandler();
}

// private, bare minimum to compile
CgiModule::Worker::Worker(const Worker &other) :
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_CgiModule(other.m_CgiModule) {}

CgiModule::Worker& CgiModule::Worker::operator=(const Worker &assign){(void)assign; return (*this);}
