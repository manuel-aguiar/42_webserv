

# include "InternalReq.hpp"


//getters
Cgi::User
InternalReq::getUser() const
{
	return (m_user);
}

const Cgi::EnvVariables& 
InternalReq::getEnvVars() const
{
	return (m_env);
}

const std::string&
InternalReq::getScriptPath() const
{
	return (m_scriptPath);
}

Cgi::Options::Mask
InternalReq::getOptions() const
{
	return (m_options);
}

unsigned int
InternalReq::getTimeoutMs() const
{
	return (m_timeoutMs);
}

TimerTracker<Timer, InternalReq*>::iterator
InternalReq::getMyTimer() const
{
	return (m_myTimer);
}

Worker*	InternalReq::accessExecutor()
{
	return (m_executor);
}

Cgi::RequestState::Type	InternalReq::getState() const
{
	return (m_state);
}



//setters
void	InternalReq::setState(const Cgi::RequestState::Type state)
{
	m_state = state;
}

void	InternalReq::assignExecutor(Worker& executor)
{
	m_executor = &executor;
}

void	InternalReq::setMyTimer(const TimerTracker<Timer, InternalReq*>::iterator& timer)
{
	m_myTimer = timer;
}