

# include "TestProtoRequest.hpp"
# include "../../HeaderData/HeaderData.hpp"
# include <unistd.h>


Cgi::IO::State
TestProtoRequest::CgiRead(int readFd)
{
	int bytesRead;

	bytesRead = m_buffer.read(readFd, m_TotalBytesRead);
	m_TotalBytesRead += bytesRead;
	
	if (bytesRead == 0)
		return (Cgi::IO::CLOSE);
	return (Cgi::IO::CONTINUE);
}

Cgi::IO::State
TestProtoRequest::CgiWrite(int writeFd)
{
	int bytesWritten;

	if (m_msgBody.size() == 0)
		return (Cgi::IO::CLOSE);

	bytesWritten = ::write(writeFd, m_msgBody.c_str(), m_msgBody.size());

	std::string wrote = m_msgBody.substr(0, bytesWritten);

	m_msgBody.erase(0, bytesWritten);
	
	if (bytesWritten == 0 || m_msgBody.size() == 0)
		return (Cgi::IO::CLOSE);

	return (Cgi::IO::CONTINUE);
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
TestProtoRequest::FailureCgi()
{	
	m_CgiResultStatus = E_CGI_STATUS_ERROR_RUNTIME;
}


Cgi::IO::State
TestProtoRequest::CgiReceiveHeaders(const Cgi::HeaderData& headers)
{
	m_headerStatus = headers.getStatusCode();
	std::vector<Cgi::Header> headersList = headers.getHeaders();

	// push headers to buffer
	for (size_t i = 0; i < headersList.size(); ++i)
	{
		m_buffer.push(headersList[i].key.data(), headersList[i].key.size());
		m_buffer.push(": ", 2);
		m_buffer.push(headersList[i].value.data(), headersList[i].value.size());
		m_buffer.push("\n", 1);
	}

	m_buffer.push("\n", 1); // end of headers
	m_buffer.push(headers.getTempBody().data(), headers.getTempBody().size());

	return (Cgi::IO::CONTINUE);
}