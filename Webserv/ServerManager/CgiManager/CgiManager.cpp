/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/15 08:20:36 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiManager.hpp"
#include "CgiLiveRequest/CgiLiveRequest.hpp"
#include "CgiPendingRequest/CgiPendingRequest.hpp"

CgiManager::CgiManager(const size_t maxConcurrent) :
	m_maxConcurrent(maxConcurrent),
	m_currentConcurrent(0),
	m_LiveRequests(maxConcurrent),
	m_readEvents(maxConcurrent)

{

}

const char* const* CgiManager::getDefaultEnvp() const
{
	return (m_defaultEnvp);
}

const char* const CgiManager::m_defaultEnvp[ENV_CGI_VAR_COUNT] = {
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

CgiManager::~CgiManager()
{

}
