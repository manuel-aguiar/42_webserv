/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestData.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:36:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:36:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequestData.hpp"

CgiRequestData::CgiRequestData() :
	m_eventManager(NULL),
	m_msgBody(""),
	m_extension(""),
	m_scriptPath("")
{
	m_env.envBase.reserve(E_CGI_ENV_COUNT);
}

CgiRequestData::~CgiRequestData()
{
}

CgiRequestData::CgiRequestData(const CgiRequestData &copy) :
	m_eventManager(copy.m_eventManager),
	m_msgBody(copy.m_msgBody),
	m_extension(copy.m_extension),
	m_scriptPath(copy.m_scriptPath)
{
	m_env.envBase = copy.m_env.envBase;
	m_env.envExtra = copy.m_env.envExtra;
}

CgiRequestData &CgiRequestData::operator=(const CgiRequestData &assign)
{
	if (this == &assign)
		return (*this);
	m_msgBody = assign.m_msgBody;
	m_extension = assign.m_extension;
	m_scriptPath = assign.m_scriptPath;
	m_env.envBase = assign.m_env.envBase;
	m_env.envExtra = assign.m_env.envExtra;
	return (*this);
}


