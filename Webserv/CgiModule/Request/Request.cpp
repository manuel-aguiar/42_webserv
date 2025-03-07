

#include "Request.hpp"

namespace Cgi
{

	Cgi::Request::Request() :
		m_user			(NULL),
		m_notifyOnError	(NULL),
		m_notifyOnSuccess(NULL),
		m_writeToScript	(NULL),
		m_receiveStatusHeaders(NULL),
		m_readBodyFromScript(NULL),
		m_timeoutMs		(0),
		m_scriptPath	(""),
		m_state			(Cgi::RequestState::IDLE),
		m_options		(0)
	{
		m_env.envBase.reserve(Cgi::Env::Enum::COUNT);
	}

	Cgi::Request::~Request()
	{

	}

	void	Cgi::Request::mf_reset()
	{
		m_interpreterPath.clear();
		m_scriptPath.clear();
		m_env.envBase.clear();
		m_env.envExtra.clear();

		m_notifyOnError = NULL;
		m_notifyOnSuccess = NULL;
		m_writeToScript = NULL;
		m_readBodyFromScript = NULL;
		m_receiveStatusHeaders = NULL;

		m_timeoutMs = 0;
		m_user = NULL;
		m_state = Cgi::RequestState::IDLE;
		m_options = 0;
	}

	Cgi::Request::Request(const Request &copy) :
		m_user(copy.m_user),
		m_notifyOnError(copy.m_notifyOnError),
		m_notifyOnSuccess(copy.m_notifyOnSuccess),
		m_writeToScript(copy.m_writeToScript),
		m_receiveStatusHeaders(copy.m_receiveStatusHeaders),
		m_readBodyFromScript(copy.m_readBodyFromScript),
		m_timeoutMs(copy.m_timeoutMs),
		m_scriptPath(copy.m_scriptPath)
	{
		*this = copy;
	}

	Cgi::Request&	Cgi::Request::operator=(const Request &assign)
	{
		if (this == &assign)
			return (*this);

		m_user = assign.m_user;
		m_notifyOnError = assign.m_notifyOnError;
		m_notifyOnSuccess = assign.m_notifyOnSuccess;

		m_writeToScript = assign.m_writeToScript;
		m_readBodyFromScript = assign.m_readBodyFromScript ;
		m_receiveStatusHeaders = assign.m_receiveStatusHeaders;		

		m_timeoutMs = assign.m_timeoutMs;
		m_scriptPath = assign.m_scriptPath;
		m_env.envBase = assign.m_env.envBase;
		m_env.envExtra = assign.m_env.envExtra;
		
		return (*this);
	}

} // namespace Cgi
