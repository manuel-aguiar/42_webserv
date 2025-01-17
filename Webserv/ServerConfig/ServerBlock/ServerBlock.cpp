

#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	m_keys["listen"]				= &ServerBlock::addListener;
	m_keys["server_name"]			= &ServerBlock::addServerName;
	m_keys["client_body_size"]		= &ServerBlock::setClientBodySize;
	m_keys["client_header_size"]	= &ServerBlock::setClientHeaderSize;
	m_keys["root"]					= &ServerBlock::setRootPath;
	m_keys["error_pages"]			= &ServerBlock::addErrorPage;
}

ServerBlock::~ServerBlock()
{
	// Nothing to do here
}

ServerBlock &ServerBlock::operator=(const ServerBlock &other)
{
	if (this == &other)
		return (*this);
	m_listen = other.m_listen;
	m_server_name = other.m_server_name;
	m_client_body_size = other.m_client_body_size;
	m_client_header_size = other.m_client_header_size;
	m_root = other.m_root;
	m_error_pages = other.m_error_pages;
	m_keys = other.m_keys;
	m_locations = other.m_locations;
	return (*this);
}

ServerBlock::ServerBlock(const ServerBlock &other)
{
	*this = other;
}

void	ServerBlock::setRootPath(const std::string &value)
{
	m_root = value;
}

void	ServerBlock::setClientBodySize(const std::string &value)
{
	try {
		StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw(e.what());
	}
	m_client_body_size = value;
}

void	ServerBlock::setClientHeaderSize(const std::string &value)
{
	try {
		StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw(e.what());
	}
	m_client_header_size = value;
}

void	ServerBlock::addListener(const std::string &value)
{
	std::string	hostname;
	std::string	port;
	size_t		portValue;
	size_t		colonPos;

	colonPos = value.find(':');
	if (colonPos == 0 || colonPos == value.length() - 1)
		throw (std::invalid_argument("Invalid 'listen' value. The input should be in 'hostname:port' format."));
	if (colonPos == std::string::npos)
	{
		hostname = DEFAULTCONF_BLOCK_IP_LISTEN;
		port = value;
	}
	else
	{
		if (value.find(':', colonPos + 1) != std::string::npos)
			throw (std::invalid_argument("Invalid 'listen' value. The input should be in 'hostname:port' format."));
		hostname = value.substr(0, colonPos);
		port = value.substr(colonPos + 1);
	}
	portValue = StringUtils::stoull(port); // fix throw
	if (!Validation::isNumber(port) || portValue <= 0 || portValue > 65535)
		throw (std::invalid_argument("Invalid port number. Port must be a number between 1 and 65535."));
	m_listen.insert(t_listeners(hostname, port));
}

void	ServerBlock::addServerName(const std::string &value)
{
	if (m_server_name.find(value) != m_server_name.end())
		throw (std::invalid_argument("server name already set"));
	// up to 253 characters and 1-63 characters long between dots
	m_server_name.insert(value);
}

void	ServerBlock::addErrorPage(const std::string &value)
{
	std::stringstream	ss;
	std::string			error_code;
	std::string			path;
	size_t				separator;

	ss << value;
	separator = value.find_first_of(':', 0);
	if (separator == std::string::npos)
		throw (std::invalid_argument("no separator \":\" found while adding error page (errorValue:path)"));
	error_code = value.substr(0, separator);
	if (!Validation::isNumber(error_code))
		throw (std::invalid_argument("error code is not a number: " + error_code));
	// path = value.substr(separator + 1, value.size() - (separator - 1));			// To retrieve the path
	m_error_pages.insert(value);
	// Get a way to add error pages without repeated values
}

void	ServerBlock::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value);
}

const std::map<std::string, ServerLocation>&		ServerBlock::getLocations() const
{
	return (m_locations);
}

const std::set<t_listeners>&	ServerBlock::getListeners() const
{
	return (m_listen);
}

const std::set<std::string>&	ServerBlock::getServerNames() const
{
	return (m_server_name);
}

size_t	ServerBlock::getClientBodySize() const
{
	return (StringUtils::parse_size(m_client_body_size));
}

size_t	ServerBlock::getClientHeaderSize() const
{
	return (StringUtils::parse_size(m_client_header_size));
}

const std::set<std::string>&	ServerBlock::getErrorPages() const
{
	return (m_error_pages);
}

const std::string&	ServerBlock::getRoot() const
{
	return (m_root);
}

void	ServerBlock::setDefaults()
{
	DefaultConfig	defaults;

	if (m_root.empty())
		setRootPath(defaults.serverRoot);
	if (m_client_body_size.empty())
		setClientBodySize(defaults.maxClientBodySize);
	if (m_client_header_size.empty())
		setClientHeaderSize(defaults.maxClientHeaderSize);
}

bool	ServerBlock::validate() const
{
	if (m_listen.empty())
	{
		std::cerr << "Error: server config validation: listening port not assigned" << std::endl;
		return (0);
	}
	if (m_server_name.empty())
	{
		std::cerr << "Error: server config validation: server_name not assigned" << std::endl;
		return (0);
	}

	return (1);
}

void	ServerBlock::setLocations(const std::vector<ServerLocation> &locations)
{
	if (locations.empty())
		return ;
	
	std::vector<ServerLocation>::const_iterator it = locations.begin();

	for (; it != locations.end(); it++)
	{
		m_locations[it->getPath()] = *it;
	}
}

// Debug

void	ServerBlock::printServerConfig() const
{
	std::set<std::string>	server_name = getServerNames();
	std::set<std::string>	error_pages = getErrorPages();
	
	std::cout << "║ ┌─ Server ─────────o\n";
	std::cout << "║ │ \n";
	std::cout << "║ │ listeners: ";
	for (std::set<t_listeners>::const_iterator it = getListeners().begin(); it != getListeners().end(); it++)
		std::cout << it->first << ":" << it->second << " ";
	std::cout << "\n";
	std::cout << "║ │ server_name: ";
	if (!server_name.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = server_name.begin(); it != server_name.end(); it++)
			std::cout << *it << " ";
	std::cout << "\n";

	std::cout << "║ │ Client Body Size: " << getClientBodySize() << '\n';
	std::cout << "║ │ Client Header Size: " << getClientHeaderSize() << '\n';

	std::cout << "║ │ Root: " << getRoot() << "\n";

	std::cout << "║ │ Error_pages: ";
	if (!error_pages.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); it++)
			std::cout << *it << " ";
	std::cout << "\n";
	std::cout << "║ │ " <<  std::endl ;

}
