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

#include <iomanip>

A_ProtoRequest::A_ProtoRequest(EventManager& manager, Globals& globals, CgiModule& cgi, int id) :
	m_eventManager(manager),
	m_globals(globals),
	m_cgi(cgi),
	m_TotalBytesRead(0),
    m_id(id),
    m_CgiResultStatus(E_CGI_STATUS_WORKING)

{
    m_buffer[0] = '\0';
}

A_ProtoRequest::~A_ProtoRequest()
{

}

A_ProtoRequest::A_ProtoRequest(const A_ProtoRequest& copy) :
	m_eventManager(copy.m_eventManager),
	m_globals(copy.m_globals),
	m_cgi(copy.m_cgi),
	m_TotalBytesRead(copy.m_TotalBytesRead)
{

}


void	A_ProtoRequest::printBufStdout()
{
	m_buffer[m_TotalBytesRead] = '\0';
	//std::cout << m_buffer << std::endl;
}


void A_ProtoRequest::debugPrint() const
{
    std::cout << "===== Debug Info for A_ProtoRequest =====" << std::endl;

    // Printing pointers (address or null)
    std::cout << "CgiRequestData: " << (m_CgiRequestData ? "Valid Pointer" : "NULL") << std::endl;

    // Printing sizes
    std::cout << "Total Bytes Read: " << m_TotalBytesRead << std::endl;

    // Printing string content
    std::cout << "Message Body: " << m_msgBody << std::endl;

    // Printing buffer content as a string and hex
    std::cout << "Buffer Content (as string): \"" << std::string(m_buffer, m_TotalBytesRead) << "\"" << std::endl;
    std::cout << "Buffer Content (hex dump): ";
    for (size_t i = 0; i < m_TotalBytesRead; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)(unsigned char)m_buffer[i] << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal formatting

    // Manager and Globals addresses
    std::cout << "EventManager Address: " << &m_eventManager << std::endl;
    std::cout << "Globals Address: " << &m_globals << std::endl;
    std::cout << "CgiModule Address: " << &m_cgi << std::endl;

    std::cout << "========================================" << std::endl;
}