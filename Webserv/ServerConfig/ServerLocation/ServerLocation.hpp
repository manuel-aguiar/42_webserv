/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:56:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 10:18:01 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATION_HPP

# define SERVERLOCATION_HPP

// C++ Headers
# include <string>
# include <set>
# include <map>
# include <iostream>
# include <exception>

// Own Headers
# include "../../GenericUtils/Validation/Validation.hpp"
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

		// ServerLocation(ServerBlock& block, t_locationType type);    //non-const, very likely location will have to update the block...?
		
		ServerLocation();
		ServerLocation(ServerBlock& server);
		~ServerLocation();
		ServerLocation &operator=(const ServerLocation &other);
		ServerLocation(const ServerLocation &other);

		// Getters & Setters
		const ServerBlock&				getServerBlock() const;
		t_locationType					getLocationType() const;
		const std::string&				getPath() const;
		const std::string&				getRoot() const;
		bool							getAutoindex() const;
		const std::set<std::string>&	getMethods() const;
		const int						getType() const;
		bool							setType(const std::string &value, const int &flag = 0);
		bool							setPath(const std::string &value, const int &flag = 0);
		bool							setRoot(const std::string &value, const int &flag = 0);
		bool							setAutoindex(const std::string &value, const int &flag = 0);
		bool							addMethod(const std::string &value, const int &flag = 0);

		void							setDefaults(const int &flag = 0);
		bool							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// DEBUG
		void							printLocationConfig() const;

	private:

		// Key/value storing for config settings
		typedef bool (ServerLocation::*f_addConfigValue)(const std::string &, const int &);
		std::map<std::string, std::set<std::string> > 	m_config;
		std::map<std::string, f_addConfigValue> 		m_keys;

		ServerBlock*					m_block; // this would be a reference if used
		t_locationType					m_type;
		std::set<std::string> 			m_validTypes;

		// some cgi stuff with path and extension here
		// some redirection stuff with URL to follow here
};



#endif
