

# include "CgiInternalRequest.hpp"

InternalRequest::InternalRequest() :
	Request(),
	m_executor(NULL),
	m_myTimer(TimerTracker<Timer, InternalRequest*>::iterator()) {}

InternalRequest::~InternalRequest() {}


void
InternalRequest::reset()
{
	Request::mf_reset();
	m_executor = NULL;
	m_myTimer = TimerTracker<Timer, InternalRequest*>::iterator();
}

void
InternalRequest::Runtime_CallTheUser(const Cgi::Notify::Type event)
{
	Cgi::Notify::Callback	 handler = m_runtime_Handlers[event];
	if (handler)
		(handler)(m_user);
}

Cgi::IO::BytesCount
InternalRequest::IO_CallTheUser(const Cgi::IO::Type type, Ws::fd targetFd)
{
	if (!(m_IO_Handlers[type] && m_user))
		return (0);
	return ((m_IO_Handlers[type])(m_user, targetFd));
}

InternalRequest::InternalRequest(const InternalRequest &copy) :
	Request(copy),
	m_executor(copy.m_executor),
	m_myTimer(copy.m_myTimer) {}

InternalRequest &InternalRequest::operator=(const InternalRequest &assign)
{
	if (this == &assign)
		return (*this);

	Request::operator=(assign);
	m_executor = assign.m_executor;
	m_myTimer = assign.m_myTimer;
	return (*this);
}