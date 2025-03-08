

// Own Headers

# include "ServerConfig.hpp"
# include "../ServerLocation/ServerLocation.hpp"
# include "../ServerBlock/ServerBlock.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../GenericUtils/Validation/Validation.hpp"

// C++ headers
# include <cstdlib> // atoi

ServerConfig::DirectiveToSetter ServerConfig::m_directiveToSetter;

ServerConfig::DirectiveToSetter::DirectiveToSetter() :
	map(std::less<std::string>(), mapPool(11))	// 10 magic: number of keys
{
	map["max_connections"]			= &ServerConfig::setMaxConnections;
	map["max_concurrent_cgi"]		= &ServerConfig::setMaxConcurrentCgi;
	map["max_cgi_backlog"]			= &ServerConfig::setMaxCgiBacklog;
	map["max_workers"]				= &ServerConfig::setMaxWorkers;
	map["cgi"]						= &ServerConfig::addCgiInterpreter;
	map["client_body_size"]			= &ServerConfig::setClientBodySize;
	map["client_header_size"]		= &ServerConfig::setClientHeaderSize;
	map["timeout_full_header"]		= &ServerConfig::setTimeoutFullHeader;
	map["timeout_inter_send"]		= &ServerConfig::setTimeoutInterSend;
	map["timeout_inter_receive"]	= &ServerConfig::setTimeoutInterReceive;
	map["timeout_keep_alive"]		= &ServerConfig::setTimeoutKeepAlive;
}

ServerConfig::ServerConfig(const char* configFilePath, const DefaultConfig& defaultConfig) :
	m_max_connections			(DefaultConfig::UINT_NONE),
	m_max_concurrent_cgi		(DefaultConfig::UINT_NONE),
	m_max_cgi_backlog			(DefaultConfig::UINT_NONE),
	m_max_workers				(DefaultConfig::UINT_NONE),	
	m_http_maxClientBodySize	(DefaultConfig::UINT_NONE),	
	m_http_maxClientHeaderSize	(DefaultConfig::UINT_NONE),	
	m_http_timeoutFullHeader	(DefaultConfig::UINT_NONE),	
	m_http_timeoutInterSend		(DefaultConfig::UINT_NONE),	
	m_http_timeoutInterReceive	(DefaultConfig::UINT_NONE),	
	m_http_timeoutKeepAlive		(DefaultConfig::UINT_NONE),
	m_configDefault				(defaultConfig),
	m_configFilePath			(configFilePath)
	{}

ServerConfig::~ServerConfig()
{
	// Nothing to do here
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this == &other)
		return (*this);

	m_max_concurrent_cgi = other.m_max_concurrent_cgi;
	m_max_connections = other.m_max_connections;
	m_max_cgi_backlog = other.m_max_cgi_backlog;
	m_max_workers = other.m_max_workers;
	m_http_maxClientBodySize = other.m_http_maxClientBodySize;
	m_http_maxClientHeaderSize = other.m_http_maxClientHeaderSize;
	m_http_timeoutFullHeader = other.m_http_timeoutFullHeader;
	m_http_timeoutInterSend = other.m_http_timeoutInterSend;
	m_http_timeoutInterReceive = other.m_http_timeoutInterReceive;
	m_http_timeoutKeepAlive = other.m_http_timeoutKeepAlive;
	m_configFilePath = other.m_configFilePath;
	m_serverBlocks = other.m_serverBlocks;

	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig &other) :
	m_max_connections			(other.m_max_connections),
	m_max_concurrent_cgi		(other.m_max_concurrent_cgi),
	m_max_cgi_backlog			(other.m_max_cgi_backlog),
	m_max_workers				(other.m_max_workers),
	m_http_maxClientBodySize	(other.m_http_maxClientBodySize),
	m_http_maxClientHeaderSize	(other.m_http_maxClientHeaderSize),
	m_http_timeoutFullHeader	(other.m_http_timeoutFullHeader),
	m_http_timeoutInterSend		(other.m_http_timeoutInterSend),
	m_http_timeoutInterReceive	(other.m_http_timeoutInterReceive),
	m_http_timeoutKeepAlive		(other.m_http_timeoutKeepAlive),
	m_configDefault				(other.m_configDefault),
	m_configFilePath			(other.m_configFilePath),
	m_serverBlocks				(other.m_serverBlocks)
{
	*this = other;
}

