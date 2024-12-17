/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiDefaults.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:21:12 by codespace         #+#    #+#             */
/*   Updated: 2024/12/17 10:13:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIDEFAULTS_H

# define CGIDEFAULTS_H

		typedef enum e_CgiEnvasfasffasf
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
        }	e_CgiEnv;


#endif
