

#include "CgiRequest.hpp"

namespace Cgi
{

void
Module::Request::setUser(Module::User user)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_user = user;
}

void
Module::Request::setRuntime_Callback(const Module::Runtime_Callback::Type type, const Module::Runtime_Callback::Handler handler)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_runtime_Handlers[type] = handler;
}

void
Module::Request::setIO_Callback(const Module::IO_Callback::Type type, Module::IO_Callback::Handler handler)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_IO_Handlers[type] = handler;
}


void
Module::Request::setEnvBase(const Cgi::Env::Enum::Type env, const Module::EnvValue& value)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_env.envBase.emplace_back(env, value);
}

void
Module::Request::setEnvExtra(const Module::EnvKey& key, const Module::EnvValue& value)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_env.envExtra.emplace_back(key, value);
}

void
Module::Request::setExtension(const Module::InterpExtension& extension)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_extension = extension;
}

void
Module::Request::setScriptPath(const std::string& path)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_scriptPath = path;
}


void
Module::Request::setTimeoutMs(const unsigned int timeoutMs)
{
	assert(m_state == Module::RequestState::ACQUIRED);
	m_timeoutMs = timeoutMs;
}

void
Module::Request::setRuntimeOptions(Module::Options::Flags options)
{
	assert(m_state == Module::RequestState::ACQUIRED);
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