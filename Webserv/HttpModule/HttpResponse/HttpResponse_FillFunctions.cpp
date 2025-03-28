# include "HttpResponse.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include <cstring>
extern BufferView getStatusMessage(Http::Status::Number statusCode);

namespace Http
{
    Http::IOStatus::Type
    Response::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Http::IOStatus::WAITING);
	}

	Http::IOStatus::Type
	Response::mf_fillResponseLine(BaseBuffer& writeBuffer)
	{
		const BufferView statusMessage = getStatusMessage(m_responseData.requestStatus);
		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size() + 1
			+ 3 + 1
			+ statusMessage.size() + 2;

		if (writeBuffer.available() < writeSize)
			return (Http::IOStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(StringUtils::to_string(m_responseData.requestStatus).c_str(), 3); // always 3 digits status code
		writeBuffer.push(" ", 1);
		writeBuffer.push(statusMessage);
		writeBuffer.push("\r\n", 2);

		// go to next stage
		m_fillFunction = &Response::mf_fillHeaders;
		return ((this->*m_fillFunction)(writeBuffer));
    }

    Http::IOStatus::Type
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
				return (Http::IOStatus::WRITING);

			writeBuffer.push(m_currentHeader->first.c_str(), m_currentHeader->first.size());
			writeBuffer.push(": ", 2);
			writeBuffer.push(m_currentHeader->second.c_str(), m_currentHeader->second.size());
			writeBuffer.push("\r\n", 2);
			m_currentHeader++;
		}

		if (writeBuffer.available() < 2)
			return (Http::IOStatus::WRITING);
		writeBuffer.push("\r\n", 2);

		ASSERT_EQUAL(m_fillFunction == NULL, false, "Response::mf_fillHeaders: m_fillFunction is NULL");
		ASSERT_EQUAL(m_fillBody == NULL, false, "Response::mf_fillHeaders: m_fillBody is NULL");

		// go to next stage
		m_fillFunction = m_fillBody;
		return ((this->*m_fillFunction)(writeBuffer));
	}

    Http::IOStatus::Type
    Response::mf_fillRedirect(BaseBuffer& writeBuffer)
    {
		if (writeBuffer.capacity() < m_defaultPageContent.size())
			return (Http::IOStatus::MARK_TO_CLOSE);

        if (writeBuffer.available() < m_defaultPageContent.size())
            return (Http::IOStatus::WAITING);

        writeBuffer.push(m_defaultPageContent.c_str(), m_defaultPageContent.size());
        return (mf_fillFinish(writeBuffer));
    }

	Http::IOStatus::Type
	Response::mf_fillDefaultPage(BaseBuffer& writeBuffer)
	{
		size_t writeSize = m_defaultPageContent.size();

		if (writeBuffer.available() < writeSize)
		{
			std::string	pageChunk = m_defaultPageContent.substr(0, writeBuffer.available());
			writeBuffer.push(pageChunk.c_str(), pageChunk.size());
			m_defaultPageContent = m_defaultPageContent.substr(pageChunk.size());
			return (Http::IOStatus::WRITING);
		}
		else
			writeBuffer.push(m_defaultPageContent.c_str(), m_defaultPageContent.size());

		return (mf_fillFinish(writeBuffer));
	}

	Http::IOStatus::Type
	Response::mf_fillExpectContinue(BaseBuffer& writeBuffer)
	{
		std::map<std::string, std::string>::const_iterator expectHeader =
		m_responseData.requestData->headers.find("Expect");

		const BufferView expectMessage = getStatusMessage(Http::Status::CONTINUE);

		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size() + 1
			+ 3 + 1
			+ expectMessage.size() + 2;

		if (expectHeader == m_responseData.requestData->headers.end())
			goto waitUpload;
			
		if (writeBuffer.available() < writeSize)
			return (Http::IOStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(StringUtils::to_string(Http::Status::CONTINUE).c_str(), 3); // always 3 digits status code
		writeBuffer.push(" ", 1);
		writeBuffer.push(expectMessage);
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Content-Length: 0\r\n", 19);
		writeBuffer.push("\r\n", 2);

	waitUpload:
		m_fillFunction = &Response::mf_fillNothingToSend;
		return (Http::IOStatus::WRITING);
	}

	Http::IOStatus::Type
	Response::mf_fillExpectFail(BaseBuffer& writeBuffer)
	{
		std::map<std::string, std::string>::const_iterator expectHeader =
		m_responseData.requestData->headers.find("Expect");
		const BufferView expectMessage = getStatusMessage(Http::Status::EXPECTATION_FAIL);
		size_t writeSize = Http::HttpStandard::HTTP_VERSION.size() + 1
			+ 3 + 1
			+ expectMessage.size() + 2;

		if (expectHeader == m_responseData.requestData->headers.end())
			goto waitUpload;
			
		if (writeBuffer.available() < writeSize)
			return (Http::IOStatus::WRITING);

		writeBuffer.push(Http::HttpStandard::HTTP_VERSION.c_str(), Http::HttpStandard::HTTP_VERSION.size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(StringUtils::to_string(Http::Status::EXPECTATION_FAIL).c_str(), 3); // always 3 digits status code
		writeBuffer.push(" ", 1);
		writeBuffer.push(expectMessage);
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Content-Length: 0\r\n", 19);
		writeBuffer.push("\r\n", 2);
		
	waitUpload:
		m_fillFunction = &Response::mf_fillNothingToSend;
		return (Http::IOStatus::WRITING);
	}

    Http::IOStatus::Type
    Response::mf_fillCgiResponse(BaseBuffer& writeBuffer)
    {
        ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response: CgiResponse not set");
        return (m_cgiResponse->fillWriteBuffer(writeBuffer));
    }

	Http::IOStatus::Type
	Response::mf_fillFinish(BaseBuffer& writeBuffer)
	{
		(void)writeBuffer;
		if (m_responseData.closeAfterSending == true)
			return (Http::IOStatus::MARK_TO_CLOSE);
		else
			return (Http::IOStatus::FINISHED);
	}
}
