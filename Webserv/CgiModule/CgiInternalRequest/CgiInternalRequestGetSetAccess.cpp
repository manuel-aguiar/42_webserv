

# include "CgiInternalRequest.hpp"

namespace Cgi
{

//getters
Module::User
Module::InternalRequest::getUser() const
{
    return (m_user);
}

Module::Runtime_Callback::Handler
Module::InternalRequest::getRuntime_Handler(const Module::Runtime_Callback::Type type) const
{
    return (m_runtime_Handlers[type]);
}



const Module::EnvVariables& 
Module::InternalRequest::getEnvVars() const
{
	return (m_env);
}

const Module::InterpExtension&
Module::InternalRequest::getExtension() const
{
	return (m_extension);
}

const std::string&
Module::InternalRequest::getScriptPath() const
{
	return (m_scriptPath);
}

Module::Options::Flags
Module::InternalRequest::getOptions() const
{
	return (m_options);
}

unsigned int
Module::InternalRequest::getTimeoutMs() const
{
	return (m_timeoutMs);
}

TimerTracker<Timer, Module::InternalRequest*>::iterator
Module::InternalRequest::getMyTimer() const
{
	return (m_myTimer);
}

Module::Worker*	Module::InternalRequest::accessExecutor()
{
	return (m_executor);
}

Module::RequestState::Type	Module::InternalRequest::getState() const
{
	return (m_state);
}



//setters
void	Module::InternalRequest::setState(const Module::RequestState::Type state)
{
	m_state = state;
}

void	Module::InternalRequest::assignExecutor(Worker& executor)
{
	m_executor = &executor;
}

void	Module::InternalRequest::setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer)
{
	m_myTimer = timer;
}

};