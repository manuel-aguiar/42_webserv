

#ifndef EVENTINTERNALSUBSCRIPTION_HPP

# define EVENTINTERNALSUBSCRIPTION_HPP

# include "../m_EventsDefinitions.h"
# include "../Subscription/Subscription.hpp"

class InternalSub : public Events::Subscription
{
    public:
        InternalSub();
        ~InternalSub();
        InternalSub(const InternalSub& copy);
        InternalSub& operator=(const InternalSub& assign);

        //informing public event of the events that took place on the target fd
        void                    setTriggeredEvents(const Events::Monitor::Mask flags);

        //validation
        bool                    isInvalid() const;

        //internal bookkeeping
        void                    updateSubscription();
        void                    unSubscribe();
        void                    setSubscribedFd(const Ws::fd fd);
        void                    setSubscribedEvents(const Events::Monitor::Mask flags);

        Ws::fd                  getSubscribedFd() const;
        Events::Monitor::Mask   getSubscribedEvents() const;
        
    private:

        Ws::fd					    m_subscribedFd;
        Events::Monitor::Mask		m_subscribedEvents;

};

#endif