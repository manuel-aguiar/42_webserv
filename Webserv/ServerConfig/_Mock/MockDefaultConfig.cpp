

# include "../DefaultConfig/DefaultConfig.hpp"

DefaultConfig::DefaultConfig()
{
	http_maxClientBodySize = DEFAULTCONF_HTTP_MAX_CLIENT_BODY_SIZE;
	http_maxClientHeaderSize = DEFAULTCONF_HTTP_MAX_CLIENT_HEADER_SIZE;
	server_IPListen = DEFAULTCONF_BLOCK_IP_LISTEN;
	server_maxConnections = DEFAULTCONF_BLOCK_MAX_CONNECTIONS;
	server_cgiWorkers = DEFAULTCONF_BLOCK_MAX_CGI;

	http_error400Path = DEFAULTCONF_BLOCK_ERROR_400_PAGE;
	http_error404Path = DEFAULTCONF_BLOCK_ERROR_404_PAGE;

	loc_autoIndex = DEFAULTCONF_LOCATION_AUTOINDEX;
	loc_loc_http_methods = DEFAULTCONF_LOCATION_METHODS;
	loc_type = DEFAULTCONF_LOCATION_TYPE;
}