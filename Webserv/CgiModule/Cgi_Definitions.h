/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_Definitions.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:08:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 16:43:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIDEFINITIONS_H

# define CGIDEFINITIONS_H

//Toolkit headers
# include "../../Toolkit/Arrays/DynArray/DynArray.hpp"

//Project Headers
# include "../GenericUtils/Webserver_Definitions.h"

//C++ headers
# include <string>
# include <utility>
# include <map>

typedef std::string						t_extension;
typedef std::string						t_CgiEnvKey;
typedef std::string						t_CgiEnvValue;

typedef enum
{
	E_CGI_ON_EXECUTE,
	E_CGI_ON_ERROR_STARTUP,
	E_CGI_ON_ERROR_RUNTIME,
	E_CGI_CALLBACK_COUNT
} 	e_CgiCallback;


typedef enum
{
	E_CGI_AUTH_TYPE,
	E_CGI_CONTENT_LENGTH,
	E_CGI_CONTENT_TYPE,
	E_CGI_GATEWAY_INTERFACE,
	E_CGI_PATH_INFO,
	E_CGI_PATH_TRANSLATED,
	E_CGI_QUERY_STRING,
	E_CGI_REMOTE_ADDR,
	E_CGI_REMOTE_HOST,
	E_CGI_REMOTE_IDENT,
	E_CGI_REMOTE_USER,
	E_CGI_REQUEST_METHOD,
	E_CGI_SCRIPT_NAME,
	E_CGI_SERVER_NAME,
	E_CGI_SERVER_PORT,
	E_CGI_SERVER_PROTOCOL,
	E_CGI_SERVER_SOFTWARE,
	E_CGI_ENV_COUNT
}	e_CgiEnv;

typedef struct s_CgiRequestEnv
{
	DynArray<std::pair<e_CgiEnv, t_CgiEnvKey> >		envBase;
	std::map<t_CgiEnvKey, t_CgiEnvValue>			envExtra;
}	t_CgiRequestEnv;

#define STR_CGI_AUTH_TYPE "AUTH_TYPE"
#define STR_CGI_CONTENT_LENGTH "CONTENT_LENGTH"
#define STR_CGI_CONTENT_TYPE "CONTENT_TYPE"
#define STR_CGI_GATEWAY_INTERFACE "GATEWAY_INTERFACE"
#define STR_CGI_PATH_INFO "PATH_INFO"
#define STR_CGI_PATH_TRANSLATED "PATH_TRANSLATED"
#define STR_CGI_QUERY_STRING "QUERY_STRING"
#define STR_CGI_REMOTE_ADDR "REMOTE_ADDR"
#define STR_CGI_REMOTE_HOST "REMOTE_HOST"
#define STR_CGI_REMOTE_IDENT "REMOTE_IDENT"
#define STR_CGI_REMOTE_USER "REMOTE_USER"
#define STR_CGI_REQUEST_METHOD "REQUEST_METHOD"
#define STR_CGI_SCRIPT_NAME "SCRIPT_NAME"
#define STR_CGI_SERVER_NAME "SERVER_NAME"
#define STR_CGI_SERVER_PORT "SERVER_PORT"
#define STR_CGI_SERVER_PROTOCOL "SERVER_PROTOCOL"
#define STR_CGI_SERVER_SOFTWARE "SERVER_SOFTWARE"


#endif