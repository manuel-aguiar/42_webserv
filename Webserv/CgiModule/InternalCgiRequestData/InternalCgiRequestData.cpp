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
	m_queuePosition = NULL;
}


// setters
void	CgiModule::InternalCgiRequestData::setExecutor(CgiModule::InternalCgiWorker* const executor)
{
	m_executor = executor;
}

void	CgiModule::InternalCgiRequestData::setQueuePosition(const List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator& position)
{
	m_queuePosition = position;
}

void	CgiModule::InternalCgiRequestData::setReadFd(const t_fd fd)
{
	m_readFd = fd;
}

void	CgiModule::InternalCgiRequestData::setWriteFd(const t_fd fd)
{
	m_writeFd = fd;
}


CgiModule::InternalCgiWorker*	CgiModule::InternalCgiRequestData::accessExecutor()
{
	return (m_executor);
}

List<CgiModule::InternalCgiRequestData *, MPool_FixedElem<CgiModule::InternalCgiRequestData *> >::iterator&	
CgiModule::InternalCgiRequestData::accessQueuePosition()
{
	return (m_queuePosition);
}