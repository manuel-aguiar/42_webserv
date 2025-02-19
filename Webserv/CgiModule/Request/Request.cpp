

#include "Request.hpp"

namespace Cgi
{

	Cgi::Request::Request() :
		m_user			(NULL),
		m_timeoutMs		(0),
		m_extension		(""),
		m_scriptPath	(""),
		m_state			(Cgi::RequestState::IDLE),
		m_options		(0)
	{
		m_env.envBase.reserve(Cgi::Env::Enum::COUNT);

		for (size_t i = 0; i < Cgi::Notify::COUNT; i++)
			m_runtime_Handlers[i] = NULL;

		for (size_t i = 0; i < Cgi::IO::COUNT; i++)
			m_IO_Handlers[i] = NULL;

	}

	Cgi::Request::~Request()
	{

	}



	void	Cgi::Request::mf_reset()
	{
		m_interpreterPath.clear();
		m_extension.clear();
		m_scriptPath.clear();
		m_env.envBase.clear();
		m_env.envExtra.clear();

		for (size_t i = 0; i < Cgi::Notify::COUNT; i++)
			m_runtime_Handlers[i] = NULL;
		for (size_t i = 0; i < Cgi::IO::COUNT; i++)
			m_IO_Handlers[i] = NULL;
		
		m_timeoutMs = 0;
		m_user = NULL;
		m_state = Cgi::RequestState::IDLE;
		m_options = 0;
	}

	Cgi::Request::Request(const Request &copy) :
		m_user(copy.m_user),
		m_timeoutMs(copy.m_timeoutMs),
		m_extension(copy.m_extension),
		m_scriptPath(copy.m_scriptPath)
	{
		*this = copy;
	}

	Cgi::Request&	Cgi::Request::operator=(const Request &assign)
	{
		if (this == &assign)
			return (*this);

		m_user = assign.m_user;

		for (size_t i = 0; i < Cgi::Notify::COUNT; i++)
			m_runtime_Handlers[i] = assign.m_runtime_Handlers[i];
		for (size_t i = 0; i < Cgi::IO::COUNT; i++)
			m_IO_Handlers[i] = assign.m_IO_Handlers[i];

		m_timeoutMs = assign.m_timeoutMs;
		m_extension = assign.m_extension;
		m_scriptPath = assign.m_scriptPath;
		m_env.envBase = assign.m_env.envBase;
		m_env.envExtra = assign.m_env.envExtra;
		
		return (*this);
	}

}; // namespace Cgi