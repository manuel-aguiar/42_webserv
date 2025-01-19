/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 14:20:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_execute(Worker& worker, InternalRequest& data, bool markFdsAsStale)
{
	data.setState(STATE_EXECUTING);
	data.assignExecutor(worker);
	worker.assignRequestData(data);
	worker.execute(markFdsAsStale);
}


/*
	Takes all timedout requests, marks them for cleanup (internally force stops the process)
	Returns the shortest time until the next timeout, to be used by epoll_wait
*/
int	CgiModule::mf_finishTimedOut()
{
	Timer timer = Timer::now();

	TimerTracker<Timer, InternalRequest*>::iterator 	it = m_timerTracker.begin();
	InternalRequest* 								curRequest;
	

	for (; it != m_timerTracker.end(); ++it) 
	{
		if (it->first < timer && it->second->getState() != STATE_IDLE)
		{
			curRequest = it->second;
			switch (curRequest->getState())
			{
				case STATE_ACQUIRED:
					mf_recycleRequestData(*curRequest); break ;
				case STATE_EXECUTING:
					mf_recycleTimeoutFailure(*curRequest->accessExecutor()); break;
				case STATE_QUEUED:
					curRequest->setState(STATE_CANCELLED); break;
				default: break ;
			}		
		}
		else
			break ;
	}
			
	if (m_timerTracker.begin() == m_timerTracker.end())
		return (-1);
	return ((timer < m_timerTracker.begin()->first) ? 1 : (m_timerTracker.begin()->first - timer).getMilliseconds());	
}
