

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
	const int	max_connections;
	const int	max_concurrent_cgi;
	const int 	max_cgi_backlog;
	const int	max_workers;

	// Server
	const char* serverRoot;
	const int	maxClientBodySize;
	const int	maxClientHeaderSize;
	const char*	IPListen;
	const char*	http400Path;
	const char*	http404Path; // add the remaining error pages

	// Location
	const char* autoIndex;
	const char*	methods;
	const char*	type;
};

#endif
