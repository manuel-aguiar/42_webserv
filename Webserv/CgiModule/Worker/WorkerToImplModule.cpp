
# include "Worker.hpp"
# include "../ImplModule/ImplModule.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

Events::Manager& Worker::mf_accessEventManager()
{
    return (m_CgiModule._WorkerAccessEventManager());
}

Globals& Worker::mf_accessGlobals()
{
    return (m_CgiModule._WorkerAccessGlobals());
}

void    Worker::mf_recycleSuccess()
{
    m_CgiModule._WorkerRecycleSuccess(*this);
}

void    Worker::mf_recycleRuntimeFailure()
{
    m_CgiModule._WorkerRecycleRuntimeFailure(*this);
}

void    Worker::mf_recycleStartupFailure(bool markFdsAsStale)
{
    m_CgiModule._WorkerRecycleStartupFailure(*this, markFdsAsStale);
}

bool	Worker::mf_prepareExecve()
{
	typedef DynArray<std::pair<Cgi::EnvKey, Cgi::EnvValue> >::const_iterator	t_EnvExtraIter;

	const Cgi::EnvVariables& 			envRequest = m_curRequestData->getEnvVars();
	const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
									envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	std::string						temp;
	

	try
	{
		std::map<Cgi::InterpExtension, Cgi::InterpPath>::const_iterator interpExtension 
		= m_CgiModule.getInterpreters().find(m_curRequestData->getExtension());

		if (interpExtension == m_CgiModule.getInterpreters().end())
			throw std::runtime_error("interpreter not found");

		assert (m_envStr.size() == 0 && m_envPtr.size() == 0 && m_argPtr.size() == 0);

		m_envStr.reserve(entryCount);
		m_envPtr.reserve(entryCount + 1);
		m_argPtr.reserve(3);

		for (size_t i = 0; i < envRequest.envBase.size(); i++)
		{
			temp = (envBase[envRequest.envBase[i].first] + "=" + envRequest.envBase[i].second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}
		
		for (t_EnvExtraIter it = envRequest.envExtra.begin(); it != envRequest.envExtra.end(); it++)
		{
			temp = (it->first + "=" + it->second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}

		for (size_t i = 0; i < entryCount; i++)
			m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));

		m_envPtr.push_back(NULL);

		m_argPtr.push_back(const_cast<char*>(interpExtension->second.c_str()));
		m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
		m_argPtr.push_back(NULL);
		
		return (true);
	}
	catch(const std::exception& e)
	{
		mf_accessGlobals().logError("InternalCgiWorker::mf_prepareExecve(): " + std::string(e.what()));
	}
	return (false);
}

