

#include "InternalSubs.hpp"
#include <iostream>
#include <cstdlib>

namespace Events
{
    Manager::InternalSubs::InternalSubs() : Subscription() {}
    Manager::InternalSubs::~InternalSubs() {}

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

    void    Manager::InternalSubs::setTriggeredFlags(const Events::Monitor::Type flags)
    {
        m_triggeredEvents = flags;
    }

    void Manager::InternalSubs::setSubscribedFd(const Ws::fd fd)
    {
        m_subscribedFd = fd;
    }

    void Manager::InternalSubs::setSubscribedFlags(const Events::Monitor::Type flags)
    {
        m_subscribedEvents = flags;
    }

    Ws::fd Manager::InternalSubs::getSubscribedFd() const
    {
        return (m_subscribedFd);
    }

    int Manager::InternalSubs::getSubscribedFlags() const 
    {
        return (m_subscribedEvents);
    }

    // Private copy constructor (not implemented to prevent copying)
    Manager::InternalSubs::InternalSubs(const InternalSubs& copy) : Subscription(copy)
    {
        (void)copy;
    }
    Manager::InternalSubs& Manager::InternalSubs::operator=(const InternalSubs& assign)
    {Subscription::operator=(assign); return (*this);};
}