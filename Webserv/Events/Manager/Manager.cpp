

# include "Manager.hpp"
# include "../Subscription/Subscription.hpp"
# include "../InternalSub/InternalSub.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
#include <iostream>

namespace Events
{
	Manager::Manager(size_t maxSubscriptions, Globals& globals) :
		m_subscribeCount(0),
		m_epollfd		(-1),
		m_globals		(globals),
		m_maxStaleFd	(0),
		m_subscriptions (maxSubscriptions, InternalSub()),
		m_availableSubs	(maxSubscriptions),
		m_staleEvents	((maxSubscriptions * 10) / 8 + 1, 0)
	{

		m_epollfd = epoll_create(1);

		if (m_epollfd == -1)
		{
			m_globals.logError("epoll_create(): " + std::string(strerror(errno)));
			throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
		}

		if (!FileDescriptor::setCloseOnExec_NonBlocking(m_epollfd))
		{
			m_globals.logError("fcntl(): " + std::string(strerror(errno)));
			throw std::runtime_error("setCloseOnExec(), critical error: " + std::string(strerror(errno)));
		}

		std::memset(m_staleEvents.getArray(), 0, sizeof(m_staleEvents[0]) * m_staleEvents.size());

		for (size_t i = 0 ; i < m_subscriptions.size(); ++i)
			m_availableSubs.emplace_back(&m_subscriptions[i]);
	}

	Events::Subscription*
	Manager::acquireSubscription()
	{
		assert (m_availableSubs.size() > 0);
		InternalSub* subscription;
		
		subscription = m_availableSubs.front();
		m_availableSubs.pop_front();

		return (subscription);
	}

	void	Manager::returnSubscription(Subscription& subscription)
	{
		InternalSub* internal;

		internal = static_cast<InternalSub*>(&subscription);

		//event must not be subscribed 
		assert(internal->getSubscribedFd() == -1 && internal->getSubscribedEvents() == Events::Monitor::NONE);
		m_availableSubs.emplace_back(internal);
	}

	void Manager::mf_markFdStale(t_fd fd)
	{
		m_maxStaleFd = (fd > m_maxStaleFd) ? fd : m_maxStaleFd;
		size_t index = fd / 8;
		size_t bit = fd % 8;
		m_staleEvents[index] |= (1 << bit);
	}

	int	Manager::mf_isFdStale(t_fd fd)
	{
		if (fd == -1)
			return (1);
		size_t index = fd / 8;
		size_t bit = fd % 8;
		return ((m_staleEvents[index] & (1 << bit)) != 0);
	}

	Manager::~Manager()
	{
		if (m_epollfd != -1)
		{
			if (close(m_epollfd) == -1)
			{
				m_globals.logError("Manager::~Manager, close(): " + std::string(strerror(errno)));
			}
		}
	}



	int                Manager::startMonitoring(Subscription& event, bool markAsStale)
	{
		t_fd			fd;
		InternalSub*	internal;
		t_epoll_event	epollEvent = (t_epoll_event){};
		
		internal = static_cast<InternalSub*>(&event);
		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;

		fd = internal->getFd();
		assert(fd != -1);

		if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &epollEvent) == -1)
		{
			assert(false); // this should never happen
			m_globals.logError("Manager::startMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}

		if (markAsStale)
			mf_markFdStale(fd);
		internal->updateSubscription();

		++m_subscribeCount;

		return (1);
	}

	int                Manager::updateEvents(Subscription& event, bool markAsStale)
	{
		t_fd			fd;
		InternalSub*	internal;
		t_epoll_event	epollEvent = (t_epoll_event){};

		internal = static_cast<InternalSub*>(&event);
		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;
		
		fd = internal->getFd();
		
		// check the subscription fd is the same but the events are different
		assert(fd == internal->getSubscribedFd() && internal->getSubscribedEvents() != internal->getMonitoredEvents());

		if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &epollEvent) == -1)
		{
			assert(false); // this should never happen
			m_globals.logError("Manager::stopMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}

		if (markAsStale)
			mf_markFdStale(fd);
			
		internal->updateSubscription();

		return (1);
	}

	int                 Manager::stopMonitoring(Subscription& event, bool markAsStale)
	{
		t_fd			fd;
		InternalSub*	internal;
		
		internal = static_cast<InternalSub*>(&event);
		fd = internal->getFd();

		assert(fd != -1);

		if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
		{
			assert(false); // this should never happen
			m_globals.logError("Manager::stopMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}
		
		if (markAsStale)
			mf_markFdStale(fd);
		internal->unSubscribe();

		--m_subscribeCount;

		return (1);
	}

	int                 Manager::ProcessEvents(int timeOut)
	{
		InternalSub*	event;
		int				waitCount;

		waitCount = epoll_wait(m_epollfd, m_epollEvents, MAX_EPOLL_EVENTS, timeOut);

		if (waitCount == -1)
		{
			m_globals.logError("Manager::ProcessEvents, epoll_wait(): " + std::string(strerror(errno)));
			return (waitCount);
		}
		
		for (int i = 0; i < waitCount; i++)
		{
			event = static_cast<InternalSub*>(m_epollEvents[i].data.ptr);

			if (event->isInvalid() 
			|| mf_isFdStale(event->getFd()))
				continue ;
			event->setTriggeredEvents(m_epollEvents[i].events);
			event->notify();
		}

		std::memset(m_staleEvents.getArray(), 0, sizeof(m_staleEvents[0]) * (((m_maxStaleFd / 8)) + 1));
		m_maxStaleFd = 0;
		return (waitCount);
	}

	size_t			Manager::getMonitoringCount() const
	{
		return (m_subscribeCount);
	}

	//private, bare minimum to compile
	Manager::Manager(const Manager& copy) :
		m_globals		(copy.m_globals)
	{}

	// private, bare minimum to compile
	Manager& Manager::operator=(const Manager& assign) {(void)assign; return (*this);}
}