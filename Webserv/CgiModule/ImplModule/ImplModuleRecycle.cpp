

# include "ImplModule.hpp"
# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

void
ImplModule::_mf_recycleWorker(Worker& worker, bool markFdsAsStale)
{
	InternalRequest* newData;

	worker.reset();

	while (m_executionQueue.size() > 0)
	{
		newData = m_executionQueue.front();
		m_executionQueue.pop_front();

		if (newData->getState() != Cgi::RequestState::CANCELLED)
		{
			_mf_execute(worker, *newData, markFdsAsStale);
			return ;
		}
		_mf_recycleRequestData(*newData);
	}
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
ImplModule::_mf_recycleRequestData(InternalRequest& data)
{	
	TimerTracker<Timer, InternalRequest*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalRequest*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}

void
ImplModule::_mf_recycleSuccess(Worker& worker)
{
	_mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_SUCCESS);
}


void
ImplModule::_mf_recycleRuntimeFailure(Worker& worker)
{
	worker.stop();
	_mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::_mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale)
{
	_mf_recycleExecutionUnit(worker, markFdsAsStale, Cgi::Notify::ON_ERROR_STARTUP);
}

void
ImplModule::_mf_recycleTimeoutFailure(Worker& worker)
{
	worker.stop();
	_mf_recycleExecutionUnit(worker, false, Cgi::Notify::ON_ERROR_TIMEOUT);
}

void
ImplModule::_mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	_mf_recycleExecutionUnit(*worker, markFdsAsStale, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::_mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalRequest* 		data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	_mf_recycleWorker(worker, markFdsAsStale);
	_mf_recycleRequestData(*data);
	if (user && handler)
		(handler)(user);
}