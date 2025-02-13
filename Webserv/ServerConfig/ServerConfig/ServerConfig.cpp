

// Own Headers

# include "ServerConfig.hpp"
# include "../ServerLocation/ServerLocation.hpp"
# include "../ServerBlock/ServerBlock.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../GenericUtils/Validation/Validation.hpp"

// C++ headers
# include <cstdlib> // atoi

ServerConfig::ServerConfig(const char* configFilePath, const DefaultConfig& defaultConfig) :
	m_max_connections(DefaultConfig::UINT_NONE),
	m_max_concurrent_cgi(DefaultConfig::UINT_NONE),
	m_max_cgi_backlog(DefaultConfig::UINT_NONE),
	m_max_workers(DefaultConfig::UINT_NONE),	
	m_configDefault(defaultConfig),
	m_configFilePath(configFilePath),
	m_serverCount(0)
{
	m_keys["max_connections"]		= &ServerConfig::setMaxConnections;
	m_keys["max_concurrent_cgi"]	= &ServerConfig::setMaxConcurrentCgi;
	m_keys["max_cgi_backlog"]		= &ServerConfig::setMaxCgiBacklog;
	m_keys["max_workers"]			= &ServerConfig::setMaxWorkers;
	m_keys["cgi"]					= &ServerConfig::addCgiInterpreter;
}

ServerConfig::~ServerConfig()
{
	// Nothing to do here
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this == &other)
		return (*this);

	m_keys = other.m_keys;
	m_max_concurrent_cgi = other.m_max_concurrent_cgi;
	m_max_connections = other.m_max_connections;
	m_max_cgi_backlog = other.m_max_cgi_backlog;
	m_max_workers = other.m_max_workers;
	m_configFilePath = other.m_configFilePath;
	m_serverBlocks = other.m_serverBlocks;
	m_serverCount = other.m_serverCount;

	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig &other) :
	m_configDefault(other.m_configDefault)
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
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value);
}

int		ServerConfig::m_parseConfigLine(const std::string &line, const size_t &currentLine, std::vector<ServerBlock> &servers,
										std::vector<ServerLocation> &locations, const int &currentLevel)
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
					locations.back().addConfigValue(key, value);
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
										std::vector<ServerBlock> &servers, 
										std::vector<ServerLocation> &locations)
{
	switch (currentLevel)
	{
		case PROGRAM_LEVEL:
			std::cerr << "Error: config parsing: stray closing bracket on line "
				<< currentLine << std::endl;
			return (0);
		case SERVER_LEVEL:
			currentLevel = PROGRAM_LEVEL;
			servers.back().setDefaults(m_configDefault);
			if (!servers.back().validate())
			{
				std::cerr << "Error: config parsing: invalid server block closing on line "
					<< currentLine << std::endl;
				return (0);
			}
			servers.back().setLocations(locations);
			locations.clear();
			break ;
		case LOCATION_LEVEL:
			currentLevel = SERVER_LEVEL;
			locations.back().setDefaults(m_configDefault);
			if (!locations.back().validate())
			{
				std::cerr << "Error: config parsing: invalid location block closing on line "
					<< currentLine << std::endl;
				return (0);
			}
			break ;
		default:
			std::cerr << "Parsing: Unexpected Error" << std::endl;
			return (0);
	}
	return (1);
}

int		ServerConfig::parseConfigFile()
{

	std::string		line;
	size_t			currentLine = 0;
	int				currentLevel = PROGRAM_LEVEL;
	ServerLocation	location;

	std::vector<ServerLocation>			locations;

	if (!m_updateFile())
		return (0);
	if (m_serverCount)
	{
		std::cerr << "Error: parsing after startup not implemented" << std::endl;
		return (0);
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
				return (0);
			}
			m_serverBlocks.push_back(ServerBlock());
			m_serverCount++;
			currentLevel = SERVER_LEVEL;
		}
		else if (line == "location {")
		{
			if (currentLevel != SERVER_LEVEL)
			{
				std::cerr << "Error: config parsing: location not on server level on line "
					<< currentLine << std::endl;
				return (0);
			}
			locations.push_back(ServerLocation());
			currentLevel = LOCATION_LEVEL;
		}
		else if (line == "}")
		{
			if (!m_handleClosingBracket(currentLevel, currentLine, m_serverBlocks, locations))
				return (0);
		}
		else
		{
			if (!m_parseConfigLine(line, currentLine, m_serverBlocks, locations, currentLevel))
			{
				std::cerr << "Error: config parsing: invalid input on line "
					<< currentLine << std::endl;
				return (0);
			}
		}
	}
	if (m_serverCount == 0)
	{
		std::cerr << "Error: no server configurations on config file" << std::endl;
		return (0);
	}
	if (currentLevel != PROGRAM_LEVEL)
	{
		std::cerr << "Error: missing closing bracket" << std::endl;
		return (0);
	}
	if (!mf_listenDNSlookup()
	|| !mf_expandCgiToLocations())
		return (0);
		
	m_setDefaults();
	return (1);
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

const DefaultConfig&	ServerConfig::getDefaultConfig() const
{
	return (m_configDefault);
}

