

#include "InternalSubs.hpp"
#include <iostream>
#include <cstdlib>

namespace Events
{
    Manager::InternalSubs::InternalSubs() : 
        Subscription(),
        m_subscribedFd(-1),
        m_subscribedEvents(Events::Monitor::NONE) {}
    Manager::InternalSubs::~InternalSubs() {}

    Manager::InternalSubs::InternalSubs(const InternalSubs& copy) : 
        Subscription(copy), 
        m_subscribedFd(copy.m_subscribedFd), 
        m_subscribedEvents(copy.m_subscribedEvents) {}
    Manager::InternalSubs& Manager::InternalSubs::operator=(const InternalSubs& assign)
    {
        if (this == &assign)
            return (*this);
        Subscription::operator=(assign);
        m_subscribedFd = assign.m_subscribedFd;
        m_subscribedEvents = assign.m_subscribedEvents;
        return (*this);
    };

    void Manager::InternalSubs::updateSubscription()
    {
        m_subscribedFd = m_fd;
        m_subscribedEvents = m_monitoredEvents;
    }

    void    Manager::InternalSubs::unSubscribe()
    {
        m_subscribedFd = -1;
        m_subscribedEvents = Events::Monitor::NONE;
    }

    bool Manager::InternalSubs::isInvalid() const
    {

        return (!(m_subscribedFd == m_fd && m_fd != -1));
    }

    void    Manager::InternalSubs::setTriggeredEvents(const Events::Monitor::Mask flags)
    {
        m_triggeredEvents = flags;
    }

    void Manager::InternalSubs::setSubscribedFd(const Ws::fd fd)
    {
        m_subscribedFd = fd;
    }

    void Manager::InternalSubs::setSubscribedEvents(const Events::Monitor::Mask flags)
    {
        m_subscribedEvents = flags;
    }

    Ws::fd Manager::InternalSubs::getSubscribedFd() const
    {
        return (m_subscribedFd);
    }

    int Manager::InternalSubs::getSubscribedEvents() const 
    {
        return (m_subscribedEvents);
    }


}