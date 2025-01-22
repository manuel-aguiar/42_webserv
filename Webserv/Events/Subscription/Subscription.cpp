

# include "Subscription.hpp"
# include <cassert>
# include <cstddef>


namespace Events
{
	Manager::Subscription::Subscription() :
		m_fd				(-1),
		m_monitoredEvents	(Events::Monitor::NONE),
		m_user				(NULL),
		m_callback			(NULL)
	{}

	Manager::Subscription::~Subscription() {}

	void 	Manager::Subscription::notify()
	{
		if (m_callback)
			m_callback(*this);
	}

	void	Manager::Subscription::reset()
	{
		m_fd = -1;
		m_monitoredEvents = Events::Monitor::NONE;
		m_user = NULL;
		m_callback = NULL;
	}

	//private
	Manager::Subscription::Subscription(const Subscription& copy)  {(void)copy;}
	Manager::Subscription& Manager::Subscription::operator=(const Subscription& assign) {(void)assign; return (*this);}
}