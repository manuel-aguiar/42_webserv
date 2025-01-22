

#ifndef EVENTINTERNALSUBS_HPP

# define EVENTINTERNALSUBS_HPP

# include "../Events.h"
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
            void                    setTriggeredFlags(const Events::Monitor::Type flags);

            //validation
            bool                    isInvalid() const;

            //internal bookkeeping
            void                    updateSubscription();
            void                    unSubscribe();
            void                    setSubscribedFd(const Ws::fd fd);
            void                    setSubscribedFlags(const Events::Monitor::Type flags);

            Ws::fd                  getSubscribedFd() const;
            Events::Monitor::Type      getSubscribedFlags() const;
            
        private:

            Ws::fd					    m_subscribedFd;
            Events::Monitor::Type		m_subscribedEvents;

    };
}

#endif