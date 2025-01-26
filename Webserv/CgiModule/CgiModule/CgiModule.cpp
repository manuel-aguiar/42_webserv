

# include "CgiModule.hpp"

ImplModule::ImplModule(size_t workers, size_t backlog, size_t maxTimeout, Events::Manager& eventManager, Globals& globals) :
	m_numWorkers			(workers),
	m_backlog				(backlog),
	m_maxTimeout			(maxTimeout),
	m_busyWorkerCount		(0),
	m_allWorkers			(workers),
	m_allRequestData		(backlog),
	m_availableWorkers		(workers),
	m_availableRequestData	(backlog),
	m_executionQueue		(backlog),
	m_baseEnv				(""),
	m_timerTracker			(backlog),
	m_eventManager			(eventManager),
	m_globals				(globals)
{
	// prepare workers
	for (size_t i = 0; i < m_numWorkers; i++)
	{
		m_allWorkers.emplace_back(*this);
		m_availableWorkers.push_back(&m_allWorkers[i]);
	}
	
	// prepare RequestData
	for (size_t i = 0; i < m_backlog; i++)
	{
		m_allRequestData.emplace_back();
		m_availableRequestData.push_back(&m_allRequestData[i]);
	}
		
	
	m_baseEnv[Cgi::Env::Enum::AUTH_TYPE] 			= Cgi::Env::Str::AUTH_TYPE;
	m_baseEnv[Cgi::Env::Enum::CONTENT_LENGTH] 		= Cgi::Env::Str::CONTENT_LENGTH;
	m_baseEnv[Cgi::Env::Enum::CONTENT_TYPE] 		= Cgi::Env::Str::CONTENT_TYPE;
	m_baseEnv[Cgi::Env::Enum::GATEWAY_INTERFACE] 	= Cgi::Env::Str::GATEWAY_INTERFACE;
	m_baseEnv[Cgi::Env::Enum::PATH_INFO] 			= Cgi::Env::Str::PATH_INFO;
	m_baseEnv[Cgi::Env::Enum::PATH_TRANSLATED] 		= Cgi::Env::Str::PATH_TRANSLATED;
	m_baseEnv[Cgi::Env::Enum::QUERY_STRING] 		= Cgi::Env::Str::QUERY_STRING;
	m_baseEnv[Cgi::Env::Enum::REMOTE_ADDR] 			= Cgi::Env::Str::REMOTE_ADDR;
	m_baseEnv[Cgi::Env::Enum::REMOTE_HOST] 			= Cgi::Env::Str::REMOTE_HOST;
	m_baseEnv[Cgi::Env::Enum::REMOTE_IDENT] 		= Cgi::Env::Str::REMOTE_IDENT;
	m_baseEnv[Cgi::Env::Enum::REMOTE_USER] 			= Cgi::Env::Str::REMOTE_USER;
	m_baseEnv[Cgi::Env::Enum::REQUEST_METHOD] 		= Cgi::Env::Str::REQUEST_METHOD;
	m_baseEnv[Cgi::Env::Enum::SCRIPT_NAME] 			= Cgi::Env::Str::SCRIPT_NAME;
	m_baseEnv[Cgi::Env::Enum::SERVER_NAME] 			= Cgi::Env::Str::SERVER_NAME;
	m_baseEnv[Cgi::Env::Enum::SERVER_PORT] 			= Cgi::Env::Str::SERVER_PORT;
	m_baseEnv[Cgi::Env::Enum::SERVER_PROTOCOL] 		= Cgi::Env::Str::SERVER_PROTOCOL;
	m_baseEnv[Cgi::Env::Enum::SERVER_SOFTWARE] 		= Cgi::Env::Str::SERVER_SOFTWARE;
}

ImplModule::~ImplModule()
{
	stopAndReset();
}

//private as usual, bare minimum implementation to compile
ImplModule::ImplModule(const ImplModule &copy) :
	m_timerTracker			(0),
	m_eventManager			(copy.m_eventManager),
	m_globals				(copy.m_globals) {}

ImplModule& ImplModule::operator=(const ImplModule &assign) {(void)assign; return (*this);}




