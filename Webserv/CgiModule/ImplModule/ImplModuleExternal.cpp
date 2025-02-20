

# include "ImplModule.hpp"
# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

// Toolkit headers
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

/*
	Provide the caller with one CgiRequestData object if available,
	such tha the user can fill it and enqueue for execution.

	This function is expected to be SAFE to be called from an event handler (Subscription).
*/
Cgi::Request*	ImplModule::acquireRequest()
{
	InternalReq*     data;

	if (!m_availableRequestData.size())
		return (NULL);
	data = m_availableRequestData.back();
	m_availableRequestData.pop_back();

	data->setState(Cgi::RequestState::ACQUIRED);
	
	return (data);
}

/*
	Places a request previouesly "acquired" in the execution queue.
	This doesn't lead to a straight execution even if the queue is empty.
	For security reasons and to avoid errors when calling enqueue from an event handler,
	the user MUST always call processRequests() to get the queue going.

	Because it doesn't execute anything or create/destroy fds, it is SAFE to call it
	from an event handler (Subscription).
*/
void	ImplModule::enqueueRequest(Cgi::Request& request, bool isCalledFromEventLoop)
{
	Worker*			worker;	
	InternalReq*	internal;
	unsigned int	timeout;

	internal = static_cast<InternalReq*>(&request);

	ASSERT_EQUAL(internal >= m_allRequestData.getArray() && internal <= m_allRequestData.getArray() + m_allRequestData.size(), true, 
	"ImplModule::enqueueRequest(), request was not supplied by this Module");
	ASSERT_EQUAL(internal->getState(), Cgi::RequestState::ACQUIRED, "ImplModule::enqueueRequest(), request state must be ACQUIRED");

	timeout = internal->getTimeoutMs();
	timeout = (timeout > m_maxTimeout) ? m_maxTimeout : timeout;	
	timeout = (timeout < (unsigned int)ImplModule::MIN_TIMEOUT) ? (unsigned int)ImplModule::MIN_TIMEOUT : timeout;
	
	// tell the requestData where its timer is, in case of premature finish/cancelation
	internal->setMyTimer(m_timerTracker.insert(Timer::now() + timeout, internal));
	if (m_availableWorkers.size() == 0)
	{
		internal->setState(Cgi::RequestState::QUEUED);
		m_executionQueue.push_back(internal);
		return ;
	}
	worker = m_availableWorkers.back();
	m_availableWorkers.pop_back();
	m_busyWorkerCount++;
	
	mf_execute(*worker, *internal, isCalledFromEventLoop);
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
	
	IT IS NOT A SAFE FUNCTION TO BE CALLED FROM AN EVENT HANDLER (Subscription).
*/
int		ImplModule::processRequests()
{
	return (mf_finishTimedOut());
}

void	ImplModule::modifyRequest(Cgi::Request& request, bool isCalledFromEventLoop, Cgi::Options::Mask newOptions)
{
	InternalReq*	internal;
	Cgi::RequestState::Type	state;

	internal = static_cast<InternalReq*>(&request);

	ASSERT_EQUAL(internal >= m_allRequestData.getArray() && internal <= m_allRequestData.getArray() + m_allRequestData.size(), true, 
	"ImplModule::modifyRequest(), request was not supplied by this Module");

	state = internal->getState();
	
	if (state == Cgi::RequestState::ACQUIRED || state == Cgi::RequestState::QUEUED)
	{
		request.setRuntimeOptions(newOptions);
		return ;
	}
	if (newOptions & Cgi::Options::CANCEL_READ)
		internal->accessExecutor()->disableReadEvent(isCalledFromEventLoop);
	if (newOptions & Cgi::Options::CANCEL_WRITE)
		internal->accessExecutor()->disableWriteEvent(isCalledFromEventLoop);
	if (newOptions & Cgi::Options::HOLD_READ)
		internal->accessExecutor()->disableReadHandler();
	if (newOptions & Cgi::Options::HOLD_WRITE)
		internal->accessExecutor()->disableWriteHandler();
	if (newOptions & Cgi::Options::RESTART_READ)
		internal->accessExecutor()->enableReadHandler();
	if (newOptions & Cgi::Options::RESTART_WRITE)
		internal->accessExecutor()->enableWriteHandler();
}

/*
	There are 3 valid scenarios, for 3 request states:

		Request is Acquired: not in any queue (neither available or executing)
			-> can safely reset and put back in the available list right away
			
		Request is Executing: worker is executing the request, not in any queue
			-> forced close the worker
			-> closed worker will return the request to the available list when it can

		Request is Queued: request is in the execution queue
			-> can't stopMonitoring from the queue, it is in the middle
			-> we mark it as cancelled
			-> as workers try to get requests, they check the cancelled ones and stopMonitoring them from
				the queue back to the available list
				(++ we don't iterate and move all elements in the middle)
				(-- someone may not be able to subscribe, even though there is a dead request in the queue
				it must wait a worker processes it)
		

		For the remining, just ignore:
			finished (already marked for cleanup)
			idle (wtv, let it be, a user may call on an already recycled request)
			cancelled (effective cleanup already, waiting for a worker to stopMonitoring it from the executionQueue)

	This function does not close any fds, so it is SAFE to be called from an event handler (Subscription).
*/
void	ImplModule::finishRequest(Cgi::Request& request, bool isCalledFromEventLoop)
{
	InternalReq*				internal;
	Cgi::RequestState::Type		state;

	internal = static_cast<InternalReq*>(&request);

	ASSERT_EQUAL(internal >= m_allRequestData.getArray() && internal <= m_allRequestData.getArray() + m_allRequestData.size(), true, 
	"ImplModule::enqueueRequest(), request was not supplied by this Module");

	state = internal->getState();
	
	switch (state)
	{
		case Cgi::RequestState::ACQUIRED:
			mf_recycleRequestData(*internal); break ;
		case Cgi::RequestState::EXECUTING:
			mf_cancelAndRecycle(*internal, isCalledFromEventLoop); break ;
		case Cgi::RequestState::QUEUED:
			internal->setState(Cgi::RequestState::CANCELLED); break ;
		default:
			break ;
	}
}


/*
	This function closes all runing processes, resets all requests and clears the execution queue.

	It closes fds, IT IS NOT SAFE TO BE CALLED FROM AN EVENT HANDLER (Subscription).
*/

void	ImplModule::stopAndReset()
{
	InternalReq* data;

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
		data->Runtime_CallTheUser(Cgi::Notify::ON_ERROR);
		mf_returnRequestData(*data);
	}
		
	m_executionQueue.clear();
}
