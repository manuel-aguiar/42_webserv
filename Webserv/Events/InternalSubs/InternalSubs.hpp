

#ifndef EVENTINTERNALSUBS_HPP

# define EVENTINTERNALSUBS_HPP

# include "../m_EventsDefinitions.h"
# include "../Manager/Manager.hpp"
# include "../Subscription/Subscription.hpp"


namespace Events
{
    class Manager::InternalSubs : public Manager::Subscription
    {
        public:
            InternalSubs();
            ~InternalSubs();
            InternalSubs(const InternalSubs& copy);
            InternalSubs& operator=(const InternalSubs& assign);

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
}

#endif