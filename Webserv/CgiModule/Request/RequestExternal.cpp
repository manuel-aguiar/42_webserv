

#include "Request.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

namespace Cgi
{
	void
	Cgi::Request::setUser(const Cgi::User user)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_user = user;
	}


	void
	Cgi::Request::setNotify_onError	(const Cgi::Notify::Callback handler)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_notifyOnError = handler;
	}

	void
	Cgi::Request::setNotify_onSuccess	(const Cgi::Notify::Callback handler)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_notifyOnSuccess = handler;
	}

	void
	Cgi::Request::setWriteToScript_Callback	(const Cgi::IO::OnReadWrite handler)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_writeToScript = handler;
	}

	void
	Cgi::Request::setReceiveStatusHeaders_Callback	(const Cgi::IO::ReceiveHeaderData handler)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_receiveStatusHeaders = handler;
	}

	void						
	Cgi::Request::setReadBodyFromScript_Callback(const Cgi::IO::OnReadWrite handler)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_readBodyFromScript = handler;
	}

	void
	Cgi::Request::setEnvBase(const CgiEnvEnum::Type env, const Cgi::EnvValue& value)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_env.envBase.emplace_back(env, value);
	}

	void
	Cgi::Request::setEnvExtra(const Cgi::EnvKey& key, const Cgi::EnvValue& value)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_env.envExtra.emplace_back(key, value);
	}

	void
	Cgi::Request::setInterpreterPath(const Cgi::InterpPath& path)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_interpreterPath = path;
	}

	void
	Cgi::Request::setScriptPath(const Cgi::ScriptPath& path)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_scriptPath = path;
	}


	void
	Cgi::Request::setTimeoutMs(const unsigned int timeoutMs)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_timeoutMs = timeoutMs;
	}

	void
	Cgi::Request::setRuntimeOptions(const Cgi::Options::Mask options)
	{
		ASSERT_EQUAL(m_state, RequestState::ACQUIRED, "Cgi::Request, setters can only be called when Request is in 'Acquired' state");
		m_options = options;
	}

	const Cgi::EnvVariables& 
	Cgi::Request::getEnvVars() const
	{
		return (m_env);
	}

	const Cgi::InterpPath&
	Cgi::Request::getInterpreterPath() const
	{
		return (m_interpreterPath);
	}

	const Cgi::ScriptPath&
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