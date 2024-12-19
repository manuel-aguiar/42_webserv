/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleManagedRequestData.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 09:13:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 09:01:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiModule.hpp"

CgiModule::ManagedRequestData::ManagedRequestData() :
	CgiRequestData(),
	m_executor(NULL)
{
}

CgiModule::ManagedRequestData::~ManagedRequestData()
{
}

CgiModule::ManagedRequestData::ManagedRequestData(const ManagedRequestData &copy) :
	CgiRequestData(copy),
	m_executor(copy.m_executor)
{
}

CgiModule::ManagedRequestData &CgiModule::ManagedRequestData::operator=(const ManagedRequestData &assign)
{
	if (this != &assign)
		CgiRequestData::operator=(assign);
	m_executor = assign.m_executor;
	return (*this);
}


void	CgiModule::ManagedRequestData::setExecutor(CgiModule::CgiLiveRequest* executor)
{
	m_executor = executor;
}

CgiModule::CgiLiveRequest*	CgiModule::ManagedRequestData::accessExecutor()
{
	return (m_executor);
}