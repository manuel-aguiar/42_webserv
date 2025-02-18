

// Project headers
#include "ServerBlock.hpp"
#include "../ServerConfig/ServerConfig.hpp"
#include "../ServerLocation/ServerLocation.hpp"
#include "../DefaultConfig/DefaultConfig.hpp"
#include "../../Ws_Namespace.h"
#include "../../GenericUtils/Validation/Validation.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C++ headers
# include <cstdlib> // for atoi

ServerBlock::DirectiveToSetter ServerBlock::m_directiveToSetter;

ServerBlock::DirectiveToSetter::DirectiveToSetter() :
	map(std::less<std::string>(), mapPool(10))	// 9 magic: number of keys
{
	map["listen"]				= &ServerBlock::addListener;
	map["server_name"]			= &ServerBlock::addServerName;
	map["client_body_size"]		= &ServerBlock::setClientBodySize;
	map["client_header_size"]	= &ServerBlock::setClientHeaderSize;
	map["timeout_full_header"]	= &ServerBlock::setTimeoutFullHeader;
	map["timeout_inter_send"]	= &ServerBlock::setTimeoutInterSend;
	map["timeout_inter_receive"]= &ServerBlock::setTimeoutInterReceive;
	map["timeout_keep_alive"]	= &ServerBlock::setTimeoutKeepAlive;
	map["root"]					= &ServerBlock::setRootPath;
	map["error_pages"]			= &ServerBlock::addErrorPage;
}

ServerBlock::ServerBlock() :
	m_http_maxClientBodySize	(DefaultConfig::UINT_NONE),
	m_http_maxClientHeaderSize	(DefaultConfig::UINT_NONE),
	m_http_timeoutFullHeader	(DefaultConfig::UINT_NONE),
	m_http_timeoutInterSend		(DefaultConfig::UINT_NONE),
	m_http_timeoutInterReceive	(DefaultConfig::UINT_NONE),
	m_http_timeoutKeepAlive		(DefaultConfig::UINT_NONE)  {}

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
	m_http_maxClientBodySize = other.m_http_maxClientBodySize;
	m_http_maxClientHeaderSize = other.m_http_maxClientHeaderSize;
	m_http_timeoutFullHeader = other.m_http_timeoutFullHeader;
	m_http_timeoutInterSend = other.m_http_timeoutInterSend;
	m_http_timeoutInterReceive = other.m_http_timeoutInterReceive;
	m_http_timeoutKeepAlive = other.m_http_timeoutKeepAlive;
	m_root = other.m_root;
	m_error_pages = other.m_error_pages;
	m_locations = other.m_locations;
	m_mapLocations = other.m_mapLocations;

	return (*this);
}

ServerBlock::ServerBlock(const ServerBlock &other) :
	m_listen					(other.m_listen),
	m_server_name				(other.m_server_name),
	m_http_maxClientBodySize	(other.m_http_maxClientBodySize),
	m_http_maxClientHeaderSize	(other.m_http_maxClientHeaderSize),
	m_http_timeoutFullHeader 	(other.m_http_timeoutFullHeader),
	m_http_timeoutInterSend 	(other.m_http_timeoutInterSend),
	m_http_timeoutInterReceive 	(other.m_http_timeoutInterReceive),
	m_http_timeoutKeepAlive 	(other.m_http_timeoutKeepAlive),
	m_root						(other.m_root),
	m_error_pages				(other.m_error_pages),
	m_locations					(other.m_locations),
	m_mapLocations				(other.m_mapLocations) {}

void	ServerBlock::setRootPath(const std::string &value)
{
	m_root = value;
}

void	ServerBlock::setClientBodySize(const std::string &value)
{
	try {
		m_http_maxClientBodySize = StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw (std::invalid_argument(e.what()));
	}
}

void	ServerBlock::setClientHeaderSize(const std::string &value)
{
	try {
		m_http_maxClientHeaderSize = StringUtils::parse_size(value);
	}
	catch (std::exception &e) {
		throw (std::invalid_argument(e.what()));
	}
}

bool	Config::Listen::operator<(const Config::Listen &rhs) const
{
	if (appLayer != rhs.appLayer)
		return (appLayer < rhs.appLayer);
	if (hostname != rhs.hostname)
		return (hostname < rhs.hostname);
	return (port < rhs.port);
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
		
	m_listen.insert((Config::Listen) {
		.appLayer = Ws::AppLayer::HTTP,
		.backlog = Ws::Listen::DEFAULT_BACKLOG,
		.hostname = hostname, 
		.port = port,
		.family = AF_INET,
		.socktype = SOCK_STREAM,
		.proto = IPPROTO_TCP,
	});
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
	std::string			errorCode;
	std::string			path;
	size_t				separator;
	bool				conversionError;

	separator = value.find_first_of(':', 0);
	if (separator == std::string::npos)
		throw (std::invalid_argument("no separator \":\" found while adding error page (errorNumber:path)"));
	errorCode = value.substr(0, separator);
	path = value.substr(separator + 1);
	if (errorCode.empty() || path.empty())
		throw (std::invalid_argument("error code or path is empty"));
	if (!Validation::isNumber(errorCode) || StringUtils::stoull(errorCode) < 100 || StringUtils::stoull(errorCode) > 599)
		throw (std::invalid_argument("error code is not a valid number: " + errorCode + ". It must be a value between 100 and 599"));
	if (m_error_pages.find(StringUtils::strToInt(errorCode, conversionError)) != m_error_pages.end())
		throw (std::invalid_argument("error page already set: " + value));
	m_error_pages[StringUtils::strToInt(errorCode, conversionError)] = path;
}

