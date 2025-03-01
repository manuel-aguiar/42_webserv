

# include "HttpResponse.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include <cstring>
extern const char* getStatusMessage(int statusCode);

namespace Http
{
    Http::ResponseStatus::Type
    Response::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
	}

	Http::ResponseStatus::Type
	Response::mf_fillResponseLine(BaseBuffer& writeBuffer)
	{
		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size()
			+ StringUtils::to_string(m_responseData.requestStatus).size() + 1
			+ std::strlen(getStatusMessage(m_responseData.requestStatus)) + 2;

		if (writeBuffer.available() < writeSize)
			return (Http::ResponseStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(StringUtils::to_string(m_responseData.requestStatus).c_str(), StringUtils::to_string(m_responseData.requestStatus).size()); // always 3 digits	
		writeBuffer.push(" ", 1);
		writeBuffer.push(getStatusMessage(m_responseData.requestStatus), std::strlen(getStatusMessage(m_responseData.requestStatus)));
		writeBuffer.push("\r\n", 2);

        // go to next stage
        m_fillFunction = &Response::mf_fillHeaders;

        return (Http::ResponseStatus::WRITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillHeaders(BaseBuffer& writeBuffer)
    {
		ASSERT_EQUAL(m_responseData.headers.empty(), false, "Response::mf_fillHeaders: m_responseData.headers is empty");

		size_t writeSize = 0;

		if (m_currentHeader == NULL)
			*m_currentHeader = m_responseData.headers.begin();

		while (*m_currentHeader != m_responseData.headers.end())
		{
			ASSERT_EQUAL((*m_currentHeader)->first.empty() || (*m_currentHeader)->second.empty(), false, "Response::mf_fillHeaders: incomplete header");

			std::string	header = (*m_currentHeader)->first + ": " + (*m_currentHeader)->second;
			writeSize = header.size();

			if (writeBuffer.available() < writeSize)
				return (Http::ResponseStatus::WRITING);

			writeBuffer.push(header.c_str(), header.size());
			(*m_currentHeader)++;
		}

		// If no body, we're done
		if (m_fillFunctionBody == NULL)
			return (Http::ResponseStatus::FINISHED);

		// go to next stage
		m_fillFunction = m_fillFunctionBody;
        return (Http::ResponseStatus::WRITING);
    }	

    Http::ResponseStatus::Type
    Response::mf_fillErrorResponse(BaseBuffer& writeBuffer)
    {

        // NOT IMPLEMENTED YET

        std::string codeStr = StringUtils::to_string(m_responseData.requestStatus);
        std::string errorPage = mf_generateDefaultErrorPage(m_responseData.requestStatus, "YOU SUCK");

		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size()
		+ StringUtils::to_string(m_responseData.requestStatus).size() + 1
		+ std::strlen(getStatusMessage(m_responseData.requestStatus)) + 2
		+ 15 + StringUtils::to_string(errorPage.size()).size() + 2
		+ 19 + std::strlen("Content-Type: text/html\r\n") + 2
		+ errorPage.size();

		if (writeBuffer.available() < writeSize)
			return (Http::ResponseStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(codeStr.c_str(), codeStr.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(getStatusMessage(m_responseData.requestStatus));
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

	Http::ResponseStatus::Type
	Response::mf_fillDirectoryListing(BaseBuffer& writeBuffer)
	{
		(void)writeBuffer;
		return (Http::ResponseStatus::WAITING);
	}

    Http::ResponseStatus::Type
    Response::mf_fillCgiResponse(BaseBuffer& writeBuffer)
    {
        ASSERT_EQUAL(m_cgiGateway != NULL, true, "Response: CgiGateway not set");
        return (m_cgiGateway->fillWriteBuffer(writeBuffer));
    }
}
