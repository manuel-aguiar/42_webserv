

// Project Headers
#include "Worker.hpp"
#include "../../Events/Subscription/Subscription.hpp"
#include "../../Events/Manager/Manager.hpp"

// C++ headers
# include <cstring> // memset

// C Headers
# include <unistd.h>
# include <sys/wait.h>

Worker::Worker(ImplModule& cgi) :
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
	
	m_EmergencyEvent = mf_accessEventManager().acquireSubscription();
	m_EmergencyEvent->setFd(-1);
	m_EmergencyEvent->setUser(this);
	m_EmergencyEvent->setCallback(mf_EventCallback_OnEmergency);
	m_EmergencyEvent->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);

	m_readEvent = mf_accessEventManager().acquireSubscription();
	m_readEvent->setFd(-1);
	m_readEvent->setUser(this);
	m_readEvent->setCallback(mf_EventCallback_onRead);
	m_readEvent->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);

	m_writeEvent = mf_accessEventManager().acquireSubscription();
	m_writeEvent->setFd(-1);
	m_writeEvent->setUser(this);
	m_writeEvent->setCallback(mf_EventCallback_onWrite);
	m_writeEvent->setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);

	m_EmergencyBytesRead = 0;
	std::memset(m_EmergencyBuffer, 0, sizeof(m_EmergencyBuffer));
}

Worker::~Worker()
{

}

void    Worker::reset()
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


void	Worker::stop()
{
	mf_KillWaitChild();
}

void	Worker::assignRequestData(InternalReq& data)
{
	m_curRequestData = &data;
}

InternalReq*	Worker::accessRequestData()
{
	return (m_curRequestData);
}

void
Worker::disableReadEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(*m_readEvent, markAsStale);
}

void
Worker::disableWriteEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(*m_writeEvent, markAsStale);
}

void
Worker::disableEmergencyEvent(bool markAsStale)
{
	mf_disableCloseMyEvent(*m_EmergencyEvent, markAsStale);
}

void
Worker::disableReadHandler()
{
	m_readEvent->setUser(NULL);
	m_readEvent->setCallback(NULL);
}

void
Worker::disableWriteHandler()
{
	m_writeEvent->setUser(NULL);
	m_writeEvent->setCallback(NULL);
}

void
Worker::disableEmergencyHandler()
{
	m_EmergencyEvent->setUser(NULL);
	m_EmergencyEvent->setCallback(NULL);
}

void
Worker::disableAllHandlers()
{
	disableReadHandler();
	disableWriteHandler();
	disableEmergencyHandler();
}


void
Worker::enableReadHandler()
{
	m_readEvent->setUser(this);
	m_readEvent->setCallback(mf_EventCallback_onRead);
}

void
Worker::enableWriteHandler()
{
	m_writeEvent->setUser(this);
	m_writeEvent->setCallback(mf_EventCallback_onWrite);
}

void
Worker::enableEmergencyHandler()
{
	m_EmergencyEvent->setUser(this);
	m_EmergencyEvent->setCallback(mf_EventCallback_OnEmergency);
}

void
Worker::enableAllHandlers()
{
	enableReadHandler();
	enableWriteHandler();
	enableEmergencyHandler();
}

void	Worker::mf_disableCloseMyEvent(Events::Subscription& myEvent, bool markAsStale)
{
	Ws::fd fd = myEvent.getFd();

	if (fd == -1)
		return ;
	mf_accessEventManager().stopMonitoring(myEvent, markAsStale);
	::close(fd);
	myEvent.setFd(-1);
}

// private, bare minimum to compile
Worker::Worker(const Worker &other) :
	m_curRequestData(other.m_curRequestData),
	m_pid(other.m_pid),
	m_CgiModule(other.m_CgiModule) {}

Worker& Worker::operator=(const Worker &assign){(void)assign; return (*this);}
