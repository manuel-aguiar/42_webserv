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

typedef enum e_CgiEvents
{
	CGI_ON_READ,
	CGI_ON_WRITE,
	CGI_ON_ERROR,
	CGI_ON_CLOSE,
	CGI_ON_TIMEOUT,
	CGI_EVENT_COUNT
} t_CgiEvents;


typedef enum e_CgiEnv
{
	CGI_AUTH_TYPE,
	CGI_CONTENT_LENGTH,
	CGI_CONTENT_TYPE,
	CGI_GATEWAY_INTERFACE,
	CGI_PATH_INFO,
	CGI_PATH_TRANSLATED,
	CGI_QUERY_STRING,
	CGI_REMOTE_ADDR,
	CGI_REMOTE_HOST,
	CGI_REMOTE_IDENT,
	CGI_REMOTE_USER,
	CGI_REQUEST_METHOD,
	CGI_SCRIPT_NAME,
	CGI_SERVER_NAME,
	CGI_SERVER_PORT,
	CGI_SERVER_PROTOCOL,
	CGI_SERVER_SOFTWARE
}	t_CgiEnv;

#endif