

# include "InternalReq.hpp"


//getters
Cgi::User
InternalRequest::getUser() const
{
	return (m_user);
}

Cgi::Notify::Callback
InternalRequest::getRuntime_Handler(const Cgi::Notify::Type type) const
{
	return (m_runtime_Handlers[type]);
}


const Cgi::EnvVariables& 
InternalRequest::getEnvVars() const
{
	return (m_env);
}

const Cgi::InterpExtension&
InternalRequest::getExtension() const
{
	return (m_extension);
}

const std::string&
InternalRequest::getScriptPath() const
{
	return (m_scriptPath);
}

Cgi::Options::Mask
InternalRequest::getOptions() const
{
	return (m_options);
}

unsigned int
InternalRequest::getTimeoutMs() const
{
	return (m_timeoutMs);
}

TimerTracker<Timer, InternalRequest*>::iterator
InternalRequest::getMyTimer() const
{
	return (m_myTimer);
}

Worker*	InternalRequest::accessExecutor()
{
	return (m_executor);
}

Cgi::RequestState::Type	InternalRequest::getState() const
{
	return (m_state);
}



//setters
void	InternalRequest::setState(const Cgi::RequestState::Type state)
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