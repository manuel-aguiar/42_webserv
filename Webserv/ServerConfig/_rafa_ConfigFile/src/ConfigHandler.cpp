#include "ConfigHandler.hpp"

// Public

ConfigHandler::ConfigHandler(): m_path(std::string(DEFAULT_CONFIG_PATH)), m_serverCount(0) {}

ConfigHandler::~ConfigHandler() {}

// Getters & Setters

int	ConfigHandler::getServerCount() const
{
	return (m_serverCount);
}

const std::string&	ConfigHandler::getConfigPath() const
{
	return (m_path);
}

int		ConfigHandler::setConfigPath(std::string new_path)
{
	this->m_path = new_path;
	return (1);
}

int		ConfigHandler::updateFile()
{
	m_file.open(m_path.c_str());
	if (!m_file.is_open()) {
		std::cerr << "Error: Could not open configuration file." << std::endl;
		return (0);
	}
	return (1);
}

int		ConfigHandler::parse_config_line(std::string &line, ServerConfig &server, size_t &current_line)
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
		try {
			server.addConfigValue(key, value);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << " on line " << current_line << std::endl;
			return (0);
		}
	}
	if (value.empty())
	{
		std::cerr << "key \"" << key <<  "\" with no value on line " << current_line << std::endl;
		return (0);
	}
	return (1);
}

int		ConfigHandler::parse_config_file()
{

	std::string	line;
	size_t		current_line	= 0;
	size_t		current_server	= 0;
	size_t		bracket_level	= 0;

	if (!updateFile())
		return (0);
	if (m_serverCount)
	{
		std::cerr << "Error: re-parsing not implemented" << std::endl;
		return (0);
	}
	while (std::getline(m_file, line))
	{
		current_line++;
		line = strtrim(line);
		if (line.empty() || line[0] == '#')
			continue ;
		if (line == "server {")
		{
			if (current_server)
			{
				std::cerr << "Error: config parsing: server inside server on line "
					<< current_line << std::endl;
				return (0);
			}
			ServerConfig Server;
			m_serverCount++;
			m_servers.push_back(Server);
			bracket_level++;
			current_server = m_serverCount;
		}
		else if (line == "}")
		{
			if (!current_server)
			{
				std::cerr << "Error: config parsing: stray closing bracket on line "
					<< current_line << std::endl;
				return (0);
			}
			else if (bracket_level > 1)
				bracket_level--;
			else
			{
				m_servers[m_serverCount - 1].set_defaults();
				current_server = 0;
				bracket_level--;
			}
		}
		else
		{
			if (current_server)
			{
				if (!parse_config_line(line, m_servers[m_serverCount - 1], current_line))
					return (0);
			}
			else
			{
				std::cerr << "Error: config parsing: invalid line on line "
					<< current_line << std::endl;
				return (0);
			}
		}
	}
	if (current_server)
	{
		std::cerr << "Error: config parsing: server missing closing bracket" << std::endl;
		return (0);
	}
	return (1);
}

// Debug functions

void	ConfigHandler::print_server_config(const ServerConfig &server)
{
	std::set<std::string>	ports = server.getPorts();
	std::set<std::string>	server_names = server.getServerNames();
	std::set<std::string>	error_pages = server.getErrorPages();

	std::cout << "Host: " << server.getHost() << std::endl;

	std::cout << "Ports: ";
	if (!ports.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = ports.begin(); it != ports.end(); it++)
			std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << "Server_names: ";
	if (!server_names.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = server_names.begin(); it != server_names.end(); it++)
			std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << "Client Body Size: " << server.getClientBodySize() << '\n';
	std::cout << "Client Header Size: " << server.getClientHeaderSize() << '\n';
	std::cout << "Max Connections: " << server.getMaxConnections() << '\n';
	std::cout << "Max Concurrent CGI: " << server.getMaxConcurrentCGI() << '\n';

	std::cout << "Root: " << server.getRoot() << std::endl;

	std::cout << "Error_pages: ";
	if (!error_pages.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); it++)
			std::cout << *it << " ";
	std::cout << std::endl;
}

void	ConfigHandler::print_servers_config()
{
	for (size_t i = 0; i < m_servers.size(); i++)
	{
		std::cout << "=== Server " << i << " parameters ===" << std::endl;
		try {
			print_server_config(m_servers[i]);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
