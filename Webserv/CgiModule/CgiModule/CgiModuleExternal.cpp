/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 18:11:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

CgiRequestData*	CgiModule::acquireRequestData()
{
	InternalCgiRequestData*     data;

	if (!m_availableRequestData.size())
		return (NULL);
	data = m_availableRequestData.back();
	m_availableRequestData.pop_back();
	return (data);
}

void	CgiModule::executeRequest(CgiRequestData& request)
{
/*
	// Debug
	#ifndef NDEBUG

		// assert the requestdata belongs to this CgiModule

		bool test = false;
		for (List<InternalCgiRequestData>::iterator iter = m_allRequestData.begin(); 
			iter != m_allRequestData.end(); 
			++iter)
		{
			if (&request == &(*iter))
			{
				test = true;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::executeRequest : CgiRequestedData is not managed by this CgiModule");
		
		// assert this request is not in the waiting queue already

		test = true;
		for (List<InternalCgiRequestData*>::iterator iter = m_executionQueue.begin(); 
			iter != m_executionQueue.end(); 
			++iter)
		{
			if (&request == (*iter))
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::executeRequest : this CgiRequestedData is already queued for execution");
		
		// assert this request is not being executed already right now
		test = true;
		for (DynArray<InternalCgiWorker>::iterator iter = m_allWorkers.begin(); 
			iter != m_allWorkers.end(); 
			++iter)
		{
			if (&request == (*iter).accessCurRequestData())
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::executeRequest : this CgiRequestedData is already being executed");
	#endif

*/

	InternalCgiRequestData*					requestData;
	InternalCgiWorker* 						worker;
	List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator 
											pendingIter;

	requestData = static_cast<InternalCgiRequestData*>(&request);

	if (m_availableWorkers.size() == 0)
	{
		m_executionQueue.push_back(requestData);
		pendingIter = --m_executionQueue.end();
		requestData->setQueuePosition(pendingIter);
		return ;
	}
	worker = m_availableWorkers.back();
	m_availableWorkers.pop_back();
	m_busyWorkerCount++;
	requestData->setExecutor(worker);
	worker->execute(*requestData);
}

void	CgiModule::cancelRequest(CgiRequestData& request)
{
/*
	// Debug
	#ifndef NDEBUG


		bool test = false;
		for (List<InternalCgiRequestData>::iterator iter = m_allRequestData.begin(); 
			iter != m_allRequestData.end(); 
			++iter)
		{
			if (&(*iter) == &request)
			{
				test = true;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::cancelRequest : CgiRequestedData is not managed by this CgiModule");
	#endif
*/	

	// implementation
	InternalCgiRequestData* requestData;
	InternalCgiWorker*		worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);
	worker = requestData->accessExecutor();

	if (worker)
		worker->forcedClose();

	mf_returnRequestData(*requestData);
}

void	CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void	CgiModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}

void	CgiModule::forceStop()
{
	for (HeapArray<InternalCgiWorker>::iterator it = m_allWorkers.begin(); it != m_allWorkers.end(); it++)
	{
		it->forcedClose();
		it->reset();
	}
}

void	CgiModule::finishRequest(CgiRequestData& request)
{
	InternalCgiRequestData* requestData;
	InternalCgiWorker*		worker;

	requestData = static_cast<InternalCgiRequestData*>(&request);
	worker = requestData->accessExecutor();
	if (worker)
		worker->cleanClose();
}