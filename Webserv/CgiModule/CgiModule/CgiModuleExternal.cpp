/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 10:01:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../CgiLiveRequest/CgiLiveRequest.hpp"
# include "CgiModule.hpp"

CgiRequestData&	CgiModule::acquireRequestData()
{
	m_allRequestData.emplace_back();
	return (m_allRequestData.back());
}

void	CgiModule::executeRequest(CgiRequestData& request)
{
	CgiLiveRequest* liveRequest;

	if (m_spareLiveRequests.size() == 0)
	{
		m_pendingRequests.push_back(static_cast<ManagedRequestData*>(&request));
		return ;
	}
	liveRequest = m_spareLiveRequests.front();
	m_spareLiveRequests.pop_front();
	liveRequest->execute(request);
}

void	CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_Interpreters[extension] = path;
}

void	CgiModule::removeInterpreter(const std::string& extension)
{
	m_Interpreters.erase(extension);
}