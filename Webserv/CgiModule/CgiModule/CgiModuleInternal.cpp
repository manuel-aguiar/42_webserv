/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 17:48:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data, bool markFdsAsStale)
{
	data.setState(InternalCgiRequestData::E_CGI_STATE_EXECUTING);
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

	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	it = m_timerTracker.begin();
	InternalCgiRequestData* 								curRequest;
	

	for (; it != m_timerTracker.end(); ++it) 
	{
		if (it->first < timer && it->second->getState() != InternalCgiRequestData::E_CGI_STATE_IDLE)
		{
			curRequest = it->second;
			switch (curRequest->getState())
			{
				case InternalCgiRequestData::E_CGI_STATE_ACQUIRED:
					mf_recycleRequestData(*curRequest); break ;
				case InternalCgiRequestData::E_CGI_STATE_EXECUTING:
					mf_recycleTimeoutFailure(*curRequest->accessExecutor()); break;
				case InternalCgiRequestData::E_CGI_STATE_QUEUED:
					curRequest->setState(InternalCgiRequestData::E_CGI_STATE_CANCELLED); break;
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
