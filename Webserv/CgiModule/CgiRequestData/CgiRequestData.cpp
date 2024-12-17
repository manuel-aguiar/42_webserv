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

CgiRequestData::CgiRequestData()
{
}

CgiRequestData::~CgiRequestData()
{
}

CgiRequestData::CgiRequestData(const CgiRequestData &copy) :
	m_msgBody(copy.m_msgBody),
	m_envBase(copy.m_envBase),
	m_envExtra(copy.m_envExtra),
	m_extension(copy.m_extension),
	m_scriptPath(copy.m_scriptPath)
{
}

CgiRequestData &CgiRequestData::operator=(const CgiRequestData &assign)
{
	if (this == &assign)
		return (*this);
	m_msgBody = assign.m_msgBody;
	m_envBase = assign.m_envBase;
	m_envExtra = assign.m_envExtra;
	m_extension = assign.m_extension;
	m_scriptPath = assign.m_scriptPath;
	return (*this);
}


