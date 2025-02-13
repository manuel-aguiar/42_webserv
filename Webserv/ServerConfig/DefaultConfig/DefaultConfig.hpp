

#ifndef DEFAULTCONFIG_HPP

# define DEFAULTCONFIG_HPP

// Project Headers
# include "WebservDefaultConfig.h"

// C headers
# include <stddef.h>
# include <string>

struct DefaultConfig
{
	DefaultConfig();

	//blabla all default and public, it is just a macro bearer, getters and setters are a waste;

	// Program
	const int	server_maxConnections;
	const int	server_cgiWorkers;
	const int 	server_cgiBacklog;
	const int	server_Workers;

	// Server
	const char* server_Root;
	const char*	server_IPListen;

	//Http
	const int	http_maxClientBodySize;
	const int	http_maxClientHeaderSize;
	const int	http_timeoutFullHeader;
	const int	http_timeoutInterSend;
	const int	http_timeoutInterReceive;
	const char*	http_error400Path;
	const char*	http_error404Path; // add the remaining error pages

	// Location
	int			loc_autoIndex;
	const char*	loc_http_methods;
	const char*	loc_type;

	enum {UINT_NONE = -1};
};

#endif
