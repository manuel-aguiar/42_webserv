

# include "ImplModule.hpp"
# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

void
ImplModule::_WorkerRecycleSuccess(Worker& worker)
{
	mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_SUCCESS);
}


void
ImplModule::_WorkerRecycleRuntimeFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::_WorkerRecycleStartupFailure(Worker& worker, bool markFdsAsStale)
{
	mf_recycleExecutionUnit(worker, markFdsAsStale, Cgi::Notify::ON_ERROR_STARTUP);
}

void
ImplModule::mf_recycleWorker(Worker& worker, bool markFdsAsStale)
{
	InternalReq* newData;

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
ImplModule::mf_recycleRequestData(InternalReq& data)
{	
	TimerTracker<Timer, InternalReq*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalReq*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}


void
ImplModule::mf_recycleTimeoutFailure(Worker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, false, Cgi::Notify::ON_ERROR_TIMEOUT);
}

void
ImplModule::mf_cancelAndRecycle(InternalReq& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_recycleExecutionUnit(*worker, markFdsAsStale, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalReq* 		data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_recycleWorker(worker, markFdsAsStale);
	mf_recycleRequestData(*data);
	if (user && handler)
		(handler)(user);
}