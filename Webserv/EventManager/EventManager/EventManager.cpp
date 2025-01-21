

# include "EventManager.hpp"
# include "../InternalEvent/InternalEvent.hpp"
# include "../../EventCallback/EventCallback.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

#include <iostream>

EventManager::EventManager(Globals& globals) :
	m_subscribeCount(0),
	m_epollfd		(-1),
	m_globals		(globals),
	m_maxStaleFd	(0)
{

	m_epollfd = epoll_create(1);

	if (m_epollfd == -1)
	{
		m_globals.logError("epoll_create(): " + std::string(strerror(errno)));
		throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
	}

	if (!FileDescriptor
::setCloseOnExec_NonBlocking(m_epollfd))
	{
		m_globals.logError("fcntl(): " + std::string(strerror(errno)));
		throw std::runtime_error("setCloseOnExec(), critical error: " + std::string(strerror(errno)));
	}

	std::memset(m_staleEvents, 0, sizeof(m_staleEvents));
}

void EventManager::mf_markFdStale(t_fd fd)
{
	//std::cout << ", fd marked as stale: " << fd;
	m_maxStaleFd = (fd > m_maxStaleFd) ? fd : m_maxStaleFd;
	size_t index = fd / 8;
	size_t bit = fd % 8;
    m_staleEvents[index] |= (1 << bit);
}

int	EventManager::mf_isFdStale(t_fd fd)
{
	if (fd == -1)
		return (1);
	size_t index = fd / 8;
	size_t bit = fd % 8;
	return ((m_staleEvents[index] & (1 << bit)) != 0);
}

EventManager::~EventManager()
{
	if (m_epollfd != -1)
	{
		if (close(m_epollfd) == -1)
		{
			m_globals.logError("EventManager::~EventManager, close(): " + std::string(strerror(errno)));
		}
	}
}



int                EventManager::addEvent(EventCallback& event, bool markAsStale)
{
	t_fd			fd;
	InternalEvent*	internal;
	t_epoll_event	epollEvent = (t_epoll_event){};
	
	internal = static_cast<InternalEvent*>(&event);
	epollEvent.events = internal->getMonitoredEvents();
	epollEvent.data.ptr = (void *)internal;

	fd = internal->getFd();
	assert(fd != -1);

	if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &epollEvent) == -1)
	{
		m_globals.logError("EventManager::addEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}

	if (markAsStale)
		mf_markFdStale(fd);
	internal->updateSubscription();

	++m_subscribeCount;

	return (1);
}

int                EventManager::modEvent(EventCallback& event, bool markAsStale)
{
	t_fd			fd;
	InternalEvent*	internal;
	t_epoll_event	epollEvent = (t_epoll_event){};

	internal = static_cast<InternalEvent*>(&event);
	epollEvent.events = internal->getMonitoredEvents();
	epollEvent.data.ptr = (void *)internal;
	
	fd = internal->getFd();
	
	if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &epollEvent) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}

	if (markAsStale)
		mf_markFdStale(fd);
		
	internal->updateSubscription();

	return (1);
}

int                 EventManager::delEvent(EventCallback& event, bool markAsStale)
{
	t_fd			fd;
	InternalEvent*	internal;
	
	internal = static_cast<InternalEvent*>(&event);
	fd = internal->getFd();

	assert(fd != -1);

	if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	
	if (markAsStale)
		mf_markFdStale(fd);
	internal->unsubscribe();

	--m_subscribeCount;

	return (1);
}

int                 EventManager::ProcessEvents(int timeOut)
{
	InternalEvent*	event;
	int				waitCount;

	waitCount = epoll_wait(m_epollfd, m_events, MAX_EPOLL_EVENTS, timeOut);

	if (waitCount == -1)
	{
		m_globals.logError("EventManager::ProcessEvents, epoll_wait(): " + std::string(strerror(errno)));
		return (waitCount);
	}
	
	for (int i = 0; i < waitCount; i++)
	{
		event = static_cast<InternalEvent*>(m_events[i].data.ptr);

		if (event->isInvalid() || mf_isFdStale(event->getFd()))
			continue ;
		event->setTriggeredFlags(m_events[i].events);
		event->handle();
	}

	std::memset(m_staleEvents, 0, ((m_maxStaleFd / 8)) + 1);
	m_maxStaleFd = 0;
	return (waitCount);
}

size_t			EventManager::getSubscribeCount() const
{
	return (m_subscribeCount);
}

//private, bare minimum to compile
EventManager::EventManager(const EventManager& copy) :
	m_globals		(copy.m_globals)
{}

// private, bare minimum to compile
EventManager& EventManager::operator=(const EventManager& assign) {(void)assign; return (*this);}