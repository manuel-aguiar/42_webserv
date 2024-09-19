#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#define DEFAULT_CONFIG_PATH "webserv.conf"

struct ServerConfig {
	std::string					host				= "127.0.0.1";
	std::vector<int>			port;								// Multiple ports
	std::vector<std::string>	server_name;						// Multiple server_names
	size_t						client_body_size	= 1024 * 1024;	// Default size (1 MB)
	std::string					root;								// path to server folder
	std::map<int, std::string>	error_pages;						// Maps status codes to file paths
	// std::map<std::string, LocationConfig>	locations;
};

// Util Function
std::string	strtrim(const std::string &str)
{
	if (str.empty())
		return (str);

	size_t	first	= str.find_first_not_of(' ');
	size_t	last	= str.find_last_not_of(' ');

	return (str.substr(first, (last - first + 1)));
}

size_t	count_servers(const std::string &)
{

}

size_t	parse_size(const std::string &size_str)
{
	size_t	multiplier = 1;

	if (size_str.back() == 'K')
		multiplier = 1024;
	else if (size_str.back() == 'M')
		multiplier = 1024 * 1024;
	else if (size_str.back() == 'G')
		multiplier = 1024 * 1024 * 1024;

	return (std::stoull(size_str) * multiplier);
}

// Debug Function
void	print_server_config(const ServerConfig &server_config)
{
	std::cout << "Host: " << server_config.host << std::endl 
		<< "Ports: ";
	if (!server_config.port.empty())
	{
		std::vector<int>::const_iterator it = server_config.port.begin();
		while (1)
		{
			std::cout << *it;
			it++;
			if (it != server_config.port.end())
				std::cout << ", ";
			else
			{
				std::cout << std::endl;
				break ;
			}
		}
	}
	else
		std::cout << "(empty)" << std::endl;
	std::cout << "Server_name: ";
	if (!server_config.server_name.empty())
	{
		std::vector<std::string>::const_iterator it = server_config.server_name.begin();
		while (1)
		{
			std::cout << *it;
			it++;
			if (it != server_config.server_name.end())
				std::cout << ", ";
			else
			{
				std::cout << std::endl;
				break ;
			}
		}
	}
	else
		std::cout << "(empty)" << std::endl;
	std::cout << "Client Body Size: " << server_config.client_body_size << std::endl
		<< "Root: " << server_config.root << std::endl;
	std::cout << "Error_page: ";
	if (!server_config.error_pages.empty())
	{
		std::map<int, std::string>::const_iterator it = server_config.error_pages.begin();
		while (1)
		{
			std::cout << it->first << " " << it->second;
			it++;
			if (it != server_config.error_pages.end())
				std::cout << ", ";
			else
			{
				std::cout << std::endl;
				break ;
			}
		}
	}
	else
		std::cout << "(empty)" << std::endl;
}

int	parse_config_line(std::string &line, ServerConfig &server_config, size_t &current_line)
{
	std::istringstream iss(line);
	std::string key, value;
	iss >> key;
	iss >> value;

	if (value.back() == ';')
		value.pop_back();

	if (key == "host")
	{
		server_config.host = value;
	}
	else if (key == "port")
	{
		server_config.port.push_back(std::stoi(value)); // execption stoi
		while (iss >> value)
			server_config.port.push_back(std::stoi(value)); // exception stoi
	}
	else if (key == "server_name")
	{
		server_config.server_name.push_back(value);
		while (iss >> value)
			server_config.server_name.push_back(value);
	}
	else if (key == "root")
	{
		server_config.root = value;
	}
	else if (key == "client_body_size")
	{
		server_config.client_body_size = parse_size(value);
	}
	else if (key == "error_page")
	{
		int error_code = std::stoi(value); // exception stoi
		iss >> value;			
		server_config.error_pages[error_code] = value;
	}
	else
	{
		std::cout << "Error: config parsing: invalid argument: " << "\""
			<< line << "\"" << " on line " << current_line << std::endl;
		return (EXIT_FAILURE);
	}
}

int	parse_config_file(const std::string &path, ServerConfig &server_config)
{
	std::ifstream	config_file(path);

	if (!config_file.is_open()) {
		std::cerr << "Error: Could not open configuration file." << std::endl;
		return (EXIT_FAILURE);
	}

	std::string	line;
	size_t		current_line	= 0;
	size_t		current_server	= 0;
	size_t		server_count	= 0;
	size_t		bracket_level	= 0; /* unused at the moment, not sure if will be needed
									  * for stuff like "location {}" */

	while (std::getline(config_file, line))
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
				return (EXIT_FAILURE);
			}
			server_count++;
			bracket_level++;
			current_server = server_count;
		}
		else if (line == "}")
		{
			if (!current_server)
			{
				std::cerr << "Error: config parsing: stray closing bracket on line "
					<< current_line << std::endl;
				return (EXIT_FAILURE);
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
				parse_config_line(line, server_config, current_line);
		}
	}
	if (current_server)
	{
		std::cout << "Error: config parsing: server missing closing bracket" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	ServerConfig	server_config; // Only one server
	std::string		config_path;

	// Get config file path
	if (argc > 1)
		config_path = argv[1];
	else
		config_path = DEFAULT_CONFIG_PATH;
	
	// Parse config file
	if (parse_config_file(config_path, server_config))
		return	(EXIT_FAILURE);
	else
	{
		std::cout << "Config parsing done! Showing stored information:" << std::endl;
		print_server_config(server_config);
	}
}