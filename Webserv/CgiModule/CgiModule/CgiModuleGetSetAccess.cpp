/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleGetSetAccess.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:03:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 14:33:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiModule.hpp"

const t_CgiEnvValue*						CgiModule::getBaseEnvKeys() const
{
	return (m_baseEnv);
}

const std::map<t_extension, t_path>&	CgiModule::getInterpreters() const
{
	return (m_Interpreters);
}

size_t									CgiModule::getBusyWorkerCount() const
{
	return (m_allWorkers.size() - m_availableWorkers.size());
}

size_t 									CgiModule::getQueueSize() const
{
	return (m_executionQueue.size());
}