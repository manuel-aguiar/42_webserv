/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 09:22:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../CgiLiveRequest/CgiLiveRequest.hpp"
# include "CgiModule.hpp"

CgiRequestData&	CgiModule::acquireRequestData()
{
	List<ManagedRequestData>::iterator newRequestData;
	
	m_allRequestData.emplace_back();
	newRequestData = --m_allRequestData.end();
	newRequestData->setDataLocation(newRequestData);
	return (m_allRequestData.back());
}

void	CgiModule::executeRequest(CgiRequestData& request)
{

	// Debug
	#ifndef NDEBUG

		// assert the requestdata belongs to this CgiModule

		bool test = true;
		for (List<ManagedRequestData>::iterator iter = m_allRequestData.begin(); 
			iter != m_allRequestData.end(); 
			++iter)
		{
			if (&request != &(*iter))
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::executeRequest : CgiRequestedData is not managed by this CgiModule");
		
		// assert this request is not in the waiting queue already

		for (List<ManagedRequestData*>::iterator iter = m_pendingRequests.begin(); 
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

		for (DynArray<CgiLiveRequest>::iterator iter = m_liveRequests.begin(); 
			iter != m_liveRequests.end(); 
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

	ManagedRequestData*					requestData;
	CgiLiveRequest* 					liveRequest;
	List<ManagedRequestData*>::iterator pendingIter;

	requestData = static_cast<ManagedRequestData*>(&request);

	if (m_spareLiveRequests.size() == 0)
	{
		m_pendingRequests.push_back(requestData);
		pendingIter = --m_pendingRequests.end();
		requestData->setPendingLocation(pendingIter);
		return ;
	}
	liveRequest = m_spareLiveRequests.front();
	m_spareLiveRequests.pop_front();
	m_liveRequestCount++;
	requestData->setExecutor(liveRequest);
	liveRequest->execute(request);
}

void	CgiModule::cancelRequest(CgiRequestData& request)
{
	// Debug
	#ifndef NDEBUG

		// assert the requestdata belongs to this CgiModule

		bool test = true;
		for (List<ManagedRequestData>::iterator iter = m_allRequestData.begin(); 
			iter != m_allRequestData.end(); 
			++iter)
		{
			if (&request != &(*iter))
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::cancelRequest : CgiRequestedData is not managed by this CgiModule");
		
		// assert this request is not in the waiting queue already

		for (List<ManagedRequestData*>::iterator iter = m_pendingRequests.begin(); 
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
			"CgiModule::cancelRequest : this CgiRequestedData is not queued for execution");
		
		// assert this request is not being executed already right now

		for (DynArray<CgiLiveRequest>::iterator iter = m_liveRequests.begin(); 
			iter != m_liveRequests.end(); 
			++iter)
		{
			if (&request == (*iter).accessCurRequestData())
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test,
			"CgiModule::cancelRequest : this CgiRequestedData is not being executed");
	#endif
	

	// implementation
	ManagedRequestData* requestData;
	CgiLiveRequest*		liveRequest;

	requestData = static_cast<ManagedRequestData*>(&request);
	liveRequest = requestData->accessExecutor();

	if (liveRequest)
	{
		requestData->setExecutor(NULL);
		liveRequest->forcedClose();
	}	
	else
		mf_deleteRequestData(*requestData);
}

void	CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void	CgiModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}