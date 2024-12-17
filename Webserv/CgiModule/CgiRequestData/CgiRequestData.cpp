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

CgiRequestData::CgiRequestData(const CgiRequestData &copy)
{
	*this = copy;
}

CgiRequestData &CgiRequestData::operator=(const CgiRequestData &copy)
{
	if (this == &copy)
		return (*this);
	m_msgBody = copy.m_msgBody;
	m_envBase = copy.m_envBase;
	m_envExtra = copy.m_envExtra;
	m_extension = copy.m_extension;
	m_scriptPath = copy.m_scriptPath;
	return (*this);
}


