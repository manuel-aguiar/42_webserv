

# include "CgiInternalRequest.hpp"

CgiModule::InternalRequest::InternalRequest() :
	Request(),
	m_executor(NULL),
	m_myTimer(TimerTracker<Timer, InternalRequest*>::iterator()) {}

CgiModule::InternalRequest::~InternalRequest() {}


void
CgiModule::InternalRequest::reset()
{
	Request::mf_reset();
	m_executor = NULL;
	m_myTimer = TimerTracker<Timer, InternalRequest*>::iterator();
}

void
CgiModule::InternalRequest::CallTheUser(const CallbackType event)
{
	Callback	 handler = m_userCallbacks[event];
	if (handler)
		(handler)(m_user);
}

CgiModule::BytesRead
CgiModule::InternalRequest::UserRead(t_fd readFd)
{
	if (!(m_readHandler && m_user))
		return (0);
	return ((m_readHandler)(m_user, readFd));
}

CgiModule::BytesWritten
CgiModule::InternalRequest::UserWrite(t_fd writeFd)
{
	if (!(m_writeHandler && m_user))
		return (0);
	return ((m_writeHandler)(m_user, writeFd));
}


CgiModule::InternalRequest::InternalRequest(const InternalRequest &copy) :
	Request(copy),
	m_executor(copy.m_executor),
	m_myTimer(copy.m_myTimer) {}

CgiModule::InternalRequest &CgiModule::InternalRequest::operator=(const InternalRequest &assign)
{
	if (this == &assign)
		return (*this);

	Request::operator=(assign);
	m_executor = assign.m_executor;
	m_myTimer = assign.m_myTimer;
	return (*this);
}