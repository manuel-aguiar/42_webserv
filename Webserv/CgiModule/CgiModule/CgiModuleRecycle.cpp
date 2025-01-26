

# include "CgiModule.hpp"
# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

void
ImplModule::mf_recycleWorker(Worker& worker, bool markFdsAsStale)
{
	InternalRequest* newData;

	worker.reset();

	while (m_executionQueue.size() > 0)
	{
		newData = m_executionQueue.front();
		m_executionQueue.pop_front();

		if (newData->getState() != Cgi::RequestState::CANCELLED)
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
ImplModule::mf_recycleRequestData(InternalRequest& data)
{	
	TimerTracker<Timer, InternalRequest*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalRequest*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}

void
ImplModule::mf_recycleSuccess(Worker& worker)
{
	mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_SUCCESS);
}


void
ImplModule::mf_recycleRuntimeFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale)
{
	mf_recycleExecutionUnit(worker, markFdsAsStale, Cgi::Notify::ON_ERROR_STARTUP);
}

void
ImplModule::mf_recycleTimeoutFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, false, Cgi::Notify::ON_ERROR_TIMEOUT);
}

void
ImplModule::mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_recycleExecutionUnit(*worker, markFdsAsStale, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalRequest* 		data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_recycleWorker(worker, markFdsAsStale);
	mf_recycleRequestData(*data);
	if (user && handler)
		(handler)(user);
}