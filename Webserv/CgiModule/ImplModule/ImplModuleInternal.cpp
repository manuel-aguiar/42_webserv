

# include "ImplModule.hpp"
# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

void	ImplModule::mf_execute(Worker& worker, InternalReq& data, bool markFdsAsStale)
{
	data.setState(Cgi::RequestState::EXECUTING);
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

	TimerTracker<Timer, InternalReq*>::iterator 	it = m_timerTracker.begin();
	InternalReq* 								curRequest;
	
	for (; it != m_timerTracker.end(); ++it) 
	{
		if (it->first < timer && it->second->getState() != Cgi::RequestState::IDLE)
		{
			curRequest = it->second;
			switch (curRequest->getState())
			{
				case Cgi::RequestState::ACQUIRED:
					mf_recycleRequestData(*curRequest); break ;
				case Cgi::RequestState::EXECUTING:
				{
					ASSERT_EQUAL(curRequest->accessExecutor() != NULL, true, "ImplModule::finishTimedOut(), request is executing but has no executor");
					ASSERT_EQUAL(curRequest->accessExecutor()->accessRequestData() != NULL, true, "ImplModule::finishTimedOut(), request is executing but executor has no request");
					ASSERT_EQUAL(curRequest->accessExecutor()->accessRequestData() == curRequest, true, "ImplModule::finishTimedOut(), request is executing but executor has different request");
					
					mf_recycleTimeoutFailure(*curRequest->accessExecutor()); break;
				}
				case Cgi::RequestState::QUEUED:
					curRequest->setState(Cgi::RequestState::CANCELLED); break;
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