void		ServerConfig::setMaxConnections(const std::string &value)
{
	size_t	number;
	
	try {
		number = StringUtils::stoull(value);
		if (number > 1048576)
			throw (std::invalid_argument("max_connections value too high"));
		if (value[0] == '-')
			throw (std::invalid_argument("max_connections must be a positive number,"));
	}
	catch (std::exception &e){
		std::string msg = e.what();
		throw (std::invalid_argument(msg));
	}
	m_max_connections = std::atoi(value.c_str());
}

void		ServerConfig::setMaxConcurrentCgi(const std::string &value)
{
	size_t	number;
	
	try {
		number = StringUtils::stoull(value);
		if (number > 1048576)
			throw (std::invalid_argument("max_concurrent_cgi value too high"));
		if (value[0] == '-')
			throw (std::invalid_argument("max_concurrent_cgi must be a positive number,"));
	}
	catch (std::exception &e){
		std::string msg = e.what();
		throw (std::invalid_argument(msg));
	}
	m_max_concurrent_cgi = std::atoi(value.c_str());
}

void	ServerConfig::setMaxCgiBacklog(const std::string &value)
{
	size_t	number;
	
	try {
		number = StringUtils::stoull(value);
		if (number > 1048576)
			throw (std::invalid_argument("max_cgi_backlog value too high"));
		if (value[0] == '-')
			throw (std::invalid_argument("max_cgi_backlog must be a positive number,"));
	}
	catch (std::exception &e){
		std::string msg = e.what();
		throw (std::invalid_argument(msg));
	}
	m_max_cgi_backlog = std::atoi(value.c_str());
}

void		ServerConfig::setMaxWorkers(const std::string &value)
{
	size_t	number;
	
	try {
		number = StringUtils::stoull(value);
		if (number > 1048576)
			throw (std::invalid_argument("max_concurrent_cgi value too high"));
		if (value[0] == '-')
			throw (std::invalid_argument("max_concurrent_cgi must be a positive number,"));
	}
	catch (std::exception &e){
		std::string msg = e.what();
		throw (std::invalid_argument(msg));
	}
	m_max_workers = std::atoi(value.c_str());
}

void		ServerConfig::addCgiInterpreter(const std::string &value)
{
	std::string	extension;
	std::string	path;
	size_t		colonPos;

	colonPos = value.find(':');
	if (colonPos == 0 || colonPos == value.length() - 1)
		goto exitError;
	if (value.find(':', colonPos + 1) != std::string::npos)
		goto exitError;
	extension = value.substr(0, colonPos);
	path = value.substr(colonPos + 1);

	if (extension.empty() || path.empty())
		goto exitError;
		
	m_cgiInterpreters[extension] = path;

	return ;

exitError:
	throw (std::invalid_argument("Invalid 'cgi' value. The input should be in 'extension:path' format."));
}

void	ServerConfig::m_setDefaults()
{
	m_max_connections 		= (m_max_connections == -1) 	? m_configDefault.server_maxConnections 	: m_max_connections;
	m_max_concurrent_cgi 	= (m_max_concurrent_cgi == -1) 	? m_configDefault.server_cgiWorkers 		: m_max_concurrent_cgi;
	m_max_cgi_backlog 		= (m_max_cgi_backlog == -1) 	? m_configDefault.server_cgiBacklog 		: m_max_cgi_backlog;
	m_max_workers 			= (m_max_workers == -1) 		? m_configDefault.server_Workers 			: m_max_workers;
}

// for each server block, for each location, for each cgi entry, uffffffff
bool	ServerConfig::mf_expandCgiToLocations()
{
	for (std::vector<ServerBlock>::iterator it = m_serverBlocks.begin(); it != m_serverBlocks.end(); it++)
	{
		std::map<Ws::path, ServerLocation>::const_iterator it2 = it->getLocations().begin();
		for (; it2 != it->getLocations().end(); it2++)
		{
			std::map<Config::CgiExtension, Config::CgiInterpreter>::iterator it3 = m_cgiInterpreters.begin();
			for (; it3 != m_cgiInterpreters.end(); it3++)
			{
				if (!it3->second.empty())	//location set its own interpreter for this extension (may or may not be equal to the global one)
					continue ;

				std::map<Config::CgiExtension, Config::CgiInterpreter>::iterator serverLevel;
				serverLevel = m_cgiInterpreters.find(it3->first);
				if (serverLevel != m_cgiInterpreters.end())
					it3->second = serverLevel->second;
				else
				{
					std::cerr << "Error: location «" << it2->first << "» 'cgi': no interpreter for extension '" << it3->first << "'" << std::endl;
					return (false);
				}
			}
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
	for (std::vector<ServerBlock>::const_iterator it = getServerBlocks().begin(); it != getServerBlocks().end(); it++)
	{
		it->printServerConfig();
		if (!it->getLocations().empty())
			for (std::map<std::string, ServerLocation>::const_iterator it2 = it->getLocations().begin(); it2 != it->getLocations().end(); it2++)
				it2->second.printLocationConfig();
		std::cout << "║ └──────────────────o\n";
	}
	std::cout << "╚═════════════════════════════O" << std::endl;
}
