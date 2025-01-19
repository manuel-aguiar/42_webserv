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

namespace Cgi
{
	typedef enum
	{
		ENV_AUTH_TYPE,
		ENV_CONTENT_LENGTH,
		ENV_CONTENT_TYPE,
		ENV_GATEWAY_INTERFACE,
		ENV_PATH_INFO,
		ENV_PATH_TRANSLATED,
		ENV_QUERY_STRING,
		ENV_REMOTE_ADDR,
		ENV_REMOTE_HOST,
		ENV_REMOTE_IDENT,
		ENV_REMOTE_USER,
		ENV_REQUEST_METHOD,
		ENV_SCRIPT_NAME,
		ENV_SERVER_NAME,
		ENV_SERVER_PORT,
		ENV_SERVER_PROTOCOL,
		ENV_SERVER_SOFTWARE,
		ENV_COUNT
	}	EnvType;


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

};



#endif