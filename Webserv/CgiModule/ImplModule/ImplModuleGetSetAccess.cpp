

#include "ImplModule.hpp"


const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
ImplModule::getBaseEnvKeys() const
{
	return (m_baseEnv);
}

const std::map<Cgi::InterpExtension, Cgi::InterpPath>&
ImplModule::getInterpreters() const
{
	return (m_Interpreters);
}

size_t
ImplModule::getBusyWorkerCount() const
{
	return (m_allWorkers.size() - m_availableWorkers.size());
}

size_t
ImplModule::getQueueSize() const
{
	return (m_executionQueue.size());
}

void
ImplModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void
ImplModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}

Events::Manager&
ImplModule::_WorkerAccessEventManager()
{
	return (m_eventManager);
}

Globals&
ImplModule::_WorkerAccessGlobals()
{
	return (m_globals);
}
