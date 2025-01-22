

#include "InternalSub.hpp"
#include <iostream>
#include <cstdlib>

namespace Events
{
    Manager::InternalSub::InternalSub() : 
        Subscription(),
        m_subscribedFd(-1),
        m_subscribedEvents(Events::Monitor::NONE) {}
    Manager::InternalSub::~InternalSub() {}

    Manager::InternalSub::InternalSub(const InternalSub& copy) : 
        Subscription(copy), 
        m_subscribedFd(copy.m_subscribedFd), 
        m_subscribedEvents(copy.m_subscribedEvents) {}
    Manager::InternalSub& Manager::InternalSub::operator=(const InternalSub& assign)
    {
        if (this == &assign)
            return (*this);
        Subscription::operator=(assign);
        m_subscribedFd = assign.m_subscribedFd;
        m_subscribedEvents = assign.m_subscribedEvents;
        return (*this);
    };

    void Manager::InternalSub::updateSubscription()
    {
        m_subscribedFd = m_fd;
        m_subscribedEvents = m_monitoredEvents;
    }

    void    Manager::InternalSub::unSubscribe()
    {
        m_subscribedFd = -1;
        m_subscribedEvents = Events::Monitor::NONE;
    }

    bool Manager::InternalSub::isInvalid() const
    {

        return (!(m_subscribedFd == m_fd && m_fd != -1));
    }

    void    Manager::InternalSub::setTriggeredEvents(const Events::Monitor::Mask flags)
    {
        m_triggeredEvents = flags;
    }

    void Manager::InternalSub::setSubscribedFd(const Ws::fd fd)
    {
        m_subscribedFd = fd;
    }

    void Manager::InternalSub::setSubscribedEvents(const Events::Monitor::Mask flags)
    {
        m_subscribedEvents = flags;
    }

    Ws::fd Manager::InternalSub::getSubscribedFd() const
    {
        return (m_subscribedFd);
    }

    int Manager::InternalSub::getSubscribedEvents() const 
    {
        return (m_subscribedEvents);
    }


}