

#include "CgiModule.hpp"


namespace Cgi
{

	const StackArray<CgiEnvKey, Cgi::Env::Enum::COUNT>&
	Cgi::ImplModule::getBaseEnvKeys() const
	{
		return (m_baseEnv);
	}

	const std::map<CgiInterpExtension, CgiInterpPath>&
	Cgi::ImplModule::getInterpreters() const
	{
		return (m_Interpreters);
	}

	size_t
	Cgi::ImplModule::getBusyWorkerCount() const
	{
		return (m_allWorkers.size() - m_availableWorkers.size());
	}

	size_t
	Cgi::ImplModule::getQueueSize() const
	{
		return (m_executionQueue.size());
	}

	void
	Cgi::ImplModule::addInterpreter(const std::string& extension, const std::string& path)
	{
		m_Interpreters[extension] = path;
	}

	void
	Cgi::ImplModule::removeInterpreter(const std::string& extension)
	{
		m_Interpreters.erase(extension);
	}

	Events::Manager&
	Cgi::ImplModule::mf_accessEventManager()
	{
		return (m_eventManager);
	}

	Globals&
	Cgi::ImplModule::mf_accessGlobals()
	{
		return (m_globals);
	}

}; // namespace Cgi