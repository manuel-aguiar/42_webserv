
#ifndef WEBSERVDEFAULTCONFIG_H

# define WEBSERVDEFAULTCONFIG_H

// Program
# define DEFAULTCONF_PROGRAM_CGI_MAXBACKLOG "100"
# define DEFAULTCONF_PROGRAM_MAX_CONNECTIONS "10"
# define DEFAULTCONF_PROGRAM_MAX_CGI "10"

// Server
# define DEFAULTCONF_BLOCK_IP_LISTEN "0.0.0.0"
# define DEFAULTCONF_BLOCK_ROOT "./"
# define DEFAULTCONF_BLOCK_HTTP_MAX_CLIENT_BODY_SIZE "1000000"
# define DEFAULTCONF_BLOCK_HTTP_MAX_CLIENT_HEADER_SIZE "1000000"

# define DEFAULTCONF_HTTP_TIMEOUT_MS_FULL_HEADER "60000"
# define DEFAULTCONF_HTTP_TIMEOUT_MS_INTER_SEND "5000"
# define DEFAULTCONF_HTTP_TIMEOUT_MS_INTER_RECEIVE "5000"

  // add the remaining error pages
# define DEFAULTCONF_BLOCK_ERROR_400_PAGE "../StaticAssets/ErrorPages/error_400.html"
# define DEFAULTCONF_BLOCK_ERROR_404_PAGE "../StaticAssets/ErrorPages/error_404.html"

// Location
# define DEFAULTCONF_LOCATION_AUTOINDEX "0"
# define DEFAULTCONF_LOCATION_METHODS "get post delete" // Methods separated by spaces, lowercase 
# define DEFAULTCONF_LOCATION_TYPE "regular"


#endif
