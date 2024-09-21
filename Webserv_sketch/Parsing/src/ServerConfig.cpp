#include "ServerConfig.hpp"

// Private

// Public
ServerConfig::ServerConfig()
{
	_config["host"].insert(DEFAULT_HOST);
	_config["port"];
	_config["server_names"];
	_config["client_body_size"].insert(DEFAULT_CLIENT_BODY_SIZE);
	_config["root"];
	_config["error_pages"];
}

bool	ServerConfig::setHost(const std::string &value)
{
	if (!validate_ipv4(value))
		return (0);
	_config["host"].clear();
	_config["host"].insert(value);
	return (1);
}

bool	ServerConfig::setRootPath(const std::string &value)
{
	// is there any check to do here?
	_config["root"].clear();
	_config["root"].insert(value);
	return (1);
}

bool	ServerConfig::setClientBodySize(const std::string &value)
{
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

bool ServerConfig::addPort(const std::string &value)
{
	if (!is_number(value) || (_config["port"].find(value) != _config["port"].end()))
		return (0);
	_config["port"].insert(value);

	return (1);
}

bool ServerConfig::addServerName(const std::string &value)
{
	// are there any more checks to do here?
	if ((_config["server_names"].find(value) != _config["server_names"].end()))
		return (0);
	_config["server_names"].insert(value);

	return (1);
}

bool	ServerConfig::addErrorPage(const std::string &value)
{
	std::stringstream ss;
	std::string error_code;
	std::string path; 
	size_t separator;

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

bool	ServerConfig::addConfigValue(std::string &key, std::string &value)
{
	if (_config.find(key) == _config.end())
		throw (std::invalid_argument("invalid key " + key));
	for (int i = 0; i < KEY_AMOUNT; i++)
		if (key == _keys[i])
			(this->*f_addConfigValue[i])(value);

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
