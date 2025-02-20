

# include "TestProtoRequest.hpp"
# include <unistd.h>


Cgi::IO::BytesCount
TestProtoRequest::CgiRead(int readFd)
{
	int bytesRead;

	bytesRead = ::read(readFd, &m_buffer[m_TotalBytesRead], sizeof(m_buffer) - m_TotalBytesRead - 1);
	m_TotalBytesRead += bytesRead;
	m_buffer[m_TotalBytesRead] = '\0';
	
	return (bytesRead);
}

Cgi::IO::BytesCount
TestProtoRequest::CgiWrite(int writeFd)
{
	int bytesWritten;

	if (m_msgBody.size() == 0)
		return (0);
		
	bytesWritten = ::write(writeFd, m_msgBody.c_str(), m_msgBody.size());

	std::string wrote = m_msgBody.substr(0, bytesWritten);

	m_msgBody.erase(0, bytesWritten);

	return (bytesWritten);
}

void
TestProtoRequest::SuccessCgi()
{
	m_CgiResultStatus = E_CGI_STATUS_SUCCESS;
}

/*
	The oposite of execute Cgi, delete events and finish request
*/
void
TestProtoRequest::cancelCgi()
{	
	m_CgiResultStatus = E_CGI_STATUS_ERROR_RUNTIME;
}

void
TestProtoRequest::falseStartCgi()
{
	
	m_CgiResultStatus = E_CGI_STATUS_ERROR_STARTUP;
}


void	
TestProtoRequest::timeoutCgi()
{
	m_CgiResultStatus = E_CGI_STATUS_TIMEOUT;
}