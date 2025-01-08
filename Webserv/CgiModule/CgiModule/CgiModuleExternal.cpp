/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 15:36:36 by mmaria-d         ###   ########.fr       */
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

	data->setState(InternalCgiRequestData::E_CGI_STATE_POPULATING);
	
	return (data);
}

void	CgiModule::executeRequest(CgiRequestData& request)
{
	InternalCgiRequestData*					requestData;
	InternalCgiWorker* 						worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	assert(requestData->getState() == InternalCgiRequestData::E_CGI_STATE_POPULATING);

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

		Request is Populating: not in any queue (neither available or executing)
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
void	CgiModule::cancelRequest(CgiRequestData& request)
{
	InternalCgiRequestData* requestData;
	InternalCgiWorker*		worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	//two asserts, so we know which one blows up
	assert(requestData->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE);
	assert(requestData->getState() != InternalCgiRequestData::E_CGI_STATE_CANCELLED);

	switch (requestData->getState())
	{
		case InternalCgiRequestData::E_CGI_STATE_POPULATING:
		{
			mf_returnRequestData(*requestData);
			break ;
		}
		case InternalCgiRequestData::E_CGI_STATE_EXECUTING:
		{
			worker = requestData->accessExecutor();
			worker->forcedClose();
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
		it->forcedClose();
		it->reset();
	}
}

void	CgiModule::finishRequest(CgiRequestData& request)
{
	InternalCgiRequestData* requestData;
	InternalCgiWorker*		worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);
	worker = requestData->accessExecutor();
	if (worker)
		worker->cleanClose();
}