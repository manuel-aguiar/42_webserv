/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:19:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 09:11:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiModule.hpp"

CgiModule::CgiModule(size_t maxConnections, Globals& globals) :
	m_maxConnections(maxConnections),
	m_liveRequests(),
	m_pendingRequests(),
	m_spareLiveRequests(),
	m_globals(globals)
{
	m_liveRequests.reserve(maxConnections);
	for (size_t i = 0; i < maxConnections; i++)
		m_spareLiveRequests.emplace_back(*this);
}

void	CgiModule::addInterpreter(const std::string& extension, const std::string& path)
{
	m_scriptInterpreters[extension] = path;
}

CgiRequestData&	CgiModule::acquireRequestData()
{
	m_pendingRequests.emplace_back();
	return (m_pendingRequests.back());
}