

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
	mf_recycleExecutionUnit(worker, true, Cgi::Notify::ON_ERROR);
}

void
ImplModule::_WorkerRecycleStartupFailure(Worker& worker, bool markFdsAsStale)
{
	mf_recycleExecutionUnit(worker, markFdsAsStale, Cgi::Notify::ON_ERROR);
}

void
ImplModule::mf_recycleWorker(Worker& worker, bool markFdsAsStale)
{
	InternalReq* newData;

	worker.reset();

	#ifndef NDEBUG
		for (size_t i = 0; i < m_availableWorkers.size(); ++i)
			ASSERT_EQUAL(m_availableWorkers[i] != &worker, true, 
			"ImplModule::mf_recycleWorker(), worker is already available");
	#endif

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
	#ifndef NDEBUG
		for (size_t i = 0; i < m_availableRequestData.size(); ++i)
			ASSERT_EQUAL(m_availableRequestData[i] != &data, true, 
			"ImplModule::mf_recycleRequestData(), request is already available");
	#endif

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
	InternalReq*	data = worker.accessRequestData();

	ASSERT_EQUAL(data != NULL, true, "ImplModule::mf_recycleTimeoutFailure(), there must be a worker that is still associated");
	
	worker.stop();
	mf_recycleExecutionUnit(worker, false, Cgi::Notify::ON_ERROR);
}

void
ImplModule::mf_cancelAndRecycle(InternalReq& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	worker->disableCloseAllEvents(markFdsAsStale);
	mf_recycleWorker(*worker, markFdsAsStale);
	mf_recycleRequestData(data);
}

void
ImplModule::mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalReq* 			data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	data->setState(Cgi::RequestState::PENDING_FINISH);
	if (user && handler)
		(handler)(user);
}

void
ImplModule::mf_recyclePendingFinish(InternalReq& data, bool markFdsAsStale)
{
	Worker*		worker = data.accessExecutor();

	ASSERT_EQUAL(worker != NULL, true, "ImplModule::mf_recyclePendingFinish(), there must be a worker that is still associated");
	mf_recycleWorker(*worker, markFdsAsStale);
	mf_recycleRequestData(data);
}
