/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:44:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:41:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
#include "CgiLiveRequest.hpp"
#include "../../Globals/Globals.hpp"

//#include "../python-cgi/pythonCgi.hpp"

// C Headers
# include <unistd.h>

CgiLiveRequest::CgiLiveRequest(CgiModule& manager, Globals& globals) :
	m_eventManager(NULL),
	m_pendingRequest(NULL),
	m_connection(NULL),
	m_pid(-1),
	m_manager(manager),
	m_globals(globals)
{
	m_ParentToChild[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
	m_ChildToParent[1] = -1;
}

CgiLiveRequest::~CgiLiveRequest()
{
}

void    CgiLiveRequest::reset()
{
	m_readEvent.reset();
	m_writeEvent.reset();
	m_pendingRequest = NULL;
	m_eventManager = NULL;

	closeFd(m_ParentToChild[0]);
	closeFd(m_ParentToChild[1]);
	closeFd(m_ChildToParent[0]);
	closeFd(m_ChildToParent[1]);
}

CgiLiveRequest::CgiLiveRequest(const CgiLiveRequest &other) :
	m_eventManager(other.m_eventManager),
	m_pendingRequest(other.m_pendingRequest),
	m_connection(other.m_connection),
	m_pid(other.m_pid),
	m_manager(other.m_manager),
	m_globals(other.m_globals)
{
    *this = other;
}

CgiLiveRequest &CgiLiveRequest::operator=(const CgiLiveRequest &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
