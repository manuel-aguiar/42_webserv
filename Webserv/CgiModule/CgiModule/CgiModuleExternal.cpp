/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 11:45:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

CgiRequestData*	CgiModule::acquireRequestData()
{
	InternalCgiRequestData*     data;

	if (!m_availableRequestData.size())
		return (NULL);
	data = m_availableRequestData.back();
	m_availableRequestData.pop_back();

	data->setState(InternalCgiRequestData::E_CGI_STATE_ACQUIRED);
	
	return (data);
}

void	CgiModule::executeRequest(CgiRequestData& request)
{
	InternalCgiRequestData*					requestData;
	InternalCgiWorker* 						worker;
	unsigned int							timeout;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	assert(requestData->getState() == InternalCgiRequestData::E_CGI_STATE_ACQUIRED);

	timeout = request.getTimeoutMs();
	timeout = (timeout > m_maxTimeout) ? m_maxTimeout : timeout;	
	timeout = (timeout < CGI_MIN_TIMEOUT) ? CGI_MIN_TIMEOUT : timeout;
	
	// tell the requestData where its timer is, in case of premature finish/cancelation
	requestData->setMyTimer(m_timerTracker.insert(Timer::now() + timeout, requestData));

	if (m_availableWorkers.size() == 0)
	{
		requestData->setState(InternalCgiRequestData::E_CGI_STATE_QUEUED);
		m_executionQueue.push_back(requestData);
		return ;
	}
	worker = m_availableWorkers.back();
	m_availableWorkers.pop_back();
	m_busyWorkerCount++;
	
	mf_execute(*worker, *requestData);
}
/*
	There are 3 valid scenarios, for 3 request states:

		Request is Acquired: not in any queue (neither available or executing)
			-> can safely reset and put back in the available list right away
			
		Request is Executing: worker is executing the request, not in any queue
			-> forced close the worker
			-> closed worker will return the request to the available list when it can

		Request is Queued: request is in the execution queue
			-> can't remove from the queue, it is in the middle
			-> we mark it as cancelled
			-> as workers try to get requests, they check the cancelled ones and remove them from
				the queue back to the available list
				(++ we don't iterate and move all elements in the middle)
				(-- someone may not be able to subscribe, even though there is a dead request in the queue
				it must wait a worker processes it)

	There are 2 invalid scenarios that are programming errors:

		Request is Idle:
			-> THIS IS A PROGRAMMING ERROR AT THE CGI MODULE LEVEL 	-> ASSERT

		Request is Cancelled:
			-> THIS IS A PROGRAMMING ERROR AT THE USER LEVEL		-> ASSERT
				(not problem with double cancelling, but why do that?)

*/

int		CgiModule::finishTimedOut()
{
	Timer timer = Timer::now();

	TimerTracker<Timer, InternalCgiRequestData*>::iterator it = m_timerTracker.begin();
	TimerTracker<Timer, InternalCgiRequestData*>::iterator next;
	
	while (it != m_timerTracker.end())
	{
		if (it->first < timer && it->second->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE)
		{
			next = ++it;
			--it;
			finishRequest(*it->second);
			it = next;
		}
		else
			break ;
	}

	if (m_timerTracker.begin() == m_timerTracker.end())
	{
		return (-1);
	}
	return ((timer < m_timerTracker.begin()->first) ? 1 : (m_timerTracker.begin()->first - timer).getMilliseconds());
}

void	CgiModule::finishRequest(CgiRequestData& request)
{
	InternalCgiRequestData* requestData;
	InternalCgiWorker*		worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	//two asserts, so we know which one blows up
	assert(requestData->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE);
	assert(requestData->getState() != InternalCgiRequestData::E_CGI_STATE_CANCELLED);

	switch (requestData->getState())
	{
		case InternalCgiRequestData::E_CGI_STATE_ACQUIRED:
		{
			mf_returnRequestData(*requestData);
			break ;
		}
		case InternalCgiRequestData::E_CGI_STATE_EXECUTING:
		{
			worker = requestData->accessExecutor();
			worker->stopExecution();
			break ;
		}
		case InternalCgiRequestData::E_CGI_STATE_QUEUED:
		{
			requestData->setState(InternalCgiRequestData::E_CGI_STATE_CANCELLED);
			break ;
		}
		case InternalCgiRequestData::E_CGI_STATE_IDLE:
		case InternalCgiRequestData::E_CGI_STATE_CANCELLED:
			assert(false);
	}
}

void	CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void	CgiModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}

void	CgiModule::forceStop()
{
	for (HeapArray<InternalCgiWorker>::iterator it = m_allWorkers.begin(); it != m_allWorkers.end(); it++)
	{
		it->stopExecution();
		it->reset();
	}
}
