

# include "../Worker/Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "ImplModule.hpp"


void
ImplModule::_mf_returnWorker(Worker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
ImplModule::_mf_returnRequestData(InternalRequest& data)
{
	_mf_recycleRequestData(data);
}

void
ImplModule::_mf_cancelAndReturn(InternalRequest& data)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	_mf_returnExecutionUnit(*worker, false, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::_mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalRequest* 		data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	_mf_returnWorker(worker);
	_mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}