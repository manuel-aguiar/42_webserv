

# include "InternalReq.hpp"

InternalReq::InternalReq() :
	Request(),
	m_executor(NULL),
	m_myTimer(TimerTracker<Timer, InternalReq*>::iterator()) {}

InternalReq::~InternalReq() {}


void
InternalReq::reset()
{
	Request::mf_reset();
	m_executor = NULL;
	m_myTimer = TimerTracker<Timer, InternalReq*>::iterator();
}

void
InternalReq::Runtime_CallTheUser(const Cgi::Notify::Type event)
{
	Cgi::Notify::Callback	 handler = m_runtime_Handlers[event];
	if (handler)
		(handler)(m_user);
}

Cgi::IO::BytesCount
InternalReq::IO_CallTheUser(const Cgi::IO::Type type, Ws::fd targetFd)
{
	if (!(m_IO_Handlers[type] && m_user))
		return (0);
	return ((m_IO_Handlers[type])(m_user, targetFd));
}

InternalReq::InternalReq(const InternalReq &copy) :
	Request(copy),
	m_executor(copy.m_executor),
	m_myTimer(copy.m_myTimer) {}

InternalReq &InternalReq::operator=(const InternalReq &assign)
{
	if (this == &assign)
		return (*this);

	Request::operator=(assign);
	m_executor = assign.m_executor;
	m_myTimer = assign.m_myTimer;
	return (*this);
}

Cgi::IO::ReceiveStatusHeaders
InternalReq::getDelieverHeadersCallback()
{
	return (m_receiveStatusHeaders);
}