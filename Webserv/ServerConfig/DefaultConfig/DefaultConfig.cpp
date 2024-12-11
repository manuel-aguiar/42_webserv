# include "DefaultConfig.hpp"

DefaultConfig::DefaultConfig()
{

	serverRoot = DEFAULTCONF_SERVER_ROOT;
	maxClientBodySize = DEFAULTCONF_HTTP_MAX_CLIENT_BODY_SIZE;
	maxClientHeaderSize = DEFAULTCONF_HTTP_MAX_CLIENT_HEADER_SIZE;
	IPListen = DEFAULTCONF_BLOCK_IP_LISTEN;
	maxConnections = DEFAULTCONF_BLOCK_MAX_CONNECTIONS;
	maxCGI = DEFAULTCONF_BLOCK_MAX_CGI;

	http400Path = DEFAULTCONF_BLOCK_ERROR_400_PAGE;
	http404Path = DEFAULTCONF_BLOCK_ERROR_404_PAGE;

	autoIndex = DEFAULTCONF_LOCATION_AUTOINDEX;
	methods = DEFAULTCONF_LOCATION_METHODS;
	type = DEFAULTCONF_LOCATION_TYPE;
}