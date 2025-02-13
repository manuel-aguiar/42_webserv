

// Own Headers
# include "./ServerLocation.hpp"
# include "../../GenericUtils/Validation/Validation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Ws_Namespace.h"
# include "../DefaultConfig/DefaultConfig.hpp"

ServerLocation::ServerLocation() :
	m_autoIndex(DefaultConfig::UINT_NONE)
{
	m_keys["path"]				= &ServerLocation::setPath;
	m_keys["root"]				= &ServerLocation::setRoot;
	m_keys["type"]				= &ServerLocation::setType;
	m_keys["auto_index"]		= &ServerLocation::setAutoindex;
	m_keys["methods"]			= &ServerLocation::addMethod;
	m_keys["cgi"]				= &ServerLocation::addCgiInterpreter;

	m_validTypes.insert("regular"); 
	m_validTypes.insert("redirection");
	m_validTypes.insert("cgi");

	m_validMethods.insert("get");
	m_validMethods.insert("post");
	m_validMethods.insert("delete");
}

ServerLocation::~ServerLocation()
{
	// Nothing to do here
}

ServerLocation &ServerLocation::operator=(const ServerLocation &other)
{
	if (this == &other)
		return (*this);

	m_keys = other.m_keys;
	m_path = other.m_path;
	m_root = other.m_root;
	m_type = other.m_type;
	m_autoIndex = other.m_autoIndex;
	m_methods = other.m_methods;
	m_validTypes = other.m_validTypes;
	m_validMethods = other.m_validMethods;
	m_cgiInterpreters = other.m_cgiInterpreters;

	return (*this);
}

ServerLocation::ServerLocation(const ServerLocation &other) :
	m_keys				(other.m_keys),
	m_validTypes		(other.m_validTypes),
	m_validMethods		(other.m_validMethods),
	m_path				(other.m_path),
	m_root				(other.m_root),
	m_type				(other.m_type),
	m_autoIndex			(other.m_autoIndex),
	m_methods			(other.m_methods),
	m_cgiInterpreters	(other.m_cgiInterpreters) {}

void	ServerLocation::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value);
}

// Getters & Setters

const std::string&	ServerLocation::getPath() const
{
	return (m_path);
}

const std::string&	ServerLocation::getRoot() const
{
	return (m_root);
}

bool	ServerLocation::getAutoIndex() const
{
	return (m_autoIndex);
}

const std::set<std::string>&	ServerLocation::getMethods() const
{
	return (m_methods);
}

const std::string&	ServerLocation::getType() const
{
	return (m_type);
}

const Config::CgiInterpreterMap&	
ServerLocation::getCgiInterpreters() const
{
	return (m_cgiInterpreters);
}

Config::CgiInterpreterMap&
ServerLocation::accessCgiInterpreters()
{
	return (m_cgiInterpreters);
}

void		ServerLocation::setType(const std::string &value)
{
	if (m_validTypes.find(value) == m_validTypes.end())
		throw (std::invalid_argument("invalid type value"));
	m_type = value;
}

void		ServerLocation::setPath(const std::string &value)
{
	m_path = value;
}

void		ServerLocation::setRoot(const std::string &value)
{
	m_root = value;
}

void		ServerLocation::setAutoindex(const std::string &value)
{
	if (value.size() != 1 || (value[0] != '0' && value[0] != '1'))
		throw (std::invalid_argument("invalid autoIndex value"));
	m_autoIndex = (value[0] == '1');
}

void		ServerLocation::addMethod(const std::string &value)
{
	std::string	lowercaseStr = StringUtils::strToLower(value);
	
	if (m_validMethods.find(lowercaseStr) == m_validMethods.end())
		throw (std::invalid_argument("invalid method"));
	if (m_methods.find(lowercaseStr) == m_methods.end())
		m_methods.insert(lowercaseStr);
}

void	ServerLocation::addCgiInterpreter(const std::string &value)
{
	std::string	extension;
	std::string	path("");
	size_t		colonPos;

	colonPos = value.find(':');
	if (colonPos == 0 || colonPos == value.length() - 1)
		goto exitError;
	if (value.find(':', colonPos + 1) != std::string::npos)
		goto exitError;
	extension = value.substr(0, colonPos);


	if (extension.empty())									// allow extension only, later resolved by ServerConfig
		goto exitError;

	if (colonPos != std::string::npos)
		path = value.substr(colonPos + 1);
		
	m_cgiInterpreters[extension] = path;
	
	return ;

exitError:
	throw (std::invalid_argument("Invalid 'cgi' value. The input should be in 'extension:path' format."));
}


bool		ServerLocation::validate() const
{
	if (m_path.empty())
	{
		std::cerr << "Error: server config validation: path not assigned" << std::endl;
		return (0);
	}
	if (m_root.empty())
	{
		std::cerr << "Error: server config validation: root not assigned" << std::endl;
		return (0);
	}
	return (1);
}

void	ServerLocation::setDefaults(const DefaultConfig& defaultConfig)
{
	std::istringstream iss(defaultConfig.loc_http_methods);
	std::string value;

	if (m_type.empty())
		setType(defaultConfig.loc_type);
	if (m_methods.empty())
		while (iss >> value)
			addMethod(value);
	if (m_autoIndex == DefaultConfig::UINT_NONE)
		m_autoIndex = defaultConfig.loc_autoIndex;
}

void		ServerLocation::printLocationConfig() const
{
	std::cout << "║ │ ┌─ Location ──┐\n";
	std::cout << "║ │ │ path: " << getPath() << "\n";
	std::cout << "║ │ │ root: " << getRoot() << "\n";
	std::cout << "║ │ │ type: " << getType() << "\n";
	std::cout << "║ │ │ autoIndex: " << getAutoIndex() << "\n";
	std::cout << "║ │ └ methods: ";
	for (std::set<std::string>::const_iterator it = getMethods().begin(); it != getMethods().end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}
