

# include "Manager.hpp"
# include "../Subscription/Subscription.hpp"
# include "../InternalSub/InternalSub.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

namespace Events
{
	Manager::Manager(size_t maxSubscriptions, Globals& globals) :
		m_subscribeCount(0),
		m_epollfd		(-1),
		m_maxStaleFd	(0),
		m_globals		(globals),
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
		//check if there are available subscriptions
		ASSERT_EQUAL(m_availableSubs.size() > 0, true, "Manager::acquireSubscription(): No available subscriptions");
		InternalSub* subscription;
		
		subscription = m_availableSubs.front();
		m_availableSubs.pop_front();
		return (subscription);
	}

	void	Manager::returnSubscription(Subscription& subscription)
	{
		InternalSub* internal;

		internal = static_cast<InternalSub*>(&subscription);

		//subscription must be part of the manager's subscriptions
		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
		true, "Manager::returnSubscription(): Subscription is not part of the EventManager's subscriptions");

		//event must not being monitored
		ASSERT_EQUAL(internal->getSubscribedFd() == -1 && internal->getSubscribedEvents() == Events::Monitor::NONE,
		true, "Manager::returnSubscription(): Subscription is still being monitored");

		internal->reset();

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
		fd = internal->getFd();

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
		true, "Manager::startMonitoring(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->getFd() == -1, false, "Manager::startMonitoring(): Subscription passed has fd = -1");
		ASSERT_EQUAL(internal->getState() == Subscription::UNSUBSCRIBED, true, "Manager::startMonitoring(): Subscription is already being monitored");
		ASSERT_EQUAL(internal->getMonitoredEvents() != Events::Monitor::NONE, true, "Manager::startMonitoring(): Subscription has no events to monitor");

		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;

		if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &epollEvent) == -1)
		{
			ASSERT_EQUAL(true, false, "Manager::startMonitoring(): Subscription data is not valid for epoll monitoring");
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
		fd = internal->getFd();

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
		true, "Manager::updateEvents(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->getFd() == -1, false, "Manager::updateEvents(): Subscription passed has fd = -1");
		ASSERT_EQUAL(internal->getState() == Subscription::SUBSCRIBED, true, "Manager::updateEvents(): Subscription is not being monitored");


		epollEvent.events = internal->getMonitoredEvents();
		epollEvent.data.ptr = (void *)internal;
		
		
		if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &epollEvent) == -1)
		{
			ASSERT_EQUAL(true, false, "Manager::updateEvents(): Subscription data is not valid for epoll monitoring");
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

		ASSERT_EQUAL(internal >= m_subscriptions.getArray() && internal < m_subscriptions.getArray() + m_subscriptions.size(), 
		true, "Manager::stopMonitoring(): Subscription is not part of the EventManager's subscriptions");
		ASSERT_EQUAL(internal->getFd() == -1, false, "Manager::stopMonitoring(): Subscription passed has fd = -1");
		ASSERT_EQUAL(internal->getState() == Subscription::SUBSCRIBED, true, "Manager::stopMonitoring(): Subscription is not being monitored");


		if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
		{
			ASSERT_EQUAL(true, false, "Manager::stopMonitoring(): Subscription data is not valid for epoll monitoring");
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