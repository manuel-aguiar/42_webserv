

# include "HttpCgiResponse.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int          checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool         isHeaderIgnored(const Cgi::Header& header);
extern BufferView   getStatusMessage(Http::Status::Number statusCode);
extern std::string  generateDefaultErrorPage(Http::Status::Number statusCode, const std::string& serverName, const std::string& errorMessage);
extern std::string  getCurrentDate();
extern std::string  getMimeType(const std::string &path);

namespace Http
{
    Http::IOStatus::Type
	CgiResponse::mf_fillErrorResponse(BaseBuffer& writeBuffer)
	{
        std::string codeStr = StringUtils::to_string(m_statusCode);

        writeBuffer.push(BufferView(Http::HttpStandard::HTTP_VERSION));
        writeBuffer.push(" ", 1);
        writeBuffer.push(BufferView(codeStr));
        writeBuffer.push(" ", 1);
        writeBuffer.push(getStatusMessage(m_statusCode));
        writeBuffer.push("\r\n", 2);

        if (m_responseData->closeAfterSending)
            writeBuffer.push("Connection: close\r\n", 19);
        else
            writeBuffer.push("Connection: keep-alive\r\n", 24);

        writeBuffer.push("Server: 42_webserv/1.0\r\n", 24);
        writeBuffer.push("Date: ", 6);
        writeBuffer.push(BufferView(getCurrentDate()));
        writeBuffer.push("\r\n", 2);

        if (m_responseData->serverBlock != NULL)
        {
            const std::map<int, std::string>& errorPages = m_responseData->serverBlock->getErrorPages();
            std::map<int, std::string>::const_iterator targetPage = errorPages.find(m_statusCode);
            if (targetPage != errorPages.end()
            && m_file.open(targetPage->second.c_str(), O_RDONLY, 0666))
            {
                writeBuffer.push("Content-Length: ", 16);
                writeBuffer.push(BufferView(StringUtils::to_string(m_file.size()).c_str()));
                writeBuffer.push("\r\n", 2);
                writeBuffer.push("Content-Type: ", 14);
                writeBuffer.push(BufferView(getMimeType(targetPage->second.c_str())));
                writeBuffer.push("\r\n\r\n", 4);
                m_fillFunction = &CgiResponse::mf_fillErrorFile;
                return (mf_fillErrorFile(writeBuffer));
            }
        }
        m_defaultErrorPage = generateDefaultErrorPage(m_statusCode, "42_webserv", "Bad Server-Side Script");
		writeBuffer.push("Content-Length: ", 16);
        writeBuffer.push(BufferView(StringUtils::to_string(m_defaultErrorPage.size()).c_str()));
        writeBuffer.push("\r\n", 2);
        writeBuffer.push("Content-Type: text/html\r\n", 25);
		writeBuffer.push("\r\n", 2);
        
        m_fillFunction = &CgiResponse::mf_fillErrorDefaultPage;
        return (mf_fillErrorDefaultPage(writeBuffer));
	}

	Http::IOStatus::Type
	CgiResponse::mf_fillErrorFile(BaseBuffer& writeBuffer)
	{
		int      bytesRead;

        ASSERT_EQUAL(m_file.fd() != Ws::FD_NONE, true, "Response: File not open");
        
        bytesRead = writeBuffer.readAppend(m_file.fd(), writeBuffer.available());

        ASSERT_EQUAL(bytesRead >= 0, true, "Response: File read error");

        m_staticReadCounter += bytesRead;
        if (m_staticReadCounter == m_file.size())
        {
            m_file.close();
            m_fillFunction = &CgiResponse::mf_fillNothingToSend;
            return (Http::IOStatus::FINISHED);
        }
        return (Http::IOStatus::WRITING);
	}

	Http::IOStatus::Type
	CgiResponse::mf_fillErrorDefaultPage(BaseBuffer& writeBuffer)
    {
        size_t thisPush = std::min(writeBuffer.available(), m_defaultErrorPage.size() - m_staticReadCounter);
        writeBuffer.push(m_defaultErrorPage.c_str() + m_staticReadCounter, thisPush);
        m_staticReadCounter += thisPush;
        if (m_staticReadCounter == m_defaultErrorPage.size())
        {
            m_fillFunction = &CgiResponse::mf_fillNothingToSend;
            return (Http::IOStatus::FINISHED);
        }
        return (Http::IOStatus::WRITING);
    }
}
