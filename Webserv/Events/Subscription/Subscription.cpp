

# include "Subscription.hpp"

namespace Events
{
	Subscription::Subscription() :
		m_fd				(-1),
		m_monitoredEvents	(Events::Monitor::NONE),
		m_user				(NULL),
		m_callback			(NULL)
	{}

	Subscription::~Subscription() {}

	void 	Subscription::notify()
	{
		if (m_callback)
			m_callback(*this);
	}

	void	Subscription::reset()
	{
		m_fd = -1;
		m_monitoredEvents = Events::Monitor::NONE;
		m_user = NULL;
		m_callback = NULL;
	}

	//private
	Subscription::Subscription(const Subscription& copy)  {(void)copy;}
	Subscription& Subscription::operator=(const Subscription& assign) {(void)assign; return (*this);}
}