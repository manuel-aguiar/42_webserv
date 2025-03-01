

# include "HttpCgiGateway.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);
extern const char* getStatusMessage(int statusCode);
extern std::string generateDefaultErrorPage(int statusCode, const std::string& serverName, const std::string& errorMessage);

namespace Http
{
    Http::ResponseStatus::Type
    CgiGateway::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    CgiGateway::mf_fillResponseLine(BaseBuffer& writeBuffer)
    {
        writeBuffer.push("HTTP/1.1 ", 9);
        writeBuffer.push(StringUtils::to_string(m_statusCode).c_str(), StringUtils::to_string(m_statusCode).size());
        writeBuffer.push(" ", 1);
        writeBuffer.push(getStatusMessage(m_statusCode));
        writeBuffer.push("\r\n", 2);

        // go to next stage
        m_fillFunction = &CgiGateway::mf_fillHeaders;
        return (mf_fillHeaders(writeBuffer));
    }

	Http::ResponseStatus::Type
	CgiGateway::mf_fillHeaders(BaseBuffer& writeBuffer)
	{
		int current;
		const std::vector<Cgi::Header>& headers = m_headers->getHeaders();

		if (m_currentHeader == -1) // nothing sent yet
		{
			if (m_headers->hasBody())
				writeBuffer.push("Transfer-Encoding: chunked\r\n", 28);
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
				return (Http::ResponseStatus::WRITING);
			}
			// push header
			writeBuffer.push(headers[current].key.data(), headers[current].key.size());
			writeBuffer.push(": ", 2);
			writeBuffer.push(headers[current].value.data(), headers[current].value.size());
			writeBuffer.push("\r\n", 2);
		}

		m_currentHeader = current;

		if ((size_t)m_currentHeader != headers.size())
            return (Http::ResponseStatus::WRITING); // unfinished, still have headers to go
        else
            writeBuffer.push("\r\n", 2); // end of headers, potentially message

        if (m_headers->hasBody())
        {
            // go to next stage
            m_fillFunction = &CgiGateway::mf_fillBodyTemp;
            return (mf_fillBodyTemp(writeBuffer));
        }
        return (Http::ResponseStatus::FINISHED);
    }

    static void fillHexHeader(char* hexHeader, int hexheaderSize, int chunkSize)
    {
        const char* hex = "0123456789abcdef";
    
        int i = hexheaderSize - 1;
        hexHeader[i--] = '\n';
        hexHeader[i--] = '\r';
        
        while (i >= 0)
        {
            if (chunkSize)
            {
                hexHeader[i--] = hex[chunkSize % 16];
                chunkSize /= 16;
            }
            else
                hexHeader[i--] = '0';
        }
    }

    Http::ResponseStatus::Type
    CgiGateway::mf_fillBodyTemp(BaseBuffer& writeBuffer)
    {
        BufferView tempBody = m_headers->getTempBody();

        char        hexHeader[10] = {0};
        const int   hexHeaderSize = sizeof(hexHeader)/sizeof(hexHeader[0]);
        int         currentPosition;

        if (tempBody.size() == 0)
            goto startBodyStream;
        if (writeBuffer.available() < tempBody.size() + hexHeaderSize)
            return (Http::ResponseStatus::WAITING); // no room
        
        currentPosition = writeBuffer.size();

        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header
        fillHexHeader(hexHeader, hexHeaderSize, tempBody.size());
        std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);
        writeBuffer.push(tempBody.data(), tempBody.size());

    startBodyStream:

        // go to next stage
        m_fillFunction = &CgiGateway::mf_fillBodyStream;
        return (mf_fillBodyStream(writeBuffer));
    }



    Http::ResponseStatus::Type
    CgiGateway::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {
        int scriptBytesRead = 0;
        char hexHeader[10] = {0};
        const int hexHeaderSize = sizeof(hexHeader)/sizeof(hexHeader[0]);

        if (!m_canRead)
            return (Http::ResponseStatus::WAITING);
            
        if (writeBuffer.available() < hexHeaderSize + 1)
            return (Http::ResponseStatus::WAITING);
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push(hexHeader, hexHeaderSize); // make room for the hex header

        scriptBytesRead = writeBuffer.readAppend(m_readFd);
        if (scriptBytesRead == 0)
        {
            fillHexHeader(hexHeader, hexHeaderSize - 2, scriptBytesRead);

            hexHeader[hexHeaderSize - 2] = '\r';
            hexHeader[hexHeaderSize - 1] = '\n';

            std::memcpy(&writeBuffer[currentPosition], hexHeader, hexHeaderSize);

            m_fillFunction = &CgiGateway::mf_fillNothingToSend;

            return (Http::ResponseStatus::FINISHED);
        }

        fillHexHeader(hexHeader, hexHeaderSize, scriptBytesRead);
        std::memcpy(&writeBuffer[currentPosition], hexHeader, 10);

        m_canRead = false;

        return (Http::ResponseStatus::WRITING);
    }

    Http::ResponseStatus::Type
	CgiGateway::mf_fillErrorResponse(BaseBuffer& writeBuffer)
	{
        std::string codeStr = StringUtils::to_string(m_statusCode);
        std::string errorPage = generateDefaultErrorPage(m_statusCode, "42_webserv", "YOU SUCK");

		writeBuffer.push("HTTP/1.1 ", 9);
		writeBuffer.push(codeStr.c_str(), codeStr.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(getStatusMessage(m_statusCode));
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Content-Length: ", 15);
        writeBuffer.push(StringUtils::to_string(errorPage.size()).c_str(), StringUtils::to_string(errorPage.size()).size());
        writeBuffer.push("\r\n", 2);
		writeBuffer.push("Connection: close\r\n", 19);
        writeBuffer.push("Content-Type: text/html\r\n", std::strlen("Content-Type: text/html\r\n"));
		writeBuffer.push("\r\n", 2);
        writeBuffer.push(errorPage.c_str(), errorPage.size());

        return (Http::ResponseStatus::MARK_TO_CLOSE);
	}

}
