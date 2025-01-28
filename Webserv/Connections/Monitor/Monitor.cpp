
# include "Monitor.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

# include <cstddef>

Monitor::Monitor(Events::Manager& eventManager) : 
	m_eventSubs(NULL), 
	m_eventManager(eventManager),
	m_state(Monitor::UNSUBSCRIBED)
{

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
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::release() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_state == Monitor::UNSUBSCRIBED, true, "Monitor::release() called on a Monitor that is still subscribed");
	
	m_eventManager.returnSubscription(*m_eventSubs);
}

void
Monitor::subscribe(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_state, Monitor::UNSUBSCRIBED, "Monitor::subscribe() called on an already subscribed Monitor");

	m_eventManager.startMonitoring(*m_eventSubs, isCalledFromEventLoop);
	m_state = Monitor::SUBSCRIBED;
}

void
Monitor::unsubscribe(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_state, Monitor::SUBSCRIBED, "Monitor::subscribe() called on an already subscribed Monitor");

	m_eventManager.stopMonitoring(*m_eventSubs, isCalledFromEventLoop);
	m_state = Monitor::UNSUBSCRIBED;
}

void
Monitor::modify(bool isCalledFromEventLoop)
{
	ASSERT_EQUAL(m_eventSubs != NULL, true, "Monitor::subscribe() called on a Monitor with a NULL Subscription");
	ASSERT_EQUAL(m_state, Monitor::SUBSCRIBED, "Monitor::subscribe() called on an already subscribed Monitor");

	m_eventManager.updateEvents(*m_eventSubs, isCalledFromEventLoop);
}

Events::Subscription*
Monitor::accessEvent()
{
	return (m_eventSubs);
}


Monitor::Monitor(const Monitor& copy) : 
	m_eventSubs(copy.m_eventSubs), 
	m_eventManager(copy.m_eventManager),
	m_state(copy.m_state) {}

Monitor&
Monitor::operator=(const Monitor& assign)
{
	if (this == &assign)
		return (*this);
	return *this;
}

void
Monitor::reset(bool isCalledFromEventLoop)
{
	if (m_state == Monitor::SUBSCRIBED)
		unsubscribe(isCalledFromEventLoop);
	m_eventSubs->reset();
}