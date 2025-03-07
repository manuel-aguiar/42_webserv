

# include "HttpCgiResponse.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int          checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool         isHeaderIgnored(const Cgi::Header& header);
extern const char*  getStatusMessage(int statusCode);
extern std::string  generateDefaultErrorPage(int statusCode, const std::string& serverName, const std::string& errorMessage);
extern std::string  getCurrentDate();

namespace Http
{
    Http::IOStatus::Type
    CgiResponse::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;

        return (Http::IOStatus::WAITING);
    }

    Http::IOStatus::Type
    CgiResponse::mf_fillFinish(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        if (m_responseData->closeAfterSending)
            return (Http::IOStatus::MARK_TO_CLOSE);
        return (Http::IOStatus::FINISHED);
    }

    Http::IOStatus::Type
    CgiResponse::mf_fillResponseLine(BaseBuffer& writeBuffer)
    {
        writeBuffer.push(BufferView(Http::HttpStandard::HTTP_VERSION));
        writeBuffer.push(" ", 1);
        writeBuffer.push(StringUtils::to_string(m_statusCode).c_str(), StringUtils::to_string(m_statusCode).size());
        writeBuffer.push(" ", 1);
        writeBuffer.push(getStatusMessage(m_statusCode));
        writeBuffer.push("\r\n", 2);

        // go to next stage
        m_fillFunction = &CgiResponse::mf_fillHeaders;
        return (mf_fillHeaders(writeBuffer));
    }

	Http::IOStatus::Type
	CgiResponse::mf_fillHeaders(BaseBuffer& writeBuffer)
	{
		int current;
		const std::vector<Cgi::Header>& headers = m_headers->getHeaders();

		if (m_currentHeader == -1) // nothing sent yet
		{
            writeBuffer.push("Connection: keep-alive\r\n", 24);
            writeBuffer.push("Server: 42_webserv/1.0\r\n", 24);
            writeBuffer.push("Date: ", 6);
            writeBuffer.push(BufferView(getCurrentDate()));
            writeBuffer.push("\r\n", 2);
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
				return (Http::IOStatus::WRITING);
			}
			// push header
			writeBuffer.push(headers[current].key.data(), headers[current].key.size());
			writeBuffer.push(": ", 2);
			writeBuffer.push(headers[current].value.data(), headers[current].value.size());
			writeBuffer.push("\r\n", 2);
		}

		m_currentHeader = current;

		if ((size_t)m_currentHeader != headers.size())
            return (Http::IOStatus::WRITING); // unfinished, still have headers to go
        else
            writeBuffer.push("\r\n", 2); // end of headers, potentially message

        if (m_headers->hasBody())
        {
            m_fillFunction = &CgiResponse::mf_fillBodyTemp;
            return (mf_fillBodyTemp(writeBuffer));
        }
        mf_finishAndRelease();
        return (Http::IOStatus::FINISHED);
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

    Http::IOStatus::Type
    CgiResponse::mf_fillBodyTemp(BaseBuffer& writeBuffer)
    {
        BufferView  thisPush;
        size_t      thisPushSize;
        const int   hexHeaderSize = 10;  // 10 bytes for the hex header (8 bytes for the size, 2 for \r\n)
        int         currentPosition;

        if (m_tempBody.size() == 0)
        {
            m_fillFunction = &CgiResponse::mf_fillBodyStream;
            return (mf_fillBodyStream(writeBuffer));
        }
        if (writeBuffer.available() < hexHeaderSize + 1 + 2) // +1 byte to send minimum, + 2 for \r\n
            return (Http::IOStatus::WAITING); // no room
        
        thisPushSize = std::min(writeBuffer.available() - hexHeaderSize - 2, m_tempBody.size());

        if (thisPushSize == 0)
            return (Http::IOStatus::WAITING);

        currentPosition = writeBuffer.size();
        writeBuffer.push("0", hexHeaderSize); // make room for the hex header

        fillHexHeader((char *)&writeBuffer[currentPosition], hexHeaderSize, thisPushSize);
        
        thisPush = m_tempBody.substr(0, thisPushSize);

        if (thisPush.size() == m_tempBody.size())
            m_tempBody = BufferView();
        else
            m_tempBody = m_tempBody.substr(thisPushSize, m_tempBody.size() - thisPushSize);

        writeBuffer.push(thisPush);
        writeBuffer.push("\r\n", 2);

        return (Http::IOStatus::WRITING);
    }



    Http::IOStatus::Type
    CgiResponse::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {
        int scriptBytesRead = 0;
        const int hexHeaderSize = 10;   // 10 bytes for the hex header (8 bytes for the size, 2 for \r\n)

        if (!m_canRead)
            return (Http::IOStatus::WAITING);

        if (m_readFd == Ws::FD_NONE)
        {
            if (writeBuffer.available() < 5)
                return (Http::IOStatus::WAITING); // no room for 0\r\n\r\n
            writeBuffer.push("0\r\n\r\n", 5);
            mf_finishAndRelease();
            m_fillFunction = &CgiResponse::mf_fillNothingToSend;
            return (Http::IOStatus::FINISHED);
        }
            
        if (writeBuffer.available() < hexHeaderSize + 1 + 2) // +1 byte to send minimum, + 2 for \r\n
            return (Http::IOStatus::WAITING);
        
        size_t currentPosition = writeBuffer.size();
        writeBuffer.push("0", hexHeaderSize); // make room for the hex header

        scriptBytesRead = writeBuffer.readAppend(m_readFd, writeBuffer.available() - 2);
        if (scriptBytesRead <= 0)
        {
            writeBuffer.pop(hexHeaderSize);
            writeBuffer.push("0\r\n\r\n", 5);
            mf_finishAndRelease();
            m_fillFunction = &CgiResponse::mf_fillNothingToSend;
            return (Http::IOStatus::FINISHED);
        }
        
        fillHexHeader((char *)&writeBuffer[currentPosition], hexHeaderSize, scriptBytesRead);
        writeBuffer.push("\r\n", 2);
        
        m_canRead = false;
        return (Http::IOStatus::WRITING);
    }

}
