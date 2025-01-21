

#ifndef INTERNALEVENT_HPP

# define INTERNALEVENT_HPP

# include "../EventManager/EventManager.hpp"
# include "../../EventCallback/EventCallback.hpp"

class EventManager::InternalEvent : public EventCallback
{
    public:
        InternalEvent();
        ~InternalEvent();

        void    setTriggeredFlags(Ws::Epoll::Events flags);

        bool    isInvalid() const;
        void    updateSubscription();
        void    unsubscribe();

        void    setSubscribedFd(Ws::fd fd);
        void    setsubscribedEvents(int flags);
        Ws::fd  getSubscribedFd() const;
        int     getsubscribedEvents() const;
        
    private:
        InternalEvent(const InternalEvent& copy);
        InternalEvent& operator=(const InternalEvent& assign);
};

#endif