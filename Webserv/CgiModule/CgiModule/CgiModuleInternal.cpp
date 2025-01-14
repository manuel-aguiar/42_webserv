/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 16:43:45 by mmaria-d         ###   ########.fr       */
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
	if (data.getMyTimer() != TimerTracker<Timer, InternalCgiRequestData*>::iterator())
		m_timerTracker.erase(data.getMyTimer());

	data.reset();	
	m_availableRequestData.push_back(&data);
}

int	CgiModule::mf_finishTimedOut()
{
	Timer timer = Timer::now();

	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	it = m_timerTracker.begin();
	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	next;
	InternalCgiRequestData* 								curRequest;
	

	while (it != m_timerTracker.end())
	{
		if (it->first < timer && it->second->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE)
		{
			next = ++it;
			--it;
			
			curRequest = it->second;
			
			// call the user if it is executing, runtime error
			if (curRequest->getState() == InternalCgiRequestData::E_CGI_STATE_EXECUTING)
				curRequest->CallTheUser(E_CGI_ON_ERROR_TIMEOUT);
			
			// if user doesn't cancel, we do it for them
			finishRequest(*it->second);
			
			//potential iterator invalidation, we only care about the ones that are timed out now
			// if inserted already timedout, it will be removed in the next iteration
			it = next;
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