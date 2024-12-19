/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleManagedRequestData.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 09:13:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 09:35:04 by mmaria-d         ###   ########.fr       */
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

// setters
void	CgiModule::ManagedRequestData::setExecutor(CgiModule::CgiLiveRequest* const executor)
{
	m_executor = executor;
}

void	CgiModule::ManagedRequestData::setDataLocation(const List<ManagedRequestData>::iterator& location)
{
	m_dataLocation = location;
}

void	CgiModule::ManagedRequestData::setPendingLocation(const List<ManagedRequestData*>::iterator& location)
{
	m_pendingLocation = location;
}


CgiModule::CgiLiveRequest*	CgiModule::ManagedRequestData::accessExecutor()
{
	return (m_executor);
}

List<CgiModule::ManagedRequestData>::iterator&	CgiModule::ManagedRequestData::accessDataLocation()
{
	return (m_dataLocation);
}

List<CgiModule::ManagedRequestData*>::iterator&	CgiModule::ManagedRequestData::accessPendingLocation()
{
	return (m_pendingLocation);
}