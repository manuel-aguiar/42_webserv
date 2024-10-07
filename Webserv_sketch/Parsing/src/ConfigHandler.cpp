#include "ConfigHandler.hpp"

// Public

ConfigHandler::ConfigHandler(): _path(std::string(DEFAULT_CONFIG_PATH)), _serverCount(0) {}

ConfigHandler::~ConfigHandler() {}

// Getters & Setters

int	ConfigHandler::getServerCount() const
{
	return (_serverCount);
}

std::string	ConfigHandler::getConfigPath() const
{
	return (_path);
}

int		ConfigHandler::setConfigPath(std::string new_path)
{
	this->_path = new_path;
	return (1);
}

int		ConfigHandler::updateFile()
{
	_file.open(_path.c_str());
	if (!_file.is_open()) {
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
	
	// if (*value.rbegin() == ';')
	// 	value = value.substr(0, value.size() - 1);
	
	while (iss >> value)
	{
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
	while (std::getline(_file, line))
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
			_serverCount++;
			_servers[_serverCount] = Server;
			bracket_level++;
			current_server = _serverCount;
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
				current_server = 0;
				bracket_level--;
			}
		}
		else
		{
			if (bracket_level == 1)
				if (!parse_config_line(line, _servers[_serverCount - 1], current_line))
					return (0);
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
	
	std::cout << "Client Body Size: " << server.getClientBodySize() << std::endl;
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
	for (size_t i = 0; i < _serverCount; i++)
	{
		std::cout << "=== Server " << i << " parameters ===" << std::endl;
		print_server_config(_servers[i]);
	}
}