

#include "CgiModule.hpp"
#include "../../EventManager/EventManager/EventManager.hpp"

namespace Cgi
{

	const StackArray<CgiEnvKey, Cgi::Env::Enum::COUNT>&
	Cgi::Module::getBaseEnvKeys() const
	{
		return (m_baseEnv);
	}

	const std::map<CgiInterpExtension, CgiInterpPath>&
	Cgi::Module::getInterpreters() const
	{
		return (m_Interpreters);
	}

	size_t
	Cgi::Module::getBusyWorkerCount() const
	{
		return (m_allWorkers.size() - m_availableWorkers.size());
	}

	size_t
	Cgi::Module::getQueueSize() const
	{
		return (m_executionQueue.size());
	}

	void
	Cgi::Module::addInterpreter(const std::string& extension, const std::string& path)
	{
		m_Interpreters[extension] = path;
	}

	void
	Cgi::Module::removeInterpreter(const std::string& extension)
	{
		m_Interpreters.erase(extension);
	}

	Manager&
	Cgi::Module::mf_accessEventManager()
	{
		return (m_eventManager);
	}

	Globals&
	Cgi::Module::mf_accessGlobals()
	{
		return (m_globals);
	}

}; // namespace Cgi