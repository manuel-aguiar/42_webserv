

#include "InternalSub.hpp"

InternalSub::InternalSub() : 
    Events::Subscription(),
    m_subscribedFd(-1),
    m_subscribedEvents(Events::Monitor::NONE) {}

InternalSub::~InternalSub() {}

InternalSub::InternalSub(const InternalSub& copy) : 
    Events::Subscription(copy), 
    m_subscribedFd(copy.m_subscribedFd), 
    m_subscribedEvents(copy.m_subscribedEvents) {}

InternalSub& InternalSub::operator=(const InternalSub& assign)
{
    if (this == &assign)
        return (*this);
    Events::Subscription::operator=(assign);
    m_subscribedFd = assign.m_subscribedFd;
    m_subscribedEvents = assign.m_subscribedEvents;
    return (*this);
};

void InternalSub::updateSubscription()
{
    m_subscribedFd = m_fd;
    m_subscribedEvents = m_monitoredEvents;
}

void    InternalSub::unSubscribe()
{
    m_subscribedFd = -1;
    m_subscribedEvents = Events::Monitor::NONE;
}

bool InternalSub::isInvalid() const
{

    return (!(m_subscribedFd == m_fd && m_fd != -1));
}

void    InternalSub::setTriggeredEvents(const Events::Monitor::Mask flags)
{
    m_triggeredEvents = flags;
}

void InternalSub::setSubscribedFd(const Ws::fd fd)
{
    m_subscribedFd = fd;
}

void InternalSub::setSubscribedEvents(const Events::Monitor::Mask flags)
{
    m_subscribedEvents = flags;
}

Ws::fd InternalSub::getSubscribedFd() const
{
    return (m_subscribedFd);
}

int InternalSub::getSubscribedEvents() const 
{
    return (m_subscribedEvents);
}
