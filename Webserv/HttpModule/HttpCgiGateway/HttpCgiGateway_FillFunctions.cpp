

# include "HttpCgiGateway.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);
extern const char* getStatusMessage(int statusCode);

namespace Http
{

    Response::Status
    CgiGateway::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Response::WAITING);
    }

    Response::Status
    CgiGateway::mf_fillResponseLine(BaseBuffer& writeBuffer)
    {
        writeBuffer.push("HTTP/1.1 ", 9);
        writeBuffer.push(TestHelpers::to_string(m_statusCode).c_str(), TestHelpers::to_string(m_statusCode).size());
        writeBuffer.push(" ", 1);
        writeBuffer.push(getStatusMessage(m_statusCode));
        writeBuffer.push("\r\n", 2);

        // go to next stage
        m_fillFunction = &CgiGateway::mf_fillHeaders;
        return ((this->*m_fillFunction)(writeBuffer));
    }

	Response::Status
	CgiGateway::mf_fillHeaders(BaseBuffer& writeBuffer)
	{
		int current;
		const std::vector<Cgi::Header>& headers = m_headers->getHeaders();

		if (m_currentHeader == -1) // nothing sent yet
		{
			if (m_headers->hasBody())
				writeBuffer.push("Transfer-Encoding: chunked\r\n", 26);
			else
				writeBuffer.push("Content-Length: 0\r\n", 19);
            m_currentHeader = 0;
		}

		current = m_currentHeader;

		for (; (size_t)current < headers.size(); current++)
		{
			// ignored header, skip
			if (isHeaderIgnored(headers[current]))
				continue ;
			
			// no room, exit
			if (writeBuffer.available() < headers[current].key.size() + headers[current].value.size() + 6) // ": " + "\r\n" * 2 to be safe at the final header
			{
				m_currentHeader = current;
				return (Response::WRITING);
			}
			// push header
			writeBuffer.push(headers[current].key.data(), headers[current].key.size());
			writeBuffer.push(": ", 2);
			writeBuffer.push(headers[current].value.data(), headers[current].value.size());
			writeBuffer.push("\r\n", 2);
		}

		m_currentHeader = current;

		if ((size_t)m_currentHeader != headers.size())
            return (Response::WRITING); // unfinished, still have headers to go
        else
            writeBuffer.push("\r\n", 2); // end of headers, potentially message

        if (m_headers->hasBody())
        {
            // go to next stage
            m_fillFunction = &CgiGateway::mf_fillBodyTemp;
            return ((this->*m_fillFunction)(writeBuffer));
        }
        return (Response::FINISHED);
    }

    static void fillHexHeader(char* hexHeader, int hexheaderSize, int chunkSize)
    {
        const char* hex = "0123456789abcdef";
    
        int i = hexheaderSize - 1;
        hexHeader[i--] = '\n';
        hexHeader[i--] = '\r';
        
        while (i >= 0)
        {
            std::cout << i << std::endl;
            if (chunkSize)
            {
                hexHeader[i--] = hex[chunkSize % 16];
                chunkSize /= 16;
            }
            else
                hexHeader[i--] = '0';
        }
    }

    Response::Status
    CgiGateway::mf_fillBodyTemp(BaseBuffer& writeBuffer)
    {
        BufferView tempBody = m_headers->getTempBody();

        char        hexHeader[10] = {0};
        const int   hexHeaderSize = sizeof(hexHeader)/sizeof(hexHeader[0]);
        int         currentPosition;

        if (tempBody.size() == 0)
            goto startBodyStream;
        if (writeBuffer.available() < tempBody.size() + hexHeaderSize)
            return (Response::WAITING); // no room
        
        currentPosition = writeBuffer.size();

        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header
        fillHexHeader(hexHeader, hexHeaderSize, tempBody.size());
        std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);
        writeBuffer.push(tempBody.data(), tempBody.size());

    startBodyStream:

        // go to next stage
        m_fillFunction = &CgiGateway::mf_fillBodyStream;
        return ((this->*m_fillFunction)(writeBuffer));
    }



    Response::Status
    CgiGateway::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {
        int scriptBytesRead = 0;
        char hexHeader[10] = {0};
        const int hexHeaderSize = sizeof(hexHeader)/sizeof(hexHeader[0]);

        if (!m_canRead)
            return (Response::WAITING);

            
        if (writeBuffer.available() < hexHeaderSize + 1)
            return (Response::WAITING);
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header

        scriptBytesRead = writeBuffer.read(m_readFd);
        if (scriptBytesRead == 0)
        {
            fillHexHeader(hexHeader, hexHeaderSize - 2, scriptBytesRead);

            hexHeader[hexHeaderSize - 1] = '\n';
            hexHeader[hexHeaderSize - 2] = '\r';

            std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);
            return (Response::FINISHED);
        }

        fillHexHeader(hexHeader, hexHeaderSize, scriptBytesRead);
        std::memcpy(&writeBuffer[currentPosition], hexHeader, 10);

        m_canRead = false;

        return (Response::WRITING);
    }

    Response::Status
	CgiGateway::mf_fillErrorResponse(BaseBuffer& writeBuffer)
	{

		// probably ask response to send response based on the error status code
        std::string codeStr = TestHelpers::to_string(m_statusCode);

		writeBuffer.push("HTTP/1.1 ", 9);
		writeBuffer.push(codeStr.c_str(), codeStr.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(getStatusMessage(m_statusCode));
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Content-Length: 37\r\n", 19);
		writeBuffer.push("Connection: close\r\n", 19);
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Cgi script failed to execute correctly", 37);

		return (Response::MARK_TO_CLOSE);
	}

}