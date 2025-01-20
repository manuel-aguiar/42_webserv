

#ifndef INTERNALEVENT_HPP

# define INTERNALEVENT_HPP

# include "../EventManager/EventManager.hpp"
# include "../../../Event/Event.hpp"

class EventManager::InternalEvent : public Event
{
    public:
        InternalEvent();
        ~InternalEvent();

        bool    isInvalid() const;
        
        void    updateSubscription();
        void    unsubscribe();

        void    setSubscribedFd(Ws::fd fd);
        void    setSubscribedFlags(int flags);
        Ws::fd  getSubscribedFd() const;
        int     getSubscribedFlags() const;
        
    private:
        InternalEvent(const InternalEvent& copy);
        InternalEvent& operator=(const InternalEvent& assign);
};

#endif