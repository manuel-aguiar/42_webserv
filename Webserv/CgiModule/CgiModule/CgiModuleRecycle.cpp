

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"

namespace Cgi
{

void
Module::mf_recycleWorker(Worker& worker, bool markFdsAsStale)
{
    InternalRequest* newData;

    worker.reset();

	while (m_executionQueue.size() > 0)
	{
        newData = m_executionQueue.front();
        m_executionQueue.pop_front();

		if (newData->getState() != RequestState::CANCELLED)
		{
			mf_execute(worker, *newData, markFdsAsStale);
			return ;
		}
        mf_recycleRequestData(*newData);
	}
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
Module::mf_recycleRequestData(InternalRequest& data)
{	
	TimerTracker<Timer, InternalRequest*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalRequest*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}

void
Module::mf_recycleSuccess(Worker& worker)
{
	mf_recycleExecutionUnit(worker, true, Runtime_Callback::ON_SUCCESS);
}


void
Module::mf_recycleRuntimeFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, true, Runtime_Callback::ON_ERROR_RUNTIME);
}

void
Module::mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale)
{
	mf_recycleExecutionUnit(worker, markFdsAsStale, Runtime_Callback::ON_ERROR_STARTUP);
}

void
Module::mf_recycleTimeoutFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, false, Runtime_Callback::ON_ERROR_TIMEOUT);
}

void
Module::mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_recycleExecutionUnit(*worker, markFdsAsStale, Runtime_Callback::ON_ERROR_RUNTIME);
}

void
Module::mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Runtime_Callback::Type callUser)
{
	InternalRequest* 			data = worker.accessRequestData();
	User 						user = data->getUser();
	Runtime_Callback::Handler	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_recycleWorker(worker, markFdsAsStale);
	mf_recycleRequestData(*data);
	if (user && handler)
		(handler)(user);
}

}; // namespace Cgi