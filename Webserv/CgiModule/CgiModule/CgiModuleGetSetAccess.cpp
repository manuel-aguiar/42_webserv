

#include "CgiModule.hpp"
#include "../../ServerManager/EventManager/EventManager/EventManager.hpp"

const StackArray<CgiModule::EnvKey, Cgi::ENV_COUNT>&
CgiModule::getBaseEnvKeys() const
{
	return (m_baseEnv);
}

const std::map<CgiModule::InterpExtension, CgiModule::InterpPath>&
CgiModule::getInterpreters() const
{
	return (m_Interpreters);
}

size_t
CgiModule::getBusyWorkerCount() const
{
	return (m_allWorkers.size() - m_availableWorkers.size());
}

size_t
CgiModule::getQueueSize() const
{
	return (m_executionQueue.size());
}

void
CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void
CgiModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}

EventManager&
CgiModule::mf_accessEventManager()
{
	return (m_eventManager);
}

Globals&
CgiModule::mf_accessGlobals()
{
	return (m_globals);
}