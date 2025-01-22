

# include "CgiInternalRequest.hpp"

namespace Cgi
{

	Cgi::Module::InternalRequest::InternalRequest() :
		Request(),
		m_executor(NULL),
		m_myTimer(TimerTracker<Timer, InternalRequest*>::iterator()) {}

	Cgi::Module::InternalRequest::~InternalRequest() {}


	void
	Cgi::Module::InternalRequest::reset()
	{
		Request::mf_reset();
		m_executor = NULL;
		m_myTimer = TimerTracker<Timer, InternalRequest*>::iterator();
	}

	void
	Cgi::Module::InternalRequest::Runtime_CallTheUser(const CgiNotify::Type event)
	{
		Notify::Callback	 handler = m_runtime_Handlers[event];
		if (handler)
			(handler)(m_user);
	}

	Module::IO::BytesCount
	Cgi::Module::InternalRequest::IO_CallTheUser(const CgiIO::Type type, Ws::fd targetFd)
	{
		if (!(m_IO_Handlers[type] && m_user))
			return (0);
		return ((m_IO_Handlers[type])(m_user, targetFd));
	}

	Cgi::Module::InternalRequest::InternalRequest(const InternalRequest &copy) :
		Request(copy),
		m_executor(copy.m_executor),
		m_myTimer(copy.m_myTimer) {}

	Module::InternalRequest &Cgi::Module::InternalRequest::operator=(const InternalRequest &assign)
	{
		if (this == &assign)
			return (*this);

		Request::operator=(assign);
		m_executor = assign.m_executor;
		m_myTimer = assign.m_myTimer;
		return (*this);
	}

}; // namespace Cgi