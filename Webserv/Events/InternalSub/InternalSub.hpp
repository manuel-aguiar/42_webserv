

#ifndef EVENTINTERNALSUBSCRIPTION_HPP

# define EVENTINTERNALSUBSCRIPTION_HPP

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

        void                    reset();

        //validation
        bool                    isInvalid() const;

        //internal bookkeeping
        void                    updateSubscription();
        void                    unSubscribe();
        void                    setSubscribedFd(const Ws::fd fd);
        void                    setSubscribedEvents(const Events::Monitor::Mask flags);

        Ws::fd                  getSubscribedFd() const;
        Events::Monitor::Mask   getSubscribedEvents() const;

        Events::Subscription::State    
                                getState() const;
        void                    setState(const Subscription::State state);
        
    private:
        Ws::fd                      m_subscribedFd;	    
        Events::Monitor::Mask		m_subscribedEvents;

};

#endif
