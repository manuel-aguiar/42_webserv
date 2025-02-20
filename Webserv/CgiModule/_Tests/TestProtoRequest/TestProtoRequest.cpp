

#include "TestProtoRequest.hpp"

#include <iomanip>

TestProtoRequest::TestProtoRequest(Globals& globals, Cgi::Module& cgi, int id) :
	m_globals(globals),
	m_cgi(cgi),
	m_TotalBytesRead(0),
    m_id(id),
    m_headerStatus(-1),
    m_CgiResultStatus(E_CGI_STATUS_WORKING)

{
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
	m_buffer.push("", 1);
	//std::cout << m_buffer << std::endl;
}
