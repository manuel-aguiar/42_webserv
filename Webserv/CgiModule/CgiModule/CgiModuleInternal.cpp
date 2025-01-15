/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 13:15:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data)
{
	data.setState(InternalCgiRequestData::E_CGI_STATE_EXECUTING);
	data.setExecutor(&worker);
	worker.execute(data);
}

void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
    worker.reset();
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void	CgiModule::mf_returnRequestData(InternalCgiRequestData& data)
{	
	mf_cleanupRequestData(data);	
	m_availableRequestData.push_back(&data);
}

void	CgiModule::mf_cleanupRequestData(InternalCgiRequestData& data)
{
	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalCgiRequestData*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();
}

int	CgiModule::mf_finishTimedOut()
{
	Timer timer = Timer::now();

	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	it = m_timerTracker.begin();
	InternalCgiRequestData* 								curRequest;
	

	for (; it != m_timerTracker.end(); ++it) 
	{
		if (it->first < timer && it->second->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE)
		{
			curRequest = it->second;
			
			// call the user if it is executing, runtime error
			if (curRequest->getState() == InternalCgiRequestData::E_CGI_STATE_EXECUTING)
			{
				mf_stopRequestPrepareCleanup(*curRequest);
				curRequest->CallTheUser(E_CGI_ON_ERROR_TIMEOUT);
			}
		}
		else
			break ;
	}
			
	// return the shortest time to the next timeout
	if (m_timerTracker.begin() == m_timerTracker.end())
	{
		return (-1);	//infinite
	}
	return ((timer < m_timerTracker.begin()->first) ? 1 : (m_timerTracker.begin()->first - timer).getMilliseconds());	
}

void	CgiModule::mf_reloadWorkers()
{
	InternalCgiRequestData* 	curRequest;
	InternalCgiWorker*			worker;

	while (m_availableWorkers.size() > 0)
	{
		worker = m_availableWorkers.front();
		
		while (m_executionQueue.size() > 0)
		{
			curRequest = m_executionQueue.front();
			
			if (curRequest->getState() != InternalCgiRequestData::E_CGI_STATE_CANCELLED)
			{
				m_busyWorkerCount++;
				m_availableWorkers.pop_front();
				m_executionQueue.pop_front();
				mf_execute(*worker, *curRequest);
				break ;
			}
			mf_returnRequestData(*curRequest);
		}
		if (m_executionQueue.size() == 0)
			break ;
	}
}

void	CgiModule::mf_stopRequestPrepareCleanup(InternalCgiRequestData& data)
{
	InternalCgiWorker*							worker;
	
	// already marked for cleanup
	if (data.getState() == InternalCgiRequestData::E_CGI_STATE_FINISH)
		return ;

	worker = data.accessExecutor();
	worker->KillExecution();
	data.setState(InternalCgiRequestData::E_CGI_STATE_FINISH);
	m_availableRequestData.push_front(&data);
}

void	CgiModule::mf_cleanupFinishedRequests()
{
	InternalCgiRequestData::t_CgiRequestState	state;
	InternalCgiRequestData*						requestData;
	InternalCgiWorker*							worker;

	for (size_t i = 0; i < m_availableRequestData.size(); i++)
	{
		requestData = m_availableRequestData[i];
		state = requestData->getState();
		if (state != InternalCgiRequestData::E_CGI_STATE_FINISH)
			break ;
		worker = requestData->accessExecutor();
		mf_returnWorker(*worker);
		mf_cleanupRequestData(*requestData);
	}
}

void		CgiModule::mf_recycleFailedStart(InternalCgiWorker& worker, InternalCgiRequestData& data, e_CgiCallback callUser)
{
	Callback 	callback = data.accessCallback(callUser);

	mf_returnWorker(worker);
	mf_returnRequestData(data);
	if (callback.getHandler() != NULL)
		callback.execute();
}
