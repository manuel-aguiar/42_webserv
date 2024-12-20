/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 10:08:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A_ProtoRequest.hpp"
#include "../../../Callback/Callback.hpp"


A_ProtoRequest::A_ProtoRequest(EventManager& manager, Globals& globals, CgiModule& cgi) :
	m_manager(manager),
	m_globals(globals),
	m_cgi(cgi)
{

}

A_ProtoRequest::~A_ProtoRequest()
{

}

void	A_ProtoRequest::requestCgi()
{
	m_curRequestData = m_cgi.acquireRequestData();
		//subscribe event callbacks
	for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
		m_curRequestData->setCallback(static_cast<e_CgiCallbacks>(i), this, A_ProtoRequest_CgiGateway::Callbacks[i]);
	
	m_curRequestData->setExtension("py");
	m_curRequestData->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
}


void	A_ProtoRequest::printBufStdout()
{
	std::cout << "called\n";
	std::cout << "Buffer: " << m_buffer << std::endl;
}