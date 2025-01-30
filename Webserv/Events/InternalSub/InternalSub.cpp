

#include "InternalSub.hpp"
#include <iostream>
InternalSub::InternalSub() : 
	Events::Subscription(),
	m_subscribedFd(Ws::FD_NONE),
	m_subscribedEvents(Events::Monitor::NONE) {}

InternalSub::~InternalSub() {}

InternalSub::InternalSub(const InternalSub& copy) : 
	Events::Subscription(copy), 
	m_subscribedFd(copy.m_subscribedFd), 
	m_subscribedEvents(copy.m_subscribedEvents) {}

InternalSub&
InternalSub::operator=(const InternalSub& assign)
{
	if (this == &assign)
		return (*this);
	Events::Subscription::operator=(assign);
	m_subscribedFd = assign.m_subscribedFd;
	m_subscribedEvents = assign.m_subscribedEvents;
	return (*this);
};

void
InternalSub::reset()
{
	Events::Subscription::reset();
	m_subscribedFd = Ws::FD_NONE;
	m_subscribedEvents = Events::Monitor::NONE;
}

void
InternalSub::updateSubscription()
{
	this->setState(Events::Subscription::SUBSCRIBED);
	m_subscribedFd = m_fdmask.getFd();
	m_subscribedEvents = m_monitoredEvents;
}

void
InternalSub::unSubscribe()
{
	this->setState(Events::Subscription::UNSUBSCRIBED);
	m_subscribedFd = Ws::FD_NONE;
	m_subscribedEvents = Events::Monitor::NONE;
}

bool
InternalSub::isInvalid() const
{
	Ws::fd fd = m_fdmask.getFd();
	return (!(m_subscribedFd == fd && fd != Ws::FD_NONE));
}

void
InternalSub::setTriggeredEvents(const Events::Monitor::Mask flags)
{
	m_triggeredEvents = flags;
}

void
InternalSub::setSubscribedFd(const Ws::fd fd)
{
	m_subscribedFd = fd;
}

void
InternalSub::setSubscribedEvents(const Events::Monitor::Mask flags)
{
	m_subscribedEvents = flags;
}

Ws::fd
InternalSub::getSubscribedFd() const
{
	return (m_subscribedFd);
}

int
InternalSub::getSubscribedEvents() const 
{
	return (m_subscribedEvents);
}

Events::Subscription::State
InternalSub::getState() const
{
	return (m_fdmask.getState());
}

void
InternalSub::setState(const Events::Subscription::State state)
{
	m_fdmask.setState(state);
}
