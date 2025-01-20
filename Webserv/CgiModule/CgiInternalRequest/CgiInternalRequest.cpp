

# include "CgiInternalRequest.hpp"

namespace Cgi
{

Module::InternalRequest::InternalRequest() :
	Request(),
	m_executor(NULL),
	m_myTimer(TimerTracker<Timer, InternalRequest*>::iterator()) {}

Module::InternalRequest::~InternalRequest() {}


void
Module::InternalRequest::reset()
{
	Request::mf_reset();
	m_executor = NULL;
	m_myTimer = TimerTracker<Timer, InternalRequest*>::iterator();
}

void
Module::InternalRequest::CallTheUser(const CallbackType event)
{
	Callback	 handler = m_userCallbacks[event];
	if (handler)
		(handler)(m_user);
}

Module::BytesRead
Module::InternalRequest::UserRead(Ws::fd readFd)
{
	if (!(m_readHandler && m_user))
		return (0);
	return ((m_readHandler)(m_user, readFd));
}

Module::BytesWritten
Module::InternalRequest::UserWrite(Ws::fd writeFd)
{
	if (!(m_writeHandler && m_user))
		return (0);
	return ((m_writeHandler)(m_user, writeFd));
}


Module::InternalRequest::InternalRequest(const InternalRequest &copy) :
	Request(copy),
	m_executor(copy.m_executor),
	m_myTimer(copy.m_myTimer) {}

Module::InternalRequest &Module::InternalRequest::operator=(const InternalRequest &assign)
{
	if (this == &assign)
		return (*this);

	Request::operator=(assign);
	m_executor = assign.m_executor;
	m_myTimer = assign.m_myTimer;
	return (*this);
}

}; // namespace Cgi