

# include "CgiInternalRequest.hpp"


//getters
CgiUser
InternalRequest::getUser() const
{
	return (m_user);
}

CgiNotify::Callback
InternalRequest::getRuntime_Handler(const CgiNotify::Type type) const
{
	return (m_runtime_Handlers[type]);
}


const CgiEnvVariables& 
InternalRequest::getEnvVars() const
{
	return (m_env);
}

const CgiInterpExtension&
InternalRequest::getExtension() const
{
	return (m_extension);
}

const std::string&
InternalRequest::getScriptPath() const
{
	return (m_scriptPath);
}

CgiOptions::Mask
InternalRequest::getOptions() const
{
	return (m_options);
}

unsigned int
InternalRequest::getTimeoutMs() const
{
	return (m_timeoutMs);
}

TimerTracker<Timer, ImplModule::InternalRequest*>::iterator
InternalRequest::getMyTimer() const
{
	return (m_myTimer);
}

Cgi::ImplModule::mt_CgiWorker*	InternalRequest::accessExecutor()
{
	return (m_executor);
}

Cgi::ImplModule::RequestState::Type	InternalRequest::getState() const
{
	return (m_state);
}



//setters
void	InternalRequest::setState(const Cgi::ImplModule::RequestState::Type state)
{
	m_state = state;
}

void	InternalRequest::assignExecutor(Worker& executor)
{
	m_executor = &executor;
}

void	InternalRequest::setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer)
{
	m_myTimer = timer;
}