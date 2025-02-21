

# include "TestProtoRequest.hpp"
# include "../../HeaderData/HeaderData.hpp"
# include <unistd.h>


Cgi::IO::State
TestProtoRequest::CgiRead(int readFd)
{
	int bytesRead;

	//std::cout << "buffer size before read: " << m_buffer.size() << std::endl;

	bytesRead = m_buffer.read(readFd, m_buffer.size());

	//std::cout << "\t read: " << bytesRead << " bytes, buffer size after read: " << m_buffer.size() << std::endl;
	//std::cout << m_buffer.view() << std::endl;
	
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
	m_CgiResultStatus = E_CGI_STATUS_ERROR;
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
	m_buffer.push(headers.getTempBody().data(), headers.getTempBody().size() - 1);

	//std::cout << "buffer size: " << m_buffer.size() << "\n\n" << m_buffer.view() << std::endl;

	//std::cout << "\n\n printed headers and temp body\n\n" << std::endl;

	return (Cgi::IO::CONTINUE);
}