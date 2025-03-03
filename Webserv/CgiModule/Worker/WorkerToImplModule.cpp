
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

	const Cgi::EnvVariables& 		envRequest = m_curRequestData->getEnvVars();
	const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
									envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	std::string						temp;
	

	try
	{
		ASSERT_EQUAL(m_envStr.size() == 0 && m_envPtr.size() == 0 && m_argPtr.size() == 0, true, 
		"InternalCgiWorker::mf_prepareExecve(), already prepared");

		m_envStr.reserve(entryCount);
		m_envPtr.reserve(entryCount + 1);
		m_argPtr.reserve(3);

		// interpreter lookup: if given, use it. If not, use extension to find interpreter. If not, there is no interpreter
		ASSERT_EQUAL(m_curRequestData->getInterpreterPath().empty(), false, "no interpreter was provided");
		ASSERT_EQUAL(m_curRequestData->getScriptPath().empty(), false, "no script path was provided");
		
		m_argPtr.push_back(const_cast<char*>(m_curRequestData->getInterpreterPath().c_str()));
		m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
		m_argPtr.push_back(NULL);

		for (size_t i = 0; i < envRequest.envBase.size(); i++)
		{
			temp.reserve(envBase[envRequest.envBase[i].first].size() + 1 + envRequest.envBase[i].second.size() + 1);
			temp = (envBase[envRequest.envBase[i].first] + "=" + envRequest.envBase[i].second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}
		
		for (t_EnvExtraIter it = envRequest.envExtra.begin(); it != envRequest.envExtra.end(); it++)
		{
			temp.reserve(it->first.size() + 1 + it->second.size() + 1);
			temp = (it->first + "=" + it->second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}

		for (size_t i = 0; i < entryCount; i++)
			m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));

		m_envPtr.push_back(NULL);

		std::cout << "arg0: " << m_argPtr[0] << std::endl;
		std::cout << "arg1: " << m_argPtr[1] << std::endl;
		
		return (true);
	}
	catch(const std::exception& e)
	{
		mf_accessGlobals().logError("InternalCgiWorker::mf_prepareExecve(): " + std::string(e.what()));
	}
	return (false);
}
