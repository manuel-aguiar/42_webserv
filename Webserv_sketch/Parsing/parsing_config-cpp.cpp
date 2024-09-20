#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#define DEFAULT_CONFIG_PATH "webserv.conf"
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_CLIENT_BODY_SIZE 1024 * 1024 // 1MB
#define SERVER_AMOUNT 2 // ConfigHandler.countServers will count the sv's on a config file

// Util Function
size_t	__stoull(const std::string &str)
{
	std::stringstream	ss(str);
	size_t				value;

	ss >> value;
    if (ss.fail() || !ss.eof())
		throw (std::invalid_argument("Invalid value (not a number): " + str));
	return (value);
}

// Util Function
std::string	strtrim(const std::string &str)
{
	if (str.empty())
		return (str);

	size_t	first	= str.find_first_not_of(' ');
	size_t	last	= str.find_last_not_of(' ');

	return (str.substr(first, (last - first + 1)));
}

// Util Function
size_t	parse_size(const std::string &size_str)
{
	size_t	multiplier = 1;
	std::reverse_iterator<std::string::const_iterator>	it;

	it = size_str.rbegin();

	if (*it == 'K')
		multiplier = 1024;
	else if (*it == 'M')
		multiplier = 1024 * 1024;
	else if (*it == 'G')
		multiplier = 1024 * 1024 * 1024;
	else if (!isdigit(*it))
		throw (std::invalid_argument("Invalid unit: " + *it));
	else
		return (__stoull(size_str) * multiplier);
	return (__stoull(size_str.substr(0, size_str.size() - 1)) * multiplier);
}

class ServerConfig {
	private:
		std::string					_host;
		std::vector<int>			_port;								// Multiple ports
		std::vector<std::string>	_server_name;						// Multiple server_names
		size_t						_client_body_size;
		std::string					_root;								// path to server folder
		std::map<int, std::string>	_error_pages;						// Maps status codes to file paths
		int							_error_pages_count;				// Maps status codes to file paths

		// std::map<std::string, LocationConfig>	locations;

	public:
		ServerConfig(): _host(DEFAULT_HOST), _client_body_size(DEFAULT_CLIENT_BODY_SIZE), _error_pages_count(0) {}

		int	setHost(const std::string &value) {
			this->_host = std::string(value);
			return (1);
		}
		int addPort(const int &value) {
			// TODO: check if is already on list and is valid
			this->_port.push_back(value);
			return (1);
		}
		int addServerName(const std::string &value) {
			// TODO: check if is already on list and is valid
			this->_server_name.push_back(value);
			return (1);
		}
		int	setRootPath(const std::string &value) {
			this->_root = value;
			return (1);
		}
		int	setClientBodySize(const std::string &value) {
			this->_client_body_size = parse_size(value); // exception __stoull
			return (1);
		}
		int	addErrorPage(const int &errorCode, const std::string &value){
			this->_error_pages_count++;
			this->_error_pages[errorCode] = value;
			return (1);
		}
		
		std::string	getHost() const {
			return (_host);
		}
		int			getPort(size_t value) const {
			if (value >= _port.size())
				return (-1);
			return (_port[value]);
		}
		size_t		getPortCount() const {
			return (_port.size());
		}		
		std::string	getServerName(size_t value) const {
			if (value >= _port.size())
				return (std::string());
			return (_server_name[value]);
		}
		int			getServerNameCount() const {
			return (_server_name.size());
		}
		int			getClientBodySize() const{
			return (_client_body_size);
		}
		std::string	getRoot() const {
			return (_root);
		}
		int			getErrorPagesCount() {
			return (_error_pages_count);
		}
};

class ConfigHandler {
	private:
		std::string		_path;
		std::ifstream	_file;
		size_t			_serverCount;
		ServerConfig	*_servers;				/* 'servers' address would be shared with
															 * another class that will be the sv manager?
															 * same for server_count	*/
		int countServers() {
			// TODO: count servers on current file
			_serverCount = SERVER_AMOUNT;
			return (1);
		}

	public:
		ConfigHandler(): _path(std::string(DEFAULT_CONFIG_PATH)), _serverCount(SERVER_AMOUNT), _servers(NULL) {
			// Servers address would come from somewhere else?
			_servers = new ServerConfig[_serverCount];
		}
		~ConfigHandler(){
			delete[] _servers;
		}

		int				getServerCount() const {
			return (_serverCount);
		}
		std::string		getConfigPath() const {
			return (_path);
		}

		int		setConfigPath(std::string new_path) {
			this->_path = new_path;
			return (1);
		}
		int		updateFile() {
			_file = std::ifstream(_path);
			if (!_file.is_open()) {
				std::cerr << "Error: Could not open configuration file." << std::endl;
				return (0);
			}
			countServers();
			return (1);
		}


		int		parse_config_line(std::string &line, ServerConfig &server, size_t &current_line)
		{
			std::istringstream iss(line);
			std::string key, value;
			iss >> key;
			iss >> value;

			if (*value.rbegin() == ';')
				value = value.substr(0, value.size() - 1);

			if (key == "host")
			{
				server.setHost(value);
			}
			else if (key == "port")
			{
				server.addPort(__stoull(value)); // catch exception
				while (iss >> value)
					server.addPort(__stoull(value)); // catch exception
			}
			else if (key == "server_name")
			{
				server.addServerName(value);
				while (iss >> value)
					server.addServerName(value);
			}
			else if (key == "root")
			{
				server.setRootPath(value);
			}
			else if (key == "client_body_size")
			{
				server.setClientBodySize(value);
			}
			else if (key == "error_page")
			{
				int error_code = __stoull(value); // catch exception
				iss >> value;			
				server.addErrorPage(error_code, value);
			}
			else
			{
				std::cout << "Error: config parsing: invalid argument: " << "\""
					<< line << "\"" << " on line " << current_line << std::endl;
				return (0);
			}
			return (1);
		}
		int		parse_config_file()
		{
			updateFile();

			std::string	line;
			size_t		current_line	= 0;
			size_t		current_server	= 0;
			size_t		server_count	= 0;
			size_t		bracket_level	= 0; /* unused at the moment, not sure if will be needed
											* for stuff like "location {}" */

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
						if (!parse_config_line(line, _servers[server_count - 1], current_line))
							return (0);
				}
			}
			if (current_server)
			{
				std::cout << "Error: config parsing: server missing closing bracket" << std::endl;
				return (0);
			}
			return (1);
		}
		
		// Debug functions
		void	print_server_config(const ServerConfig &server)
{
	size_t	portCount = server.getPortCount();
	size_t	serverNameCount = server.getServerNameCount();
	
	std::cout << "Host: " << server.getHost() << std::endl;
	
	std::cout << "Ports: ";
	if (portCount <= 0)
		std::cout << "(empty)";
	else
		for (size_t i = 0; i < portCount; i++)
			std::cout << server.getPort(i) << " ";
	std::cout << std::endl;

	std::cout << "Server_names: ";
	if (serverNameCount <= 0)
		std::cout << "(empty)" << std::endl;
	else
		for (size_t i = 0; i < serverNameCount; i++)
			std::cout << server.getServerName(i) << " ";
	std::cout << std::endl;
	
	std::cout << "Client Body Size: " << server.getClientBodySize() << std::endl;
	std::cout << "Root: " << server.getRoot() << std::endl;
	// std::cout << "Error_page: ";
	// if (!server_config.error_pages.empty())
	// {
	// 	std::map<int, std::string>::const_iterator it = server_config.error_pages.begin();
	// 	while (1)
	// 	{
	// 		std::cout << it->first << " " << it->second;
	// 		it++;
	// 		if (it != server_config.error_pages.end())
	// 			std::cout << ", ";
	// 		else
	// 		{
	// 			std::cout << std::endl;
	// 			break ;
	// 		}
	// 	}
	// }
	// else
	// 	std::cout << "(empty)" << std::endl;
}
		void	print_servers_config()
{
	for (size_t i = 0; i < _serverCount; i++)
	{
		std::cout << "=== Server " << i << " parameters ===" << std::endl;
		print_server_config(_servers[i]);
	}
}

};

int main(int argc, char **argv)
{
	ConfigHandler	configHandler;

	// Set config-file path
	if (argc > 1)
		configHandler.setConfigPath(argv[1]);
	else
		configHandler.setConfigPath(DEFAULT_CONFIG_PATH);
	
	// Parse config file
	if (!configHandler.parse_config_file())
		return (1);
	else
	{
		std::cout << "Config parsing done! Showing stored information:" << std::endl;
		configHandler.print_servers_config();
	}
	// delete[] servers;
}