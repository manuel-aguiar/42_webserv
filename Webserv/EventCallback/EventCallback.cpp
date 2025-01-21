

# include "EventCallback.hpp"
# include <cassert>
# include <cstddef>

EventCallback::EventCallback() :
	m_fd				(-1),
	m_monitoredEvents	(Ws::Epoll::NONE),
	m_triggeredEvents	(Ws::Epoll::NONE),
	m_user				(NULL),
	m_handler			(NULL),
	m_subscribedFd		(-1),
	m_subscribedEvents	(Ws::Epoll::NONE)
{}

EventCallback::~EventCallback() {}


void EventCallback::handle()
{
	if (m_handler)
		m_handler(*this);
}

void	EventCallback::reset()
{
	m_fd = -1;
	m_monitoredEvents = Ws::Epoll::NONE;
	m_triggeredEvents = Ws::Epoll::NONE;
	m_user = NULL;
	m_handler = NULL;
}

//private
EventCallback::EventCallback(const EventCallback& copy)  {(void)copy;}
EventCallback& EventCallback::operator=(const EventCallback& assign) {(void)assign; return (*this);}
