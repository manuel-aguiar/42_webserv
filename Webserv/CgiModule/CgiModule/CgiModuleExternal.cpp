

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"

namespace Cgi
{

	/*
		Provide the caller with one CgiRequestData object if available,
		such tha the user can fill it and enqueue for execution.

		This function is expected to be SAFE to be called from an event handler (Callback).
	*/
	Cgi::Module::Request*	Cgi::Module::acquireRequest()
	{
		InternalRequest*     data;

		if (!m_availableRequestData.size())
			return (NULL);
		data = m_availableRequestData.back();
		m_availableRequestData.pop_back();

		data->setState(RequestState::ACQUIRED);
		
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
	void	Cgi::Module::enqueueRequest(Request& request, bool isCalledFromEventLoop)
	{
		Worker*						worker;	
		InternalRequest*			requestData;
		unsigned int				timeout;

		requestData = static_cast<InternalRequest*>(&request);

		assert(requestData->getState() == RequestState::ACQUIRED);

		timeout = requestData->getTimeoutMs();
		timeout = (timeout > m_maxTimeout) ? m_maxTimeout : timeout;	
		timeout = (timeout < CGI_MIN_TIMEOUT) ? CGI_MIN_TIMEOUT : timeout;
		
		// tell the requestData where its timer is, in case of premature finish/cancelation
		requestData->setMyTimer(m_timerTracker.insert(Timer::now() + timeout, requestData));
		if (m_availableWorkers.size() == 0)
		{
			requestData->setState(RequestState::QUEUED);
			m_executionQueue.push_back(requestData);
			return ;
		}
		worker = m_availableWorkers.back();
		m_availableWorkers.pop_back();
		m_busyWorkerCount++;
		
		mf_execute(*worker, *requestData, isCalledFromEventLoop);
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
	int		Cgi::Module::processRequests()
	{
		return (mf_finishTimedOut());
	}

	void	Cgi::Module::modifyRequest(Request& data, Module::Options::Flags newOptions, bool isCalledFromEventLoop)
	{
		InternalRequest*	requestData;
		RequestStateEnum::Type	state;

		requestData = static_cast<InternalRequest*>(&data);
		state = requestData->getState();
		
		if (state == RequestState::ACQUIRED || state == RequestState::QUEUED)
		{
			data.setRuntimeOptions(newOptions);
			return ;
		}
		if (newOptions & Options::CANCEL_READ)
			requestData->accessExecutor()->disableReadEvent(isCalledFromEventLoop);
		if (newOptions & Options::CANCEL_WRITE)
			requestData->accessExecutor()->disableWriteEvent(isCalledFromEventLoop);
		if (newOptions & Options::HOLD_READ)
			requestData->accessExecutor()->disableReadHandler();
		if (newOptions & Options::HOLD_WRITE)
			requestData->accessExecutor()->disableWriteHandler();
		if (newOptions & Options::RESTART_READ)
			requestData->accessExecutor()->enableReadHandler();
		if (newOptions & Options::RESTART_WRITE)
			requestData->accessExecutor()->enableWriteHandler();
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
	void	Cgi::Module::finishRequest(Request& request, bool isCalledFromEventLoop)
	{
		InternalRequest*					requestData;
		Cgi::Module::RequestState::Type		state;

		requestData = static_cast<InternalRequest*>(&request);
		state = requestData->getState();
		
		switch (state)
		{
			case RequestState::ACQUIRED:
				mf_recycleRequestData(*requestData); break ;
			case RequestState::EXECUTING:
				mf_cancelAndRecycle(*requestData, isCalledFromEventLoop); break ;
			case RequestState::QUEUED:
				requestData->setState(RequestStateEnum::CANCELLED); break ;
			default:
				break ;
		}
	}


	/*
		This function closes all runing processes, resets all requests and clears the execution queue.

		It closes fds, IT IS NOT SAFE TO BE CALLED FROM AN EVENT HANDLER (Callback).
	*/

	void	Cgi::Module::stopAndReset()
	{
		InternalRequest* data;

		for (size_t i = 0; i < m_allWorkers.size(); ++i)
		{
			data = m_allWorkers[i].accessRequestData();
			if (!data)
				continue ;
			mf_cancelAndReturn(*data); break ;
		}
		
		for (size_t i = 0; i < m_executionQueue.size(); ++i)
		{
			data = m_executionQueue[i];
			data->Runtime_CallTheUser(CgiRuntime_Callback::ON_ERROR_RUNTIME);
			mf_returnRequestData(*data);
		}
			
		m_executionQueue.clear();
	}

}; // namespace Cgi