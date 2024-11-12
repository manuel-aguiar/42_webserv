/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:50:14 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiManager.hpp"

CgiManager::CgiManager(const size_t maxConcurrent) :
	m_maxConcurrent(maxConcurrent),
	m_currentConcurrent(0),
	m_LiveRequests(maxConcurrent),
	m_readEvents(maxConcurrent)

{

}

CgiManager::~CgiManager()
{

}
