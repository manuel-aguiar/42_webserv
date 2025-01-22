

#include "InternalEvent.hpp"
#include <iostream>
#include <cstdlib>


Manager::InternalEvent::InternalEvent() : Subscription() {}
Manager::InternalEvent::~InternalEvent() {}

void Manager::InternalEvent::updateSubscription()
{
    m_subscribedFd = m_fd;
    m_subscribedEvents = m_monitoredEvents;
}

void    Manager::InternalEvent::unSubscribe()
{
    m_subscribedFd = -1;
    m_subscribedEvents = Ws::Epoll::NONE;
}

bool Manager::InternalEvent::isInvalid() const
{

    return (!(m_subscribedFd == m_fd && m_fd != -1));
}

void    Manager::InternalEvent::setTriggeredEvents(const Ws::Epoll::Events flags)
{
    m_triggeredEvents = flags;
}

void Manager::InternalEvent::setSubscribedFd(const Ws::fd fd)
{
    m_subscribedFd = fd;
}

void Manager::InternalEvent::setSubscribedEvents(const Ws::Epoll::Events flags)
{
    m_subscribedEvents = flags;
}

Ws::fd Manager::InternalEvent::getSubscribedFd() const
{
    return (m_subscribedFd);
}

int Manager::InternalEvent::getSubscribedEvents() const 
{
    return (m_subscribedEvents);
}

// Private copy constructor (not implemented to prevent copying)
Manager::InternalEvent::InternalEvent(const InternalEvent& copy) : Subscription(copy)
{
    (void)copy;
}
Manager::InternalEvent& Manager::InternalEvent::operator=(const InternalEvent& assign)
{Subscription::operator=(assign); return (*this);};