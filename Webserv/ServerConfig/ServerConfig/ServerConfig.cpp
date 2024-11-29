/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:10:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/19 13:10:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerConfig.hpp"

ServerConfig::ServerConfig(const char* configFilePath, Globals* globals)
{
	DefaultConfig configDefault;

	m_globals = globals;
	m_configFilePath = configFilePath;
	m_serverCount = 0;

	m_keys["max_connections"]		= &ServerConfig::setMaxConnections;
	m_keys["max_concurrent_cgi"]	= &ServerConfig::setMaxConcurrentCgi;

	m_config["max_connections"];
	m_config["max_concurrent_cgi"];

	setMaxConcurrentCgi(configDefault.maxCGI);
	setMaxConnections(configDefault.maxConnections);
}

ServerConfig::~ServerConfig()
{
	// Nothing to do here
}

ServerConfig::ServerConfig()
{
	// Nothing to do here
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		m_configDefault = other.m_configDefault;
		m_configFilePath = other.m_configFilePath;
		m_serverBlocks = other.m_serverBlocks;
		m_keys = other.m_keys;
		m_config = other.m_config;
	}
	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig &other)
{
	*this = other;
}

int		ServerConfig::m_updateFile()
{
	m_configFileStream.open(m_configFilePath.c_str());
	if (!m_configFileStream.is_open()) {
		std::cerr << "Error: Could not open configuration file." << std::endl;
		return (0);
	}
	return (1);
}

int		ServerConfig::m_setConfigValue(const std::string &key, const std::string &value)
{
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value, 0);

	return (1);
}

int		ServerConfig::m_parseConfigLine(const std::string &line, const size_t &currentLine, ServerBlock &server,
										ServerLocation &location, const int &currentLevel)
{
	std::istringstream iss(line);
	std::string key, value;
	iss >> key;

	if (key.empty() || key[0] == '#') // should we check key.empty?
		return (2);
	while (iss >> value)
	{
		if (*value.rbegin() == ';') // TO FIX: this will clear semicolons at the end of the value (even if not the last one!!)
			value = value.substr(0, value.size() - 1);
		switch (currentLevel)
		{
			case PROGRAM_LEVEL:
				try {
					m_setConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
			case SERVER_LEVEL:
				try {
				server.addConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
			case LOCATION_LEVEL:
				try {
					location.addConfigValue(key, value);
				}
				catch (std::exception &e) {
					std::cerr << e.what() << " on line " << currentLine << std::endl;
					return (0);
				}
				break ;
		}
	}
	if (value.empty())
	{
		std::cerr << "key \"" << key <<  "\" with no value on line " << currentLine << std::endl;
		return (0);
	}
	return (1);
}

int		ServerConfig::parseConfigFile()
{

	std::string		line;
	size_t			currentLine	= 0;
	int				currentLevel	= PROGRAM_LEVEL;
	ServerBlock		server;
	ServerLocation	location;

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
		line = strtrim(line);
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
			server = ServerBlock();
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
			currentLevel = LOCATION_LEVEL;
			location = ServerLocation();
		}
		else if (line == "}")
		{
			switch (currentLevel)
			{
				case PROGRAM_LEVEL:
					std::cerr << "Error: config parsing: stray closing bracket on line "
						<< currentLine << std::endl;
					return (0);
				case SERVER_LEVEL:
					currentLevel = PROGRAM_LEVEL;
					server.setDefaults();
					if (!server.validate())
					{
						std::cerr << "Error: config parsing: invalid server block closing on line "
							<< currentLine << std::endl;
						return (0);
					}
					server.setLocations(m_locations);
					m_servers.push_back(server);
					///* DEBUG */ server.printServerConfig();
					break ;
				case LOCATION_LEVEL:
					currentLevel = SERVER_LEVEL;
					location.setDefaults();
					if (!location.validate())
					{
						std::cerr << "Error: config parsing: invalid location block closing on line "
							<< currentLine << std::endl;
						return (0);
					}
					m_locations.push_back(location);
					///* DEBUG */ location.printLocationConfig();
					break ;
				default:
					std::cerr << "Parsing: Unexpected Error" << std::endl;
					return (0);
			}
		}
		else
		{
			if (!m_parseConfigLine(line, currentLine, server, location, currentLevel))
			{
				std::cerr << "Error: config parsing: invalid input on line "
					<< currentLine << std::endl;
				return (0);
			}
		}
	}
	m_setServers(m_servers);
	return (1);
}

// Getters & Setters
const t_path&		ServerConfig::getConfigPath() const
{
	return (m_configFilePath);
}

const std::map<std::string, ServerBlock>&	ServerConfig::getServerBlocks() const
{
	return (m_serverBlocks);
}

const std::string		&ServerConfig::getMaxWorkers() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("max_workers");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

const std::string		&ServerConfig::getMaxConnections() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("max_connections");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

const std::string		&ServerConfig::getMaxConcurrentCgi() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("max_concurrent_cgi");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

void		ServerConfig::setConfigPath(const t_path &path)
{
	m_configFilePath = path;
}

bool		ServerConfig::setMaxConnections(const std::string &value, const int &flag)
{
	return (m_config["max_connections"].insert(value).second);
}

bool		ServerConfig::setMaxConcurrentCgi(const std::string &value, const int &flag)
{
	return (m_config["max_concurrent_cgi"].insert(value).second);
}

void	ServerConfig::m_setServers(std::vector<ServerBlock> &servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		m_serverBlocks[servers[i].getHost()] = servers[i];
	}
}

// Debug functions

void	ServerConfig::printProgramConfig() const
{
	std::cout << "=== Program Configurations ===" << std::endl;
	std::cout << "Maximum Concurrent CGI: " << getMaxConcurrentCgi() << std::endl;
	std::cout << "Maximum Simultaneous Connections: " << getMaxConnections() << std::endl;
}

void	ServerConfig::printConfigs() const
{
	printProgramConfig();
	for (std::map<std::string, ServerBlock>::const_iterator it = getServerBlocks().begin(); it != getServerBlocks().end(); it++)
	{
		it->second.printServerConfig();
		for (std::map<std::string, ServerLocation>::const_iterator it2 = it->second.getLocations().begin(); it2 != it->second.getLocations().end(); it2++)
			it2->second.printLocationConfig();
	}
}
