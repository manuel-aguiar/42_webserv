/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 14:06:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

/*
	Provide the caller with one CgiRequestData object if available,
	such tha the user can fill it and enqueue for execution.

	This function is expected to be SAFE to be called from an event handler (Callback).
*/
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

/*
	Places a request previouesly "acquired" in the execution queue.
	This doesn't lead to a straight execution even if the queue is empty.
	For security reasons and to avoid errors when calling enqueue from an event handler,
	the user MUST always call processRequests() to get the queue going.

	Because it doesn't execute anything or create/destroy fds, it is SAFE to call it
	from an event handler (Callback).
*/
void	CgiModule::enqueueRequest(CgiRequestData& request)
{
	InternalCgiRequestData*					requestData;
	unsigned int							timeout;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	assert(requestData->getState() == InternalCgiRequestData::E_CGI_STATE_ACQUIRED);

	timeout = requestData->getTimeoutMs();
	timeout = (timeout > m_maxTimeout) ? m_maxTimeout : timeout;	
	timeout = (timeout < CGI_MIN_TIMEOUT) ? CGI_MIN_TIMEOUT : timeout;
	
	// tell the requestData where its timer is, in case of premature finish/cancelation
	requestData->setMyTimer(m_timerTracker.insert(Timer::now() + timeout, requestData));
	requestData->setState(InternalCgiRequestData::E_CGI_STATE_QUEUED);
	m_executionQueue.push_back(requestData);
}

/*
	Effectively starts execution of requests in the queue.
	Returns the shortest time until the next timeout, may be used by epoll_wait

	Internally, force closes timed out requests, cleans up finished requests,
	since the previous call to processRequests and reloads workers with requests from the queue.

	To avoid stale events to get mixed up, a worker does not auto reload if there are requests in the queue
	without calling this function.

	The reason being that a request may take several epoll_wait cycles to finish,
	we don't want to mix fds in the same epoll_wait cycle.

	Having said that, because this function closes and opens fds, 
	
	IT IS NOT A SAFE FUNCTION TO BE CALLED FROM AN EVENT HANDLER (Callback).
*/
int		CgiModule::processRequests()
{
	int nextWait;

	nextWait = mf_finishTimedOut();
	mf_cleanupFinishedRequests();
	mf_reloadWorkers();

	return (nextWait);
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
		

		For the remining, just ignore:
			finished (already marked for cleanup)
			idle (wtv, let it be, a user may call on an already recycled request)
			cancelled (effective cleanup already, waiting for a worker to remove it from the executionQueue)

	This function does not close any fds, so it is SAFE to be called from an event handler (Callback).
*/
void	CgiModule::finishRequest(CgiRequestData& request)
{
	InternalCgiRequestData*						requestData;
	InternalCgiRequestData::t_CgiRequestState	state;

	requestData = static_cast<InternalCgiRequestData*>(&request);
	state = requestData->getState();
	
	switch (state)
	{
		case InternalCgiRequestData::E_CGI_STATE_ACQUIRED:
			mf_returnRequestData(*requestData); break ;
		case InternalCgiRequestData::E_CGI_STATE_EXECUTING:
			mf_markRequestForCleanup(*requestData); break ;
		case InternalCgiRequestData::E_CGI_STATE_QUEUED:
			requestData->setState(InternalCgiRequestData::E_CGI_STATE_CANCELLED); break ;
		default:
			break ;
	}
}


/*
	This function closes all runing processes, resets all requests and clears the execution queue.

	It closes fds, IT IS NOT SAFE TO BE CALLED FROM AN EVENT HANDLER (Callback).
*/

void	CgiModule::stopAndReset()
{
	InternalCgiRequestData* data;

	for (size_t i = 0; i < m_allWorkers.size(); ++i)
	{
		data = m_allWorkers[i].accessRequestData();
		if (!data || data->getState() == InternalCgiRequestData::E_CGI_STATE_FINISH)	
			continue ;
		mf_markWorkerForCleanup(m_allWorkers[i]);
		data->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
	}
	
	mf_cleanupFinishedRequests();

	for (size_t i = 0; i < m_executionQueue.size(); ++i)
		m_executionQueue[i]->reset();
	m_executionQueue.clear();
}