void	ServerBlock::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_directiveToSetter.map.find(key) == m_directiveToSetter.map.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_directiveToSetter.map[key])(value);
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

void		ServerBlock::setTimeoutFullHeader(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutFullHeader = std::atoi(value.c_str());
}

void		ServerBlock::setTimeoutInterSend(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutInterSend = std::atoi(value.c_str());
}

void		ServerBlock::setTimeoutInterReceive(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutInterReceive = std::atoi(value.c_str());
}

void		ServerBlock::setTimeoutKeepAlive(const std::string &value)
{
	_throw_ifInvalidNumber(value, 100000000);
	m_http_timeoutKeepAlive = std::atoi(value.c_str());
}

const std::vector<ServerLocation>&		ServerBlock::getLocations() const
{
	return (m_locations);
}

const std::map<std::string, const ServerLocation*>&		ServerBlock::getMappedLocations() const
{
	return (m_mapLocations);
}



const std::set<Config::Listen>&	ServerBlock::getListeners() const
{
	return (m_listen);
}

const std::set<std::string>&	ServerBlock::getServerNames() const
{
	return (m_server_name);
}

size_t	ServerBlock::getClientBodySize() const
{
	return (m_http_maxClientBodySize);
}

size_t	ServerBlock::getClientHeaderSize() const
{
	return (m_http_maxClientHeaderSize);
}

int		ServerBlock::getTimeoutFullHeader() const
{
	return (m_http_timeoutFullHeader);
}

int		ServerBlock::getTimeoutInterSend() const
{
	return (m_http_timeoutInterSend);
}

int		ServerBlock::getTimeoutInterReceive() const
{
	return (m_http_timeoutInterReceive);
}

int		ServerBlock::getTimeoutKeepAlive() const
{
	return (m_http_timeoutKeepAlive);
}

const std::map<int, std::string>&	ServerBlock::getErrorPages() const
{
	return (m_error_pages);
}

const std::string&	ServerBlock::getRoot() const
{
	return (m_root);
}


std::vector<ServerLocation>&	ServerBlock::accessLocations()
{
	return (m_locations);
}

std::map<std::string, const ServerLocation*>&	ServerBlock::accessMappedLocations()
{
	return (m_mapLocations);
}

void	ServerBlock::setDefaults(const DefaultConfig& defaultConfig)
{
	if (m_root.empty())
		setRootPath(defaultConfig.server_Root);
}

bool	ServerBlock::fillInheritedSettings(const ServerConfig& config)
{
	m_http_maxClientBodySize 	= ((int)m_http_maxClientBodySize 	== DefaultConfig::UINT_NONE) ? config.getClientBodySize() 	: m_http_maxClientBodySize; 
	m_http_maxClientHeaderSize 	= ((int)m_http_maxClientHeaderSize 	== DefaultConfig::UINT_NONE) ? config.getClientHeaderSize()	: m_http_maxClientHeaderSize; 
	m_http_timeoutFullHeader 	= (m_http_timeoutFullHeader 	== DefaultConfig::UINT_NONE) ? config.getTimeoutFullHeader() 	: m_http_timeoutFullHeader;
	m_http_timeoutInterSend 	= (m_http_timeoutInterSend 		== DefaultConfig::UINT_NONE) ? config.getTimeoutInterSend() 	: m_http_timeoutInterSend;
	m_http_timeoutInterReceive 	= (m_http_timeoutInterReceive 	== DefaultConfig::UINT_NONE) ? config.getTimeoutInterReceive() 	: m_http_timeoutInterReceive;
	m_http_timeoutKeepAlive 	= (m_http_timeoutKeepAlive 		== DefaultConfig::UINT_NONE) ? config.getTimeoutKeepAlive() 	: m_http_timeoutKeepAlive;
	return (true);
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

void	ServerBlock::mapLocations()
{
	if (m_locations.empty())
		return ;
	
	std::vector<ServerLocation>::const_iterator it = m_locations.begin();

	for (; it != m_locations.end(); it++)
	{
		m_mapLocations[it->getPath()] = &(*it);
	}
}

// Debug

void	ServerBlock::printServerConfig() const
{
	std::set<std::string>		server_name = getServerNames();
	std::map<int, std::string>	error_pages = getErrorPages();
	
	std::cout << "║ ┌─ Server ─────────o\n";
	std::cout << "║ │ \n";
	std::cout << "║ │ listeners: ";
	for (std::set<Config::Listen>::const_iterator it = getListeners().begin(); it != getListeners().end(); it++)
		std::cout << it->hostname << ":" << it->port << " ";
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
		for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); it++)
			std::cout << it->first << ":" << it->second << " ";
	std::cout << "\n";
	std::cout << "║ │ " <<  std::endl ;

}

void	ServerBlock::addListenAddress(const struct sockaddr* addr)
{
	m_myListenAddresses.push_back(addr);
}

const std::vector<const struct sockaddr*>&	
ServerBlock::getListenAddresses() const
{
	return (m_myListenAddresses);
}



