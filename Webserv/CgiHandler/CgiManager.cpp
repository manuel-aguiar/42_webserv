/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:36:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiManager.hpp"

CgiManager::CgiManager(const size_t maxConcurrent) :
	m_maxConcurrent(maxConcurrent),
	m_LiveRequests(maxConcurrent)
{

}

CgiManager::~CgiManager()
{

}
