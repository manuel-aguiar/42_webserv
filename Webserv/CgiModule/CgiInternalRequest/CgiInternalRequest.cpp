

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
Module::InternalRequest::Runtime_CallTheUser(const Module::Runtime_Callback::Type event)
{
	Runtime_Callback::Handler	 handler = m_runtime_Handlers[event];
	if (handler)
		(handler)(m_user);
}

Module::IO_Callback::BytesCount
Module::InternalRequest::IO_CallTheUser(const Module::IO_Callback::Type type, Ws::fd targetFd)
{
	if (!(m_IO_Handlers[type] && m_user))
		return (0);
	return ((m_IO_Handlers[type])(m_user, targetFd));
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