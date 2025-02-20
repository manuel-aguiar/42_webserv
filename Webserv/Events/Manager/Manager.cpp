

# include "Manager.hpp"
# include "../Subscription/Subscription.hpp"
# include "../InternalSub/InternalSub.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"


// C++ headers
# include <cerrno>
# include <stdexcept> // std::runtime_error

// C headers
# include <unistd.h>

namespace Events
{
	Manager::Manager(const size_t maxSubscriptions, Globals& globals, const size_t maxFdsEstimate) :
		m_monitoringCount(0),
		m_epollfd		(-1),
		m_globals		(globals),
		m_subscriptions (maxSubscriptions, InternalSub()),
		m_availableSubs	(maxSubscriptions),
		m_fdTracker		(maxFdsEstimate)
	{
		m_epollfd = ::epoll_create(1);

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

		for (size_t i = 0 ; i < m_subscriptions.size(); ++i)
			m_availableSubs.emplace_back(&m_subscriptions[i]);
	}

	Events::Subscription*
	Manager::acquireSubscription()
	{
		//check if there are available subscriptions
		InternalSub* subscription;

		ASSERT_EQUAL(m_availableSubs.size() > 0, true, "Manager::acquireSubscription(): No available subscriptions");
		
		subscription = m_availableSubs.front();
		m_availableSubs.pop_front();
		return (subscription);
	}

	void
	Manager::returnSubscription(Subscription& subscription)
	{
		InternalSub* internal;

		internal = static_cast<InternalSub*>(&subscription);

		//subscription must be part of the manager's subscriptions
		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
				true, "Manager::returnSubscription(): Subscription is not part of the EventManager's subscriptions");

		//event must not being monitored
		ASSERT_EQUAL(internal->isSubscribed(), false,  
				"Manager::returnSubscription(): Subscription is still being monitored, stopMonitoring before attempting to return");

		internal->reset();

		m_availableSubs.emplace_back(internal);
	}

	Manager::~Manager()
	{
		if (m_epollfd != Ws::FD_NONE)
		{
			if (::close(m_epollfd) == -1)
			{
				m_globals.logError("Manager::~Manager, close(): " + std::string(strerror(errno)));
			}
		}
	}



	int
	Manager::startMonitoring(Subscription& event, bool markAsStale)
	{
		Ws::fd			fd;
		InternalSub*	internal;
		Events::EpollEvent	epollEvent = (Events::EpollEvent){};
		
		internal = static_cast<InternalSub*>(&event);
		fd = internal->getFd();

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
				true, "Manager::startMonitoring(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->getFd() == Ws::FD_NONE, false, "Manager::startMonitoring(): Subscription passed has fd = Ws::FD_NONE");
		ASSERT_EQUAL(internal->isSubscribed(), false, "Manager::startMonitoring(): Subscription is already being monitored");
		ASSERT_EQUAL(internal->getMonitoredEvents() != Events::Monitor::NONE, true, "Manager::startMonitoring(): Subscription has no events to monitor");

		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;

		if (::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &epollEvent) == -1)
		{
			ASSERT_EQUAL(true, false, std::string("Manager::startMonitoring(): Subscription data is not valid for epoll monitoring: ") + std::string(strerror(errno)));
			m_globals.logError("Manager::startMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}

		if (markAsStale)
			m_fdTracker.markFdStale(fd);
		internal->updateSubscription();

		++m_monitoringCount;

		return (1);
	}

	int
	Manager::updateEvents(Subscription& event, bool markAsStale)
	{
		Ws::fd			fd;
		InternalSub*	internal;
		Events::EpollEvent	epollEvent = (Events::EpollEvent){};

		internal = static_cast<InternalSub*>(&event);
		fd = internal->getFd();

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
				true, "Manager::updateEvents(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->getFd() == Ws::FD_NONE, false, "Manager::updateEvents(): Subscription passed has fd = Ws::FD_NONE");
		ASSERT_EQUAL(internal->isSubscribed(), true, "Manager::updateEvents(): Subscription is not being monitored");
		ASSERT_EQUAL(internal->getMonitoredEvents() != Events::Monitor::NONE, true, "Manager::startMonitoring(): Subscription has no events to monitor");

		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;
		
		
		if (::epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &epollEvent) == -1)
		{
			ASSERT_EQUAL(true, false, std::string("Manager::updateEvents(): Subscription data is not valid for epoll monitoring: ") + std::string(strerror(errno)));
			m_globals.logError("Manager::stopMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}

		if (markAsStale)
			m_fdTracker.markFdStale(fd);
			
		internal->updateSubscription();

		return (1);
	}

	int
	Manager::stopMonitoring(Subscription& event, bool markAsStale)
	{
		Ws::fd			fd;
		InternalSub*	internal;
		
		internal = static_cast<InternalSub*>(&event);
		fd = internal->getFd();

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
				true, "Manager::stopMonitoring(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->isSubscribed(), true, "Manager::stopMonitoring(): Subscription is not being monitored");

		if (::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
		{
			ASSERT_EQUAL(true, false, std::string("Manager::stopMonitoring(): Subscription data is not valid for epoll monitoring: ") + strerror(errno));
			m_globals.logError("Manager::stopMonitoring, epoll_ctl(): " + std::string(strerror(errno)));
			return (0);
		}
		
		if (markAsStale)
			m_fdTracker.markFdStale(fd);
		internal->unSubscribe();

		--m_monitoringCount;

		return (1);
	}

	int
	Manager::ProcessEvents(int timeOut)
	{
		InternalSub*	event;
		int				waitCount;

		if (m_monitoringCount == 0)
			return (0);

		waitCount = ::epoll_wait(m_epollfd, m_epollEvents, MAX_EPOLL_EVENTS, timeOut);

		if (waitCount == -1)
		{
			m_globals.logError("Manager::ProcessEvents, epoll_wait(): " + std::string(strerror(errno)));
			return (waitCount);
		}
		
		for (int i = 0; i < waitCount; i++)
		{
			event = static_cast<InternalSub*>(m_epollEvents[i].data.ptr);

			if (event->isInvalid() 
			|| m_fdTracker.isFdStale(event->getFd()))
				continue ;
			event->setTriggeredEvents(m_epollEvents[i].events);
			event->notify();
		}

		m_fdTracker.clear();
		return (waitCount);
	}

	size_t
	Manager::getMonitoringCount() const
	{
		return (m_monitoringCount);
	}

	//private, bare minimum to compile
	Manager::Manager(const Manager& copy) :
		m_globals		(copy.m_globals),
		m_fdTracker		(copy.m_fdTracker)
	{}

	// private, bare minimum to compile
	Manager& Manager::operator=(const Manager& assign) {(void)assign; return (*this);}
}