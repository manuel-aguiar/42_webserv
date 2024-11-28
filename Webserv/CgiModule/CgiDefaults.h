/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiDefaults.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:21:12 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 18:44:05 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_DEFAULTS_H

# define CGI_DEFAULTS_H

		typedef enum e_CgiEnv
		{
			ENV_CGI_AUTH_TYPE,
			ENV_CGI_CONTENT_LENGTH,
			ENV_CGI_CONTENT_TYPE,
			ENV_CGI_GATEWAY_INTERFACE,
			ENV_CGI_PATH_INFO,
			ENV_CGI_PATH_TRANSLATED,
			ENV_CGI_QUERY_STRING,
			ENV_CGI_REMOTE_ADDR,
			ENV_CGI_REMOTE_HOST,
			ENV_CGI_REMOTE_IDENT,
			ENV_CGI_REMOTE_USER,
			ENV_CGI_REQUEST_METHOD,
			ENV_CGI_SCRIPT_NAME,
			ENV_CGI_SERVER_NAME,
			ENV_CGI_SERVER_PORT,
			ENV_CGI_SERVER_PROTOCOL,
			ENV_CGI_SERVER_SOFTWARE,
			ENV_CGI_VAR_COUNT
        }	t_CgiEnv;

/*
	GET /index.php?name=John HTTP/1.1
	Host: example.com
	User-Agent: Mozilla/5.0
	Content-Length: 123
	Content-Type: application/json
	X-Custom-Header: MyCustomValue

	CGI environment variables passed:

	REQUEST_METHOD = GET
	QUERY_STRING = name=John
	HTTP_HOST = example.com
	HTTP_USER_AGENT = Mozilla/5.0
	CONTENT_LENGTH = 123
	CONTENT_TYPE = application/json
	HTTP_X_CUSTOM_HEADER = MyCustomValue


*/

#endif
