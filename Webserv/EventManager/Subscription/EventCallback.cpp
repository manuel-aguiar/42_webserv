

# include "EventCallback.hpp"
# include <cassert>
# include <cstddef>


namespace Event
{
	Subscription::Subscription() :
		m_fd				(-1),
		m_monitoredEvents	(Event::Flags::NONE),
		m_user				(NULL),
		m_handler			(NULL)
	{}

	Subscription::~Subscription() {}


	void 	Subscription::notifyUser()
	{
		if (m_handler)
			m_handler(*this);
	}

	void	Subscription::reset()
	{
		m_fd = -1;
		m_monitoredEvents = Event::Flags::NONE;
		m_user = NULL;
		m_handler = NULL;
	}

	//private
	Subscription::Subscription(const Subscription& copy)  {(void)copy;}
	Subscription& Subscription::operator=(const Subscription& assign) {(void)assign; return (*this);}
}