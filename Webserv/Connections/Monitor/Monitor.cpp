
# include "Monitor.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

# include <cstddef>

Monitor::Monitor() : 
	m_eventSubs(NULL)
{

}

Monitor::~Monitor()
{
	ASSERT_EQUAL(m_eventSubs == NULL, true, "Monitor::~Monitor() called on a Monitor that still has a subscription");
}



void
Monitor::acquire(Events::Manager& eventManager)
{
	ASSERT_EQUAL(m_eventSubs == NULL, true, "Monitor::acquire() called on a Monitor thal already has a subscription");
	
	m_eventSubs = eventManager.acquireSubscription();
}

void
Monitor::release(Events::Manager& eventManager)
{
	if (m_eventSubs == NULL)
		return ;

	reset(eventManager, true);
	eventManager.returnSubscription(*m_eventSubs);
	m_eventSubs = NULL;
}

void
Monitor::subscribe(Events::Manager& eventManager, bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_eventSubs->isSubscribed(), false, "Monitor::subscribe() called on a Monitor with a Subscription that is already subscribed");
	eventManager.startMonitoring(*m_eventSubs, isCalledFromEventLoop);
}

void
Monitor::unsubscribe(Events::Manager& eventManager, bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_eventSubs->isSubscribed(), true, "Monitor::subscribe() called on an already subscribed Monitor");

	eventManager.stopMonitoring(*m_eventSubs, isCalledFromEventLoop);
}

void
Monitor::modify(Events::Manager& eventManager, bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_eventSubs->isSubscribed(), true, "Monitor::subscribe() called on an already subscribed Monitor");

	eventManager.updateEvents(*m_eventSubs, isCalledFromEventLoop);
}

Events::Subscription*
Monitor::accessEvent()
{
	return (m_eventSubs);
}


Monitor::Monitor(const Monitor& copy) : 
	m_eventSubs(copy.m_eventSubs) {}

Monitor&
Monitor::operator=(const Monitor& assign)
{
	if (this == &assign)
		return (*this);
	return *this;
}

void
Monitor::reset(Events::Manager& eventManager, bool isCalledFromEventLoop)
{
	if (!m_eventSubs)
		return;
	
	if (m_eventSubs->isSubscribed())
		unsubscribe(eventManager, isCalledFromEventLoop);
	m_eventSubs->reset();
}