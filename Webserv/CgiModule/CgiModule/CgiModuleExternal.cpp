/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 13:59:02 by mmaria-d         ###   ########.fr       */
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
	data = m_availableRequestData.front();
	m_availableRequestData.pop_front();
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
		for (List<InternalCgiRequestData*>::iterator iter = m_pendingRequests.begin(); 
			iter != m_pendingRequests.end(); 
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

	if (m_availableCgiWorkers.size() == 0)
	{
		m_pendingRequests.push_back(requestData);
		pendingIter = --m_pendingRequests.end();
		requestData->setPendingLocation(pendingIter);
		return ;
	}
	worker = m_availableCgiWorkers.front();
	m_availableCgiWorkers.pop_front();
	m_busyWorkerCount++;
	requestData->setExecutor(worker);
	worker->execute(request);
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
	for (DynArray<InternalCgiWorker>::iterator it = m_allWorkers.begin(); it != m_allWorkers.end(); it++)
	{
		it->forcedClose();
		it->reset();
	}
}