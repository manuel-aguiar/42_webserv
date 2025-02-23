

# include "Subscription.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

namespace Events
{
	Subscription::Subscription() :
		m_fdmask			(Ws::FD_NONE),
		m_monitoredEvents	(Events::Monitor::NONE),
		m_triggeredEvents	(Events::Monitor::NONE),
		m_user				(NULL),
		m_callback			(NULL)
	{}

	Subscription::~Subscription() {}

	void 
	Subscription::notify()
	{
		if (m_callback)
			m_callback(*this);
	}

	void
	Subscription::reset()
	{
		m_fdmask.reset();
		m_monitoredEvents = Events::Monitor::NONE;
		m_user = NULL;
		m_callback = NULL;
	}

	//private
	Subscription::Subscription(const Subscription& copy) :
		m_fdmask			(copy.m_fdmask),
		m_monitoredEvents	(copy.m_monitoredEvents),
		m_triggeredEvents	(copy.m_triggeredEvents),
		m_user				(copy.m_user),
		m_callback			(copy.m_callback)
	{}
	
	Subscription&
	Subscription::operator=(const Subscription& assign)
	{
		if (this == &assign)
			return (*this);

		m_fdmask = assign.m_fdmask;
		m_monitoredEvents = assign.m_monitoredEvents;
		m_triggeredEvents = assign.m_triggeredEvents;
		m_user = assign.m_user;
		m_callback = assign.m_callback;

		return (*this);
	}

	/////////////////////////////////////
	//FdMask

	Subscription::FdMask::FdMask(const Ws::fd fd)
	{
		ASSERT_EQUAL(fd >= Ws::FD_NONE, true, "Subscription::FdMask: fd must be bigger than -1");

		m_fd = fd | (Subscription::UNSUBSCRIBED << 31);
	}

	Subscription::FdMask::~FdMask() {}

	Subscription::FdMask::FdMask(const Subscription::FdMask& copy) : m_fd(copy.m_fd) {}

	Subscription::FdMask& Subscription::FdMask::operator=(const Subscription::FdMask& assign)
	{
		if (this == &assign)
			return (*this);

		m_fd = assign.m_fd;

		return (*this);
	}

	Subscription::State
	Subscription::FdMask::getState() const
	{
		return (State)((m_fd >> 31) & 1);
	}

	void
	Subscription::FdMask::setState(const State state)
	{
		m_fd = (m_fd & ~(1 << 31)) | (state << 31);
	}

	Ws::fd
	Subscription::FdMask::getFd() const
	{
		return ((m_fd == Ws::FD_NONE) ? Ws::FD_NONE : m_fd & ~(1 << 31));
	}

	void
	Subscription::FdMask::setFd(const Ws::fd fd)
	{
		ASSERT_EQUAL(fd >= Ws::FD_NONE, true, "Subscription::FdMask::setFd: fd must be bigger than -1");

		Subscription::State state = getState();

		ASSERT_EQUAL(state == Subscription::UNSUBSCRIBED, true, 
		"Subscription::FdMask::setFd: cannot set fd when the subscription is already being monitored. Unsubcribe first, then change the fd and resubscribe");

		m_fd = fd | (state << 31);
	}

	void
	Subscription::FdMask::reset()
	{
		m_fd = Ws::FD_NONE;
	}

	bool					
	Subscription::FdMask::operator==(const FdMask& rhs) const 
	{
		return (this->getFd() == rhs.getFd());
	}

}
