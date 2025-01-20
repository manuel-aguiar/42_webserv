

#include "InternalEvent.hpp"
#include <iostream>
#include <cstdlib>


EventManager::InternalEvent::InternalEvent() : Event() {}
EventManager::InternalEvent::~InternalEvent() {}

void EventManager::InternalEvent::updateSubscription()
{
    m_subscribedFd = m_fd;
    m_subscribedFlags = m_monitoredFlags;
}

void    EventManager::InternalEvent::unsubscribe()
{
    m_subscribedFd = -1;
    m_subscribedFlags = -1;
}

bool EventManager::InternalEvent::isInvalid() const
{

    return (!(m_subscribedFd == m_fd 
        && m_subscribedFlags == m_monitoredFlags
        && m_fd != -1));
}

void EventManager::InternalEvent::setSubscribedFd(Ws::fd fd)
{
    m_subscribedFd = fd;
}

void EventManager::InternalEvent::setSubscribedFlags(Ws::Epoll::Flags flags)
{
    m_subscribedFlags = flags;
}

Ws::fd EventManager::InternalEvent::getSubscribedFd() const
{
    return (m_subscribedFd);
}

int EventManager::InternalEvent::getSubscribedFlags() const 
{
    return (m_subscribedFlags);
}

// Private copy constructor (not implemented to prevent copying)
EventManager::InternalEvent::InternalEvent(const InternalEvent& copy) : Event(copy)
{
    (void)copy;
}
EventManager::InternalEvent& EventManager::InternalEvent::operator=(const InternalEvent& assign)
{Event::operator=(assign); return (*this);};