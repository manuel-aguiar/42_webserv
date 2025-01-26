

#include "CgiRequest.hpp"

namespace Cgi
{
	void
	Cgi::Request::setUser(const CgiUser user)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_user = user;
	}

	void
	Cgi::Request::setNotify_Callback(const CgiNotify::Type type, const CgiNotify::Callback handler)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_runtime_Handlers[type] = handler;
	}

	void
	Cgi::Request::setIO_Callback(const CgiIO::Type type, CgiIO::Callback handler)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_IO_Handlers[type] = handler;
	}


	void
	Cgi::Request::setEnvBase(const CgiEnvEnum::Type env, const ImplModule::EnvValue& value)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_env.envBase.emplace_back(env, value);
	}

	void
	Cgi::Request::setEnvExtra(const CgiEnvKey& key, const CgiEnvValue& value)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_env.envExtra.emplace_back(key, value);
	}

	void
	Cgi::Request::setExtension(const CgiInterpExtension& extension)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_extension = extension;
	}

	void
	Cgi::Request::setScriptPath(const std::string& path)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_scriptPath = path;
	}


	void
	Cgi::Request::setTimeoutMs(const unsigned int timeoutMs)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_timeoutMs = timeoutMs;
	}

	void
	Cgi::Request::setRuntimeOptions(const CgiOptions::Mask options)
	{
		assert(m_state == RequestState::ACQUIRED);
		m_options = options;
	}

	const CgiEnvVariables& 
	Cgi::Request::getEnvVars() const
	{
		return (m_env);
	}

	const CgiInterpExtension&
	Cgi::Request::getExtension() const
	{
		return (m_extension);
	}

	const std::string&
	Cgi::Request::getScriptPath() const
	{
		return (m_scriptPath);
	}

	unsigned int
	Cgi::Request::getTimeoutMs() const
	{
		return (m_timeoutMs);
	}

}; // namespace Cgi