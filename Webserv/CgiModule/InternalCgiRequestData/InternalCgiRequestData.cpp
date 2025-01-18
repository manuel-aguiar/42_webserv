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
	m_myTimer(TimerTracker<Timer, InternalCgiRequestData*>::iterator()) {}

CgiModule::InternalCgiRequestData::~InternalCgiRequestData() {}


void	CgiModule::InternalCgiRequestData::reset()
{
	CgiRequestData::mf_reset();
	m_executor = NULL;
	m_myTimer = TimerTracker<Timer, InternalCgiRequestData*>::iterator();
}


// setters
void	CgiModule::InternalCgiRequestData::assignExecutor(CgiModule::InternalCgiWorker& executor)
{
	m_executor = &executor;
}

void	CgiModule::InternalCgiRequestData::setMyTimer(const TimerTracker<Timer, InternalCgiRequestData*>::iterator& timer)
{
	m_myTimer = timer;
}

TimerTracker<Timer, CgiModule::InternalCgiRequestData*>::iterator
CgiModule::InternalCgiRequestData::getMyTimer() const
{
	return (m_myTimer);
}

CgiModule::InternalCgiWorker*	CgiModule::InternalCgiRequestData::accessExecutor()
{
	return (m_executor);
}


void	CgiModule::InternalCgiRequestData::setState(const InternalCgiRequestData::t_CgiRequestState state)
{
	m_state = state;
}

CgiModule::InternalCgiRequestData::t_CgiRequestState	CgiModule::InternalCgiRequestData::getState() const
{
	return (m_state);
}


void	CgiModule::InternalCgiRequestData::CallTheUser(const e_CgiCallback event)
{
	t_func_CgiHandler	 handler = m_stateCallbacks[event];
	if (handler)
		(handler)(m_user);
}

CgiRequestData::t_bytesRead	CgiModule::InternalCgiRequestData::UserRead(t_fd readFd)
{
	if (!(m_readHandler && m_user))
		return (0);
	return ((m_readHandler)(m_user, readFd));
}

CgiRequestData::t_bytesWritten	CgiModule::InternalCgiRequestData::UserWrite(t_fd writeFd)
{
	if (!(m_writeHandler && m_user))
		return (0);
	return ((m_writeHandler)(m_user, writeFd));
}

//private
CgiModule::InternalCgiRequestData::InternalCgiRequestData(const InternalCgiRequestData &copy) :
	CgiRequestData(copy),
	m_executor(copy.m_executor),
	m_myTimer(copy.m_myTimer)  {}

CgiModule::InternalCgiRequestData &CgiModule::InternalCgiRequestData::operator=(const InternalCgiRequestData &assign)
{
	if (this != &assign)
		CgiRequestData::operator=(assign);
	m_executor = assign.m_executor;
	m_myTimer = assign.m_myTimer;
	return (*this);
}