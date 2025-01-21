

#ifndef INTERNALEVENT_HPP

# define INTERNALEVENT_HPP

# include "../EventManager/EventManager.hpp"
# include "../../EventCallback/EventCallback.hpp"

class EventManager::InternalEvent : public EventCallback
{
    public:
        InternalEvent();
        ~InternalEvent();

        //informing public event of the events that took place on the target fd
        void                    setTriggeredEvents(const Ws::Epoll::Events flags);

        //validation
        bool                    isInvalid() const;


        //internal bookkeeping
        void                    updateSubscription();
        void                    unSubscribe();
        void                    setSubscribedFd(const Ws::fd fd);
        void                    setSubscribedEvents(const Ws::Epoll::Events flags);

        Ws::fd                  getSubscribedFd() const;
        Ws::Epoll::Events       getSubscribedEvents() const;
        
    private:
        InternalEvent(const InternalEvent& copy);
        InternalEvent& operator=(const InternalEvent& assign);
};

#endif