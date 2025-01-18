/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 14:05:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data)
{
	data.setState(InternalCgiRequestData::E_CGI_STATE_EXECUTING);
	data.assignExecutor(worker);
	worker.assignRequestData(data);
	worker.execute();
}

void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
    worker.reset();
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void	CgiModule::mf_returnRequestData(InternalCgiRequestData& data)
{	
	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalCgiRequestData*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}

/*
	Takes all timedout requests, marks them for cleanup (internally force stops the process)
	Returns the shortest time until the next timeout, to be used by epoll_wait
*/
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
			if (curRequest->getState() == InternalCgiRequestData::E_CGI_STATE_EXECUTING)
			{
				mf_markRequestForCleanup(*curRequest);
				curRequest->CallTheUser(E_CGI_ON_ERROR_TIMEOUT);
			}
		}
		else
			break ;
	}
			
	if (m_timerTracker.begin() == m_timerTracker.end())
		return (-1);
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

void	CgiModule::mf_markWorkerForCleanup(InternalCgiWorker& worker)
{
	InternalCgiRequestData*							data;
	
	data = worker.accessRequestData();
	
	if (data->getState() == InternalCgiRequestData::E_CGI_STATE_FINISH)
		return ;

	worker.stop();
	data->setState(InternalCgiRequestData::E_CGI_STATE_FINISH);
	m_availableWorkers.push_front(&worker);
}

void	CgiModule::mf_markRequestForCleanup(InternalCgiRequestData& data)
{
	InternalCgiWorker*								worker;
	
	if (data.getState() == InternalCgiRequestData::E_CGI_STATE_FINISH)
		return ;

	worker = data.accessExecutor();
	worker->stop();
	data.setState(InternalCgiRequestData::E_CGI_STATE_FINISH);
	m_availableWorkers.push_front(worker);
}

void	CgiModule::mf_cleanupFinishedRequests()
{
	InternalCgiRequestData*						requestData;
	InternalCgiWorker*							worker;
	size_t										availWorkers;	

	availWorkers = m_availableWorkers.size();
	for (size_t i = 0; i < availWorkers; i++)
	{
		worker = m_availableWorkers[0];
		requestData = worker->accessRequestData();
		if (!requestData)
			break ;
		m_availableWorkers.pop_front();
		mf_returnWorker(*worker);
		mf_returnRequestData(*requestData);
	}
}

void		CgiModule::mf_recycleFailedStart(InternalCgiWorker& worker, InternalCgiRequestData& data, e_CgiCallback callUser)
{
	InternalCgiRequestData::t_ptr_CgiUser 		user = data.getUser();
	InternalCgiRequestData::t_func_CgiHandler 	handler = data.getHandler(callUser);

	mf_returnWorker(worker);
	mf_returnRequestData(data);
	if (user && handler)
		(handler)(user);
}
