

# include "CgiInternalRequest.hpp"

namespace Cgi
{

	//getters
	CgiUser
	Cgi::Module::InternalRequest::getUser() const
	{
		return (m_user);
	}

	CgiNotify::Callback
	Cgi::Module::InternalRequest::getRuntime_Handler(const CgiNotify::Type type) const
	{
		return (m_runtime_Handlers[type]);
	}


	const CgiEnvVariables& 
	Cgi::Module::InternalRequest::getEnvVars() const
	{
		return (m_env);
	}

	const CgiInterpExtension&
	Cgi::Module::InternalRequest::getExtension() const
	{
		return (m_extension);
	}

	const std::string&
	Cgi::Module::InternalRequest::getScriptPath() const
	{
		return (m_scriptPath);
	}

	CgiOptions::Mask
	Cgi::Module::InternalRequest::getOptions() const
	{
		return (m_options);
	}

	unsigned int
	Cgi::Module::InternalRequest::getTimeoutMs() const
	{
		return (m_timeoutMs);
	}

	TimerTracker<Timer, Module::InternalRequest*>::iterator
	Cgi::Module::InternalRequest::getMyTimer() const
	{
		return (m_myTimer);
	}

	Cgi::Module::mt_CgiWorker*	Cgi::Module::InternalRequest::accessExecutor()
	{
		return (m_executor);
	}

	Cgi::Module::RequestState::Type	Cgi::Module::InternalRequest::getState() const
	{
		return (m_state);
	}



	//setters
	void	Cgi::Module::InternalRequest::setState(const Cgi::Module::RequestState::Type state)
	{
		m_state = state;
	}

	void	Cgi::Module::InternalRequest::assignExecutor(Worker& executor)
	{
		m_executor = &executor;
	}

	void	Cgi::Module::InternalRequest::setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer)
	{
		m_myTimer = timer;
	}

};