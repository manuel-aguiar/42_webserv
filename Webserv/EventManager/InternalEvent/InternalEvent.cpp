

#include "InternalEvent.hpp"
#include <iostream>
#include <cstdlib>


EventManager::InternalEvent::InternalEvent() : EventCallback() {}
EventManager::InternalEvent::~InternalEvent() {}

void EventManager::InternalEvent::updateSubscription()
{
    m_subscribedFd = m_fd;
    m_subscribedEvents = m_monitoredEvents;
}

void    EventManager::InternalEvent::unSubscribe()
{
    m_subscribedFd = -1;
    m_subscribedEvents = Ws::Epoll::NONE;
}

bool EventManager::InternalEvent::isInvalid() const
{

    return (!(m_subscribedFd == m_fd && m_fd != -1));
}

void    EventManager::InternalEvent::setTriggeredEvents(const Ws::Epoll::Events flags)
{
    m_triggeredEvents = flags;
}

void EventManager::InternalEvent::setSubscribedFd(const Ws::fd fd)
{
    m_subscribedFd = fd;
}

void EventManager::InternalEvent::setSubscribedEvents(const Ws::Epoll::Events flags)
{
    m_subscribedEvents = flags;
}

Ws::fd EventManager::InternalEvent::getSubscribedFd() const
{
    return (m_subscribedFd);
}

int EventManager::InternalEvent::getSubscribedEvents() const 
{
    return (m_subscribedEvents);
}

// Private copy constructor (not implemented to prevent copying)
EventManager::InternalEvent::InternalEvent(const InternalEvent& copy) : EventCallback(copy)
{
    (void)copy;
}
EventManager::InternalEvent& EventManager::InternalEvent::operator=(const InternalEvent& assign)
{EventCallback::operator=(assign); return (*this);};