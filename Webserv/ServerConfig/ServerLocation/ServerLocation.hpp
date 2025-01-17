

#ifndef SERVERLOCATION_HPP

# define SERVERLOCATION_HPP

// C++ Headers
# include <string>
# include <set>
# include <map>
# include <iostream>
# include <exception>
# include <sstream>

// Own Headers
# include "../../GenericUtils/Validation/Validation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../DefaultConfig/DefaultConfig.hpp"

class ServerBlock;

class ServerLocation
{
	public:

		typedef enum e_locationType
		{
			E_LOCATION_REGULAR,
			E_LOCATION_REDIRECTION,
			E_LOCATION_CGI
		}	t_locationType;
	
		ServerLocation();
		~ServerLocation();
		ServerLocation &operator=(const ServerLocation &other);
		ServerLocation(const ServerLocation &other);

		// Getters & Setters
		const ServerBlock&				getServerBlock() const;
		const std::string&				getPath() const;
		const std::string&				getRoot() const;
		bool							getAutoindex() const;
		const std::set<std::string>&	getMethods() const;
		std::string						getType() const;
		void							setType(const std::string &value);
		void							setPath(const std::string &value);
		void							setRoot(const std::string &value);
		void							setAutoindex(const std::string &value);
		void							addMethod(const std::string &value);

		void							setDefaults();
		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// DEBUG
		void							printLocationConfig() const;

	private:

		// Key/value storing for config settings
		typedef void (ServerLocation::*f_addConfigValue)(const std::string &);
		std::map<std::string, f_addConfigValue> 		m_keys;

		std::set<std::string> 			m_validTypes;
		std::set<std::string>			m_validMethods;

		std::string						m_path;
		std::string						m_root;
		std::string						m_type;
		std::string						m_autoIndex;
		std::set<std::string>			m_methods;
		// some cgi stuff with path and extension here
		// some redirection stuff with URL to follow here
};



#endif