bool	ServerConfig::m_updateFile()
{
	m_configFileStream.open(m_configFilePath);
	if (!m_configFileStream.is_open())
	{
		std::cerr << "Error: Could not open configuration file." << std::endl;
		return (false);
	}
	return (true);
}

void	ServerConfig::m_setConfigValue(const std::string &key, const std::string &value)
{
	if (m_directiveToSetter.map.find(key) == m_directiveToSetter.map.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_directiveToSetter.map[key])(value);
}

int		ServerConfig::m_parseConfigLine(const std::string &line, 
										const size_t &currentLine, 
										std::vector<ServerBlock> &servers, 
										const int &currentLevel)
{
	std::istringstream iss(line);
	std::string key, value;
	iss >> key;

	if (key[0] == '#')
		return (2);
	while (iss >> value)
	{
		if (*value.rbegin() == ';')
			value = value.substr(0, value.size() - 1);
		switch (currentLevel)
		{
			case PROGRAM_LEVEL:
				try {
					m_setConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << "Error: " << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
			case SERVER_LEVEL:
				try {
					servers.back().addConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << "Error: " << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
			case LOCATION_LEVEL:
				try {
					servers.back().accessLocations().back().addConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << "Error: " << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
		}
	}
	if (value.empty())
	{
		std::cerr << "Error: key \"" << key <<  "\" with no value on line " << currentLine << std::endl;
		return (0);
	}
	return (1);
}

bool		ServerConfig::m_handleClosingBracket(int &currentLevel, size_t currentLine,
										std::vector<ServerBlock> &servers)
{
	switch (currentLevel)
	{
		case PROGRAM_LEVEL:
		{
			std::cerr << "Error: config parsing: stray closing bracket on line "
				<< currentLine << std::endl;
			return (0);
		}
		case SERVER_LEVEL:
		{
			currentLevel = PROGRAM_LEVEL;
			servers.back().setDefaults(m_configDefault);
			if (!servers.back().validate())
			{
				std::cerr << "Error: config parsing: invalid server block closing on line "
					<< currentLine << std::endl;
				return (0);
			}
			servers.back().mapLocations();
			break ;
		}
		case LOCATION_LEVEL:
		{
			currentLevel = SERVER_LEVEL;
			ServerLocation& location = servers.back().accessLocations().back();
			location.setDefaults(m_configDefault);
			if (!location.validate())
			{
				std::cerr << "Error: config parsing: invalid location block closing on line "
					<< currentLine << std::endl;
				return (0);
			}
			break ;
		}
		default:
			std::cerr << "Parsing: Unexpected Error" << std::endl;
			return (0);
	}
	return (1);
}

bool	ServerConfig::parseConfigFile()
{

	std::string		line;
	size_t			currentLine = 0;
	int				currentLevel = PROGRAM_LEVEL;

	if (!m_updateFile())
		return (false);
	if (m_serverBlocks.size() > 0)
	{
		std::cerr << "Error: parsing after startup not implemented" << std::endl;
		return (false);
	}
	while (std::getline(m_configFileStream, line))
	{
		currentLine++;
		line = StringUtils::strtrim(line);
		if (line.empty() || line[0] == '#')
			continue ;
		if (line == "server {")
		{
			if (currentLevel != PROGRAM_LEVEL)
			{
				std::cerr << "Error: config parsing: server not on program level on line "
					<< currentLine << std::endl;
				return (false);
			}
			m_serverBlocks.push_back(ServerBlock());
			currentLevel = SERVER_LEVEL;
		}
		else if (line == "location {")
		{
			if (currentLevel != SERVER_LEVEL)
			{
				std::cerr << "Error: config parsing: location not on server level on line "
					<< currentLine << std::endl;
				return (false);
			}
			m_serverBlocks.back().accessLocations().push_back(ServerLocation());
			currentLevel = LOCATION_LEVEL;
		}
		else if (line == "}")
		{
			if (!m_handleClosingBracket(currentLevel, currentLine, m_serverBlocks))
				return (false);
		}
		else
		{
			if (!m_parseConfigLine(line, currentLine, m_serverBlocks, currentLevel))
			{
				std::cerr << "Error: config parsing: invalid input on line "
					<< currentLine << std::endl;
				return (false);
			}
		}
	}
	if (m_serverBlocks.size() == 0)
	{
		std::cerr << "Error: no server configurations on config file" << std::endl;
		return (false);
	}
	if (currentLevel != PROGRAM_LEVEL)
	{
		std::cerr << "Error: missing closing bracket" << std::endl;
		return (false);
	}

	m_setDefaults();

	if (!mf_listenDNSlookup()
	|| 	!mf_applyInheritedSettings())
		return (false);
		

	return (true);
}

// Getters & Setters
const char*		ServerConfig::getConfigPath() const
{
	return (m_configFilePath);
}

const std::vector<ServerBlock>&	ServerConfig::getServerBlocks() const
{
	return (m_serverBlocks);
}

int		ServerConfig::getMaxConnections() const
{
	return (m_max_connections);
}

int		ServerConfig::getMaxConcurrentCgi() const
{
	return (m_max_concurrent_cgi);
}

int		ServerConfig::getMaxCgiBacklog() const
{
	return (m_max_cgi_backlog);
}

int		ServerConfig::getMaxWorkers() const
{
	return (m_max_workers);
}

size_t	ServerConfig::getClientBodySize() const
{
	return (m_http_maxClientBodySize);
}

size_t	ServerConfig::getClientHeaderSize() const
{
	return (m_http_maxClientHeaderSize);
}

int		ServerConfig::getTimeoutFullHeader() const
{
	return (m_http_timeoutFullHeader);
}

int		ServerConfig::getTimeoutInterSend() const
{
	return (m_http_timeoutInterSend);
}

int		ServerConfig::getTimeoutInterReceive() const
{
	return (m_http_timeoutInterReceive);
}

int		ServerConfig::getTimeoutKeepAlive() const
{
	return (m_http_timeoutKeepAlive);
}

const DefaultConfig&	ServerConfig::getDefaultConfig() const
{
	return (m_configDefault);
}

const Config::CgiInterpreterMap&
ServerConfig::getCgiInterpreters() const
{
	return (m_cgiInterpreters);
}

static	void _throw_ifInvalidNumber(const std::string& value, const size_t max)
{
	size_t	number;
	
	try {
		number = StringUtils::stoull(value);
		if (number > max)
			throw (std::invalid_argument("value too high"));
		if (value[0] == '-')
			throw (std::invalid_argument("must be a positive number,"));
	}
	catch (std::exception &e){
		std::string msg = e.what();
		throw (std::invalid_argument(msg));
	}
}

void		ServerConfig::setMaxConnections(const std::string &value)
{
	_throw_ifInvalidNumber(value, 1048576);
	m_max_connections = std::atoi(value.c_str());
}

void		ServerConfig::setMaxConcurrentCgi(const std::string &value)
{
	const size_t 	max = StringUtils::parse_size("10M");
	_throw_ifInvalidNumber(value, max);
	m_max_concurrent_cgi = std::atoi(value.c_str());
}

void	ServerConfig::setMaxCgiBacklog(const std::string &value)
{
	const size_t 	max = StringUtils::parse_size("10M");
	_throw_ifInvalidNumber(value, max);
	m_max_cgi_backlog = std::atoi(value.c_str());
}

void		ServerConfig::setMaxWorkers(const std::string &value)
{
	const size_t 	max = StringUtils::parse_size("10M");
	_throw_ifInvalidNumber(value, max);
	m_max_workers = std::atoi(value.c_str());
}

void	ServerConfig::setClientBodySize(const std::string &value)
{
	try {
		m_http_maxClientBodySize = StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw (std::invalid_argument(e.what()));
	}
}

void	ServerConfig::setClientHeaderSize(const std::string &value)
{
	try {
		m_http_maxClientHeaderSize = StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw (std::invalid_argument(e.what()));
	}
}

void		ServerConfig::setTimeoutFullHeader(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutFullHeader = std::atoi(value.c_str());
}

void		ServerConfig::setTimeoutInterSend(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutInterSend = std::atoi(value.c_str());
}

void		ServerConfig::setTimeoutInterReceive(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutInterReceive = std::atoi(value.c_str());
}

void		ServerConfig::setTimeoutKeepAlive(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutKeepAlive = std::atoi(value.c_str());
}



void		ServerConfig::addCgiInterpreter(const std::string &value)
{
	std::string	extension;
	std::string	path;
	size_t		colonPos;

	colonPos = value.find(':');
	if (colonPos == std::string::npos)
		goto exitError;
	if (colonPos == 0 || colonPos == value.length() - 1)
		goto exitError;
	if (value.find(':', colonPos + 1) != std::string::npos)
		goto exitError;
	extension = value.substr(0, colonPos);
	path = value.substr(colonPos + 1);

	if (extension.empty() || path.empty())
		goto exitError;
		
	m_cgiInterpreters[extension] = path;

exitSuccess:
	return ;

exitError:
	throw (std::invalid_argument("Invalid 'cgi' value. The input should be in 'extension:path' format."));
	return ;
}

void	ServerConfig::m_setDefaults()
{
	m_max_connections 		= (m_max_connections 	== DefaultConfig::UINT_NONE) ? m_configDefault.server_maxConnections: m_max_connections;
	m_max_concurrent_cgi 	= (m_max_concurrent_cgi == DefaultConfig::UINT_NONE) ? m_configDefault.server_cgiWorkers 	: m_max_concurrent_cgi;
	m_max_cgi_backlog 		= (m_max_cgi_backlog 	== DefaultConfig::UINT_NONE) ? m_configDefault.server_cgiBacklog 	: m_max_cgi_backlog;
	m_max_workers 			= (m_max_workers 		== DefaultConfig::UINT_NONE) ? m_configDefault.server_Workers 		: m_max_workers;
	m_http_maxClientBodySize 	= ((int)m_http_maxClientBodySize 	== DefaultConfig::UINT_NONE) ? m_configDefault.http_maxClientBodySize 	: m_http_maxClientBodySize; 
	m_http_maxClientHeaderSize 	= ((int)m_http_maxClientHeaderSize 	== DefaultConfig::UINT_NONE) ? m_configDefault.http_maxClientHeaderSize : m_http_maxClientHeaderSize; 
	m_http_timeoutFullHeader 	= (m_http_timeoutFullHeader 	== DefaultConfig::UINT_NONE) ? m_configDefault.http_timeoutFullHeader 	: m_http_timeoutFullHeader;
	m_http_timeoutInterSend 	= (m_http_timeoutInterSend 		== DefaultConfig::UINT_NONE) ? m_configDefault.http_timeoutInterSend 	: m_http_timeoutInterSend;
	m_http_timeoutInterReceive 	= (m_http_timeoutInterReceive 	== DefaultConfig::UINT_NONE) ? m_configDefault.http_timeoutInterReceive : m_http_timeoutInterReceive;
	m_http_timeoutKeepAlive 	= (m_http_timeoutKeepAlive 		== DefaultConfig::UINT_NONE) ? m_configDefault.http_timeoutKeepAlive 	: m_http_timeoutKeepAlive;
}

bool	ServerConfig::mf_applyInheritedSettings()
{
	for (std::vector<ServerBlock>::iterator thisBlock = m_serverBlocks.begin(); thisBlock != m_serverBlocks.end(); thisBlock++)
	{
		// fills inherited
		if (!thisBlock->fillInheritedSettings(*this))
			return (false);

		// each location fills inherited
		std::vector<ServerLocation>& locations = thisBlock->accessLocations();
		std::vector<ServerLocation>::iterator thisLocation = locations.begin();
		for (; thisLocation != locations.end(); thisLocation++)
		{
			if (!thisLocation->fillInheritedSettings(*this))
				return (false);
		}
	}

	return (true);
}

const std::vector<Ws::BindInfo>&	ServerConfig::getAllBindAddresses() const
{
	return (m_bindAddresses);
}

// Debug functions
void	ServerConfig::printProgramConfig() const
{
	std::cout << "╔═ Program ══════════════════O\n";
	std::cout << "║ \n";
	std::cout << "║ Maximum Concurrent CGI: " << getMaxConcurrentCgi() << "\n";
	std::cout << "║ Maximum Simultaneous Connections: " << getMaxConnections() << "\n";
	std::cout << "║ Maximum CGI Backlog: " << getMaxCgiBacklog() << "\n";
	std::cout << "║ \n";
}

void	ServerConfig::printConfigs() const
{
	printProgramConfig();
	for (std::vector<ServerBlock>::const_iterator thisBlock = getServerBlocks().begin(); thisBlock != getServerBlocks().end(); thisBlock++)
	{
		thisBlock->printServerConfig();
		if (!thisBlock->getLocations().empty())
			for (size_t i = 0; i < thisBlock->getLocations().size(); i++)
				thisBlock->getLocations()[i].printLocationConfig();
		std::cout << "║ └──────────────────o\n";
	}
	std::cout << "╚═════════════════════════════O" << std::endl;
}
