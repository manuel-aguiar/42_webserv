

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
ImplModule::mf_cancelAndReturn(InternalReq& data)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_returnExecutionUnit(*worker, false, Cgi::Notify::ON_ERROR);
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
