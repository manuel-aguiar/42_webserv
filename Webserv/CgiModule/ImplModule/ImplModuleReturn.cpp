

# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "ImplModule.hpp"

void
ImplModule::mf_returnWorker(Worker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
ImplModule::mf_returnRequestData(InternalReq& data)
{
	mf_recycleRequestData(data);
}

void
ImplModule::mf_cancelAndReturn(Worker& worker)
{
	InternalReq*		data = worker.accessRequestData();
	
	worker.stop();
	worker.disableCloseAllEvents(true);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
}

void
ImplModule::mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalReq* 			data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}

void
ImplModule::mf_returnPendingFinish(Worker& worker)
{
	InternalReq* 			data = worker.accessRequestData();
	
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
}
