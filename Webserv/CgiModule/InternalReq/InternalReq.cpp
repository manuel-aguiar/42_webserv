

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

Cgi::Notify::Callback	
InternalReq::getNotifyOnError_Callback() const
{
	return (m_notifyOnError);
}

Cgi::Notify::Callback	
InternalReq::getNotifyOnSuccess_Callback() const
{
	return (m_notifyOnSuccess);
}

Cgi::IO::Callback		
InternalReq::getWriteToScript_Callback() const
{
	return(m_writeToScript);
}

Cgi::IO::Callback		
InternalReq::getReceiveScriptBody_Callback() const
{
	return (m_receiveScriptBody);
}

Cgi::IO::ReceiveStatusHeaders
InternalReq::getReceiveHeaders_Callback() const
{
	return (m_receiveStatusHeaders);
}

Cgi::Notify::Callback
InternalReq::getRuntime_Handler(const Cgi::Notify::Type type) const
{
	switch (type)
	{
		case Cgi::Notify::ON_ERROR: return (m_notifyOnError);
		case Cgi::Notify::ON_SUCCESS: return (m_notifyOnSuccess);
		default: return (NULL);
	}
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
