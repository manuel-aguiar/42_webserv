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
		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size() + 1
			+ 3 + 1
			+ std::strlen(getStatusMessage(m_responseData.requestStatus)) + 2;

		if (writeBuffer.available() < writeSize)
			return (Http::ResponseStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(StringUtils::to_string(m_responseData.requestStatus).c_str(), 3); // always 3 digits status code
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

		while (m_currentHeader != m_responseData.headers.end())
		{
			ASSERT_EQUAL(m_currentHeader->first.empty() || m_currentHeader->second.empty(), false, "Response::mf_fillHeaders: incomplete header");

			std::string	header = m_currentHeader->first + ": " + m_currentHeader->second + "\r\n";
			writeSize = header.size() + 2;

			if (writeBuffer.available() < writeSize)
				return (Http::ResponseStatus::WRITING);

			writeBuffer.push(header.c_str(), header.size());
			m_currentHeader++;
		}

		if (writeBuffer.available() < 2)
			return (Http::ResponseStatus::WRITING);
		writeBuffer.push("\r\n", 2);

		// If no body, we're done
		if (m_fillFunctionBody == NULL)
			return (mf_fillFinish());

		// go to next stage
		m_fillFunction = m_fillFunctionBody;
		return (Http::ResponseStatus::WRITING);
	}

    Http::ResponseStatus::Type
    Response::mf_fillRedirect(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        std::string codeStr = StringUtils::to_string(m_responseData.requestStatus);
        std::string redirPage = mf_generateRedirectPage(m_responseData.requestStatus, m_responseData.headers["location"]);

        size_t writeSize = Http::HttpStandard::HTTP_VERSION.size()
		+ StringUtils::to_string(m_responseData.requestStatus).size() + 1
		+ std::strlen(getStatusMessage(m_responseData.requestStatus)) + 2
		+ 15 + StringUtils::to_string(redirPage.size()).size() + 2
		+ 19 + std::strlen("Content-Type: text/html\r\n") + 2
		+ redirPage.size();

        if (writeBuffer.available() < writeSize)
            return (Http::ResponseStatus::WRITING);

        writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
        writeBuffer.push(codeStr.c_str(), codeStr.size());
        writeBuffer.push(" ", 1);
        writeBuffer.push(getStatusMessage(m_responseData.requestStatus), std::strlen(getStatusMessage(m_responseData.requestStatus)));
        writeBuffer.push("\r\n", 2);
        writeBuffer.push("Content-Length: ", 15);
        writeBuffer.push(StringUtils::to_string(redirPage.size()).c_str(), StringUtils::to_string(redirPage.size()).size());
        writeBuffer.push("\r\n", 2);
        writeBuffer.push("Content-Type: text/html\r\n", std::strlen("Content-Type: text/html\r\n"));
        writeBuffer.push("\r\n", 2);
        writeBuffer.push(redirPage.c_str(), redirPage.size());
        return (mf_fillFinish());
    }

	Http::ResponseStatus::Type
	Response::mf_fillDefaultPage(BaseBuffer& writeBuffer)
	{
		size_t writeSize = m_defaultPageContent.size();

		if (writeBuffer.available() < writeSize)
			return (Http::ResponseStatus::WRITING);

		writeBuffer.push(m_defaultPageContent.c_str(), m_defaultPageContent.size());

		return (mf_fillFinish());
	}

    Http::ResponseStatus::Type
    Response::mf_fillCgiResponse(BaseBuffer& writeBuffer)
    {
        ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response: CgiResponse not set");
        return (m_cgiResponse->fillWriteBuffer(writeBuffer));
    }

	Http::ResponseStatus::Type
	Response::mf_fillFinish()
	{
		if (m_responseData.closeAfterSending == true)
			return (Http::ResponseStatus::MARK_TO_CLOSE);
		else
			return (Http::ResponseStatus::FINISHED);
	}
}