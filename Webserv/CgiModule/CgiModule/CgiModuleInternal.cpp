

# include "CgiModule.hpp"
# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

void	ImplModule::mf_execute(Worker& worker, InternalRequest& data, bool markFdsAsStale)
{
	data.setState(RequestState::EXECUTING);
	data.assignExecutor(worker);
	worker.assignRequestData(data);
	worker.execute(markFdsAsStale);
}


/*
	Takes all timedout requests, marks them for cleanup (internally force stops the process)
	Returns the shortest time until the next timeout, to be used by epoll_wait
*/
int	ImplModule::mf_finishTimedOut()
{
	Timer timer = Timer::now();

	TimerTracker<Timer, InternalRequest*>::iterator 	it = m_timerTracker.begin();
	InternalRequest* 								curRequest;
	
	for (; it != m_timerTracker.end(); ++it) 
	{
		if (it->first < timer && it->second->getState() != RequestState::IDLE)
		{
			curRequest = it->second;
			switch (curRequest->getState())
			{
				case RequestState::ACQUIRED:
					mf_recycleRequestData(*curRequest); break ;
				case RequestState::EXECUTING:
					mf_recycleTimeoutFailure(*curRequest->accessExecutor()); break;
				case RequestState::QUEUED:
					curRequest->setState(RequestState::CANCELLED); break;
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