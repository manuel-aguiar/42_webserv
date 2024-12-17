/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleExecute.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:05:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 15:10:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiModule.hpp"

void	CgiModule::executeRequest(CgiRequestData& request)
{
	CgiLiveRequest* liveRequest;

	if (m_spareLiveRequests.size() == 0)
	{
		m_pendingRequests.push_back(&request);
		return ;
	}
	liveRequest = m_spareLiveRequests.front();
	m_spareLiveRequests.pop_front();
	liveRequest->execute(request);
}