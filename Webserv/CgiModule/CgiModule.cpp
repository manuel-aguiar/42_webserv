/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:04:30 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 10:24:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiModule.hpp"




const char* const CgiModule::m_defaultEnvp[ENV_CGI_VAR_COUNT] = {
	"AUTH_TYPE=",
	"CONTENT_LENGTH=",
	"CONTENT_TYPE=",
	"GATEWAY_INTERFACE=",
	"PATH_INFO=",
	"PATH_TRANSLATED=",
	"QUERY_STRING=",
	"REMOTE_ADDR=",
	"REMOTE_HOST=",
	"REMOTE_IDENT=",
	"REMOTE_USER=",
	"REQUEST_METHOD=",
	"SCRIPT_NAME=",
	"SERVER_NAME=",
	"SERVER_PORT=",
	"SERVER_PROTOCOL=",
	"SERVER_SOFTWARE="
};