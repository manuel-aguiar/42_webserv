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
	m_executor(NULL),
	m_state(E_CGI_STATE_IDLE) {}

CgiModule::InternalCgiRequestData::~InternalCgiRequestData() {}


void	CgiModule::InternalCgiRequestData::reset()
{
	CgiRequestData::reset();
	m_executor = NULL;
	m_state = E_CGI_STATE_IDLE;
}


// setters
void	CgiModule::InternalCgiRequestData::setExecutor(CgiModule::InternalCgiWorker* const executor)
{
	m_executor = executor;
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

CgiModule::InternalCgiRequestData::InternalCgiRequestData(const InternalCgiRequestData &copy) :
	CgiRequestData(copy),
	m_executor(copy.m_executor),
	m_state(copy.m_state) {}

CgiModule::InternalCgiRequestData &CgiModule::InternalCgiRequestData::operator=(const InternalCgiRequestData &assign)
{
	if (this != &assign)
		CgiRequestData::operator=(assign);
	m_executor = assign.m_executor;
	m_state = assign.m_state;
	return (*this);
}