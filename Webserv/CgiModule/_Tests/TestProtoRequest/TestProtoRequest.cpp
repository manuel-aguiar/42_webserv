

#include "TestProtoRequest.hpp"

#include <iomanip>

TestProtoRequest::TestProtoRequest(Globals& globals, Cgi::Module& cgi, int id) :
	m_globals(globals),
	m_cgi(cgi),
	m_TotalBytesRead(0),
    m_id(id),
    m_CgiResultStatus(E_CGI_STATUS_WORKING)

{
    m_buffer[0] = '\0';
}

TestProtoRequest::~TestProtoRequest()
{

}

TestProtoRequest::TestProtoRequest(const TestProtoRequest& copy) :
	m_globals(copy.m_globals),
	m_cgi(copy.m_cgi),
	m_TotalBytesRead(copy.m_TotalBytesRead)
{

}


void	TestProtoRequest::printBufStdout()
{
	m_buffer[m_TotalBytesRead] = '\0';
	//std::cout << m_buffer << std::endl;
}


void TestProtoRequest::debugPrint() const
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
    std::cout << "Globals Address: " << &m_globals << std::endl;
    std::cout << "Cgi::Module Address: " << &m_cgi << std::endl;

    std::cout << "========================================" << std::endl;
}