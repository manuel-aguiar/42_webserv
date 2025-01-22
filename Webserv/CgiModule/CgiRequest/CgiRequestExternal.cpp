

#include "CgiRequest.hpp"

namespace Cgi
{

	void
	Cgi::Module::Request::setUser(const CgiUser user)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_user = user;
	}

	void
	Cgi::Module::Request::setRuntime_Callback(const CgiRuntime_Callback::Type type, const CgiRuntime_Callback::Service handler)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_runtime_Handlers[type] = handler;
	}

	void
	Cgi::Module::Request::setIO_Callback(const CgiIO_Callback::Type type, CgiIO_Callback::Service handler)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_IO_Handlers[type] = handler;
	}


	void
	Cgi::Module::Request::setEnvBase(const CgiEnvEnum::Type env, const Module::EnvValue& value)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_env.envBase.emplace_back(env, value);
	}

	void
	Cgi::Module::Request::setEnvExtra(const CgiEnvKey& key, const CgiEnvValue& value)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_env.envExtra.emplace_back(key, value);
	}

	void
	Cgi::Module::Request::setExtension(const CgiInterpExtension& extension)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_extension = extension;
	}

	void
	Cgi::Module::Request::setScriptPath(const std::string& path)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_scriptPath = path;
	}


	void
	Cgi::Module::Request::setTimeoutMs(const unsigned int timeoutMs)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_timeoutMs = timeoutMs;
	}

	void
	Cgi::Module::Request::setRuntimeOptions(const CgiOptions::Monitor options)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_options = options;
	}

	const CgiEnvVariables& 
	Cgi::Module::Request::getEnvVars() const
	{
		return (m_env);
	}

	const CgiInterpExtension&
	Cgi::Module::Request::getExtension() const
	{
		return (m_extension);
	}

	const std::string&
	Cgi::Module::Request::getScriptPath() const
	{
		return (m_scriptPath);
	}

	unsigned int
	Cgi::Module::Request::getTimeoutMs() const
	{
		return (m_timeoutMs);
	}

}; // namespace Cgi