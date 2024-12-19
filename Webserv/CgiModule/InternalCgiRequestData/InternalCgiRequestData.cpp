/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternalCgiRequestData.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 09:13:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 09:35:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "InternalCgiRequestData.hpp"

CgiModule::InternalCgiRequestData::InternalCgiRequestData() :
	CgiRequestData(),
	m_executor(NULL)
{
}

CgiModule::InternalCgiRequestData::~InternalCgiRequestData()
{
}

CgiModule::InternalCgiRequestData::InternalCgiRequestData(const InternalCgiRequestData &copy) :
	CgiRequestData(copy),
	m_executor(copy.m_executor)
{
}

CgiModule::InternalCgiRequestData &CgiModule::InternalCgiRequestData::operator=(const InternalCgiRequestData &assign)
{
	if (this != &assign)
		CgiRequestData::operator=(assign);
	m_executor = assign.m_executor;
	return (*this);
}

void	CgiModule::InternalCgiRequestData::reset()
{
	CgiRequestData::reset();
	m_executor = NULL;
	m_pendingLocation = NULL;
}


// setters
void	CgiModule::InternalCgiRequestData::setExecutor(CgiModule::InternalCgiWorker* const executor)
{
	m_executor = executor;
}

void	CgiModule::InternalCgiRequestData::setPendingLocation(const List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator& location)
{
	m_pendingLocation = location;
}


CgiModule::InternalCgiWorker*	CgiModule::InternalCgiRequestData::accessExecutor()
{
	return (m_executor);
}

List<CgiModule::InternalCgiRequestData *, MPool_FixedElem<CgiModule::InternalCgiRequestData *> >::iterator&	
CgiModule::InternalCgiRequestData::accessPendingLocation()
{
	return (m_pendingLocation);
}