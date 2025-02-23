

# include "Monitor.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"


# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

Monitor::Monitor(Events::Manager& eventManager) : 
	m_eventSubs(NULL),
	m_eventManager(eventManager) {}

Monitor::Monitor(const Monitor& copy) : 
	m_eventSubs(copy.m_eventSubs),
	m_eventManager(copy.m_eventManager)
{
	ASSERT_EQUAL(copy.m_eventSubs == NULL, true, "Monitor::Monitor(const Monitor& copy) from a Monitor that still has a subscription");
}


Monitor::~Monitor()
{
	release();
}

void
Monitor::acquire()
{
	ASSERT_EQUAL(m_eventSubs == NULL, true, "Monitor::acquire() called on a Monitor thal already has a subscription");
	
	m_eventSubs = m_eventManager.acquireSubscription();
}

void
Monitor::release()
{
	if (m_eventSubs == NULL)
		return ;

	reset(true);
	m_eventManager.returnSubscription(*m_eventSubs);
	m_eventSubs = NULL;
}

void
Monitor::subscribe(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_eventSubs->isSubscribed(), false, "Monitor::subscribe() called on a Monitor with a Subscription that is already subscribed");
	m_eventManager.startMonitoring(*m_eventSubs, isCalledFromEventLoop);
}

void
Monitor::unsubscribe(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	if (m_eventSubs->isSubscribed())
		m_eventManager.stopMonitoring(*m_eventSubs, isCalledFromEventLoop);
}

void
Monitor::modify(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_eventSubs->isSubscribed(), true, "Monitor::subscribe() called on an already subscribed Monitor");

	m_eventManager.updateEvents(*m_eventSubs, isCalledFromEventLoop);
}

Events::Subscription&
Monitor::accessEvent()
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::accessEvent() accessing event when none was acquired");
	return (*m_eventSubs);
}

const Events::Subscription&
Monitor::getEvent() const
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::accessEvent() accessing event when none was acquired");
	return (*m_eventSubs);
}

void
Monitor::reset(bool isCalledFromEventLoop)
{
	if (!m_eventSubs)
		return;
	
	if (m_eventSubs->isSubscribed())
		unsubscribe(isCalledFromEventLoop);
	m_eventSubs->reset();
}

Events::Manager&
Monitor::accessEventManager()
{
	return (m_eventManager);
}

const Events::Manager&
Monitor::getEventManager() const
{
	return (m_eventManager);
}

Monitor&
Monitor::operator=(const Monitor& assign)
{
	ASSERT_EQUAL(false, true, "Monitor::operator= called, monitor is not assignable");

	if (this == &assign)
		return (*this);
	return (*this);
}
