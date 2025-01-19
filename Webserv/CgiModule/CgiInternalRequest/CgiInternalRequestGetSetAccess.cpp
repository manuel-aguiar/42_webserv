

# include "CgiInternalRequest.hpp"

//getters
CgiModule::User
CgiModule::InternalRequest::getUser() const
{
    return (m_user);
}

CgiModule::Callback
CgiModule::InternalRequest::getUserCallback(const CallbackType type) const
{
    return (m_userCallbacks[type]);
}



const CgiModule::EnvVariables& 
CgiModule::InternalRequest::getEnvVars() const
{
	return (m_env);
}

const CgiModule::InterpExtension&
CgiModule::InternalRequest::getExtension() const
{
	return (m_extension);
}

const std::string&
CgiModule::InternalRequest::getScriptPath() const
{
	return (m_scriptPath);
}

CgiModule::RuntimeOptions
CgiModule::InternalRequest::getOptions() const
{
	return (m_options);
}

unsigned int
CgiModule::InternalRequest::getTimeoutMs() const
{
	return (m_timeoutMs);
}

TimerTracker<Timer, CgiModule::InternalRequest*>::iterator
CgiModule::InternalRequest::getMyTimer() const
{
	return (m_myTimer);
}

CgiModule::Worker*	CgiModule::InternalRequest::accessExecutor()
{
	return (m_executor);
}

CgiModule::RequestState	CgiModule::InternalRequest::getState() const
{
	return (m_state);
}



//setters
void	CgiModule::InternalRequest::setState(const RequestState state)
{
	m_state = state;
}

void	CgiModule::InternalRequest::assignExecutor(Worker& executor)
{
	m_executor = &executor;
}

void	CgiModule::InternalRequest::setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer)
{
	m_myTimer = timer;
}
