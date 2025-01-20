

#include "CgiRequest.hpp"

namespace Cgi
{

void
Module::Request::setUser(Module::User user)
{
	assert(m_state == STATE_ACQUIRED);
	m_user = user;
}

void
Module::Request::setUserCallback(const Module::CallbackType type, const Module::Callback handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_userCallbacks[type] = handler;
}

void
Module::Request::setReadHandler(Module::IO_Handler handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_readHandler = handler;
}

void
Module::Request::setWriteHandler(Module::IO_Handler handler)
{
	assert(m_state == STATE_ACQUIRED);
	m_writeHandler = handler;
}

void
Module::Request::setEnvBase(const Cgi::Env::Enum::Type env, const Module::EnvValue& value)
{
	assert(m_state == STATE_ACQUIRED);
	m_env.envBase.emplace_back(env, value);
}

void
Module::Request::setEnvExtra(const Module::EnvKey& key, const Module::EnvValue& value)
{
	assert(m_state == STATE_ACQUIRED);
	m_env.envExtra.emplace_back(key, value);
}

void
Module::Request::setExtension(const Module::InterpExtension& extension)
{
	assert(m_state == STATE_ACQUIRED);
	m_extension = extension;
}

void
Module::Request::setScriptPath(const std::string& path)
{
	assert(m_state == STATE_ACQUIRED);
	m_scriptPath = path;
}


void
Module::Request::setTimeoutMs(const unsigned int timeoutMs)
{
	assert(m_state == STATE_ACQUIRED);
	m_timeoutMs = timeoutMs;
}

void
Module::Request::setRuntimeOptions(Module::RuntimeOptions options)
{
	assert(m_state == STATE_ACQUIRED);
	m_options = options;
}

const Module::EnvVariables& 
Module::Request::getEnvVars() const
{
	return (m_env);
}

const Module::InterpExtension&
Module::Request::getExtension() const
{
	return (m_extension);
}

const std::string&
Module::Request::getScriptPath() const
{
	return (m_scriptPath);
}

unsigned int
Module::Request::getTimeoutMs() const
{
	return (m_timeoutMs);
}

}; // namespace Cgi