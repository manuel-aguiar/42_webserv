# include "DefaultConfig.hpp"

DefaultConfig::DefaultConfig() :
	server_maxConnections		(DEFAULTCONF_PROGRAM_MAX_CONNECTIONS),
	server_cgiWorkers			(DEFAULTCONF_PROGRAM_MAX_CGI),
	server_cgiBacklog			(DEFAULTCONF_PROGRAM_CGI_MAXBACKLOG),
	server_Workers				(DEFAULTCONF_PROGRAM_MAX_WORKERS),
	server_Root					(DEFAULTCONF_BLOCK_ROOT),
	server_IPListen				(DEFAULTCONF_BLOCK_IP_LISTEN),
	http_maxClientBodySize		(DEFAULTCONF_BLOCK_HTTP_MAX_CLIENT_BODY_SIZE),
	http_maxClientHeaderSize	(DEFAULTCONF_BLOCK_HTTP_MAX_CLIENT_HEADER_SIZE),
	http_timeoutFullHeader		(DEFAULTCONF_HTTP_TIMEOUT_MS_FULL_HEADER),
	http_timeoutInterSend		(DEFAULTCONF_HTTP_TIMEOUT_MS_INTER_SEND),
	http_timeoutInterReceive	(DEFAULTCONF_HTTP_TIMEOUT_MS_INTER_RECEIVE),
	http_timeoutKeepAlive		(DEFAULTCONF_HTTP_TIMEOUT_MS_KEEP_ALIVE),
	http_error400Path			(DEFAULTCONF_BLOCK_ERROR_400_PAGE),
	http_error404Path			(DEFAULTCONF_BLOCK_ERROR_404_PAGE),
	loc_autoIndex				(DEFAULTCONF_LOCATION_AUTOINDEX),
	loc_http_methods			(DEFAULTCONF_LOCATION_METHODS),
	loc_type					(DEFAULTCONF_LOCATION_TYPE)
{

}
