

#include "CgiModule.hpp"
#include "../../ServerManager/EventManager/EventManager/EventManager.hpp"

namespace Cgi
{

const StackArray<Module::EnvKey, Cgi::Env::Enum::COUNT>&
Module::getBaseEnvKeys() const
{
	return (m_baseEnv);
}

const std::map<Module::InterpExtension, Module::InterpPath>&
Module::getInterpreters() const
{
	return (m_Interpreters);
}

size_t
Module::getBusyWorkerCount() const
{
	return (m_allWorkers.size() - m_availableWorkers.size());
}

size_t
Module::getQueueSize() const
{
	return (m_executionQueue.size());
}

void
Module::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void
Module::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}

EventManager&
Module::mf_accessEventManager()
{
	return (m_eventManager);
}

Globals&
Module::mf_accessGlobals()
{
	return (m_globals);
}

}; // namespace Cgi