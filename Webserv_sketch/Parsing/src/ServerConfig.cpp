#include "ServerConfig.hpp"

// Private

// Public
ServerConfig::ServerConfig()
{
	_keys["host"]				= &ServerConfig::setHost;
	_keys["port"]				= &ServerConfig::addPort;
	_keys["server_names"]		= &ServerConfig::addServerName;
	_keys["client_body_size"]	= &ServerConfig::setClientBodySize;
	_keys["client_header_size"]	= &ServerConfig::setClientHeaderSize;
	_keys["max_connections"]	= &ServerConfig::setMaxConnections;
	_keys["max_concurrent_cgi"]	= &ServerConfig::setMaxConcurrentCGI;
	_keys["root"]				= &ServerConfig::setRootPath;
	_keys["error_pages"]		= &ServerConfig::addErrorPage;

	_config["host"];
	_config["port"];
	_config["server_names"];
	_config["client_body_size"];
	_config["client_header_size"];
	_config["max_connections"];
	_config["max_concurrent_cgi"];
	_config["root"];
	_config["error_pages"];
	// Locations
}

bool	ServerConfig::setHost(const std::string &value, const int &flag)
{
	if (!flag && !_config["host"].empty())
		throw (std::invalid_argument("host already set"));
	if (!validate_ipv4(value))
		throw (std::invalid_argument("not an ipv4 address"));
	_config["host"].clear();
	_config["host"].insert(value);
	return (1);
}

bool	ServerConfig::setRootPath(const std::string &value, const int &flag)
{
	if (!flag && !_config["root"].empty())
		throw (std::invalid_argument("root path already set"));

	// is there any check to do here?
	
	_config["root"].clear();
	_config["root"].insert(value);
	return (1);
}

bool	ServerConfig::setClientBodySize(const std::string &value, const int &flag)
{
	if (!flag && !_config["client_body_size"].empty())
		throw (std::invalid_argument("client body size already set"));
	try {
		parse_size(value);
	}
	catch (std::exception &e) {
		std::cerr << e.what();
		return (0);
	}
	_config["client_body_size"].clear();
	_config["client_body_size"].insert(value);

	return (1);
}

bool	ServerConfig::setClientHeaderSize(const std::string &value, const int &flag)
{
	if (!flag && !_config["client_header_size"].empty())
		throw (std::invalid_argument("client header size already set"));
	try {
		parse_size(value);
	}
	catch (std::exception &e) {
		std::cerr << e.what();
		return (0);
	}
	_config["client_header_size"].clear();
	_config["client_header_size"].insert(value);

	return (1);
}

bool	ServerConfig::setMaxConnections(const std::string &value, const int &flag)
{
	if (!flag && !_config["max_connections"].empty())
		throw (std::invalid_argument("max connections already set"));
	if (!is_number(value))
		throw (std::invalid_argument("value not a number"));
	_config["max_connections"].clear();
	_config["max_connections"].insert(value);


	return (1);

}

bool	ServerConfig::setMaxConcurrentCGI(const std::string &value, const int &flag)
{
	if (!flag && !_config["max_concurrent_cgi"].empty())
		throw (std::invalid_argument("max concurrent cgi already set"));
	if (!is_number(value))
		throw (std::invalid_argument("value not a number"));
	_config["max_concurrent_cgi"].clear();
	_config["max_concurrent_cgi"].insert(value);

	return (1);
}



bool	ServerConfig::addPort(const std::string &value, const int &flag)
{
	(void)flag;
	if (!is_number(value) || (_config["port"].find(value) != _config["port"].end()))
		return (0);
	_config["port"].insert(value);

	return (1);
}

bool	ServerConfig::addServerName(const std::string &value, const int &flag)
{
	(void)flag;
	// are there any more checks to do here?
	if ((_config["server_names"].find(value) != _config["server_names"].end()))
		return (0);
	_config["server_names"].insert(value);

	return (1);
}

bool	ServerConfig::addErrorPage(const std::string &value, const int &flag)
{
	(void)flag;
	std::stringstream	ss;
	std::string			error_code;
	std::string			path; 
	size_t				separator;

	ss << value;
	separator = value.find_first_of(':', 0);
	if (separator == std::string::npos)
		throw (std::invalid_argument("no separator \":\" found while adding error page (errorValue:path)"));
	error_code = value.substr(0, separator);
	if (!is_number(error_code))
		throw (std::invalid_argument("error code is not a number: " + error_code));
	// path = value.substr(separator + 1, value.size() - (separator - 1));			// To retrieve the path
	_config["error_pages"].insert(ss.str());

	return (1);
}

bool	ServerConfig::addConfigValue(const std::string &key, const std::string &value)
{
	if (_keys.find(key) == _keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*_keys[key])(value, 0);

	return (1);
}

std::string	ServerConfig::getHost() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("host");

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

std::set<std::string>	ServerConfig::getPorts() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("port");

	if (it != _config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

std::set<std::string>	ServerConfig::getServerNames() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("server_names");

	if (it != _config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

std::string	ServerConfig::getClientBodySize() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("client_body_size");

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

std::string	ServerConfig::getClientHeaderSize() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("client_header_size");

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

std::string	ServerConfig::getMaxConnections() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("max_connections");

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

std::string	ServerConfig::getMaxConcurrentCGI() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("max_concurrent_cgi");
	

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}


std::set<std::string>	ServerConfig::getErrorPages() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("error_pages");

	if (it != _config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

std::string	ServerConfig::getRoot() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = _config.find("root");

	if (it != _config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

void	ServerConfig::set_defaults(const int &flag)
{
	try {
		setHost(DEFAULT_HOST, flag);
	}
	catch (std::exception &e) {}
	if (_config["port"].empty())
		_config["port"].insert(DEFAULT_PORT);
	try {
		setClientBodySize(DEFAULT_CLIENT_BODY_SIZE, flag);
	}
	catch (std::exception &e) {}
	try {
		setClientHeaderSize(DEFAULT_HEADER_SIZE, flag);
	}
	catch (std::exception &e) {}
	try {
		setMaxConnections(DEFAULT_MAX_CONNECTIONS, flag);
	}
	catch (std::exception &e) {}
	try {
		setMaxConcurrentCGI(DEFAULT_MAX_CONCURRENT_CGI, flag);
	}
	catch (std::exception &e) {}
}
