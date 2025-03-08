

# include "HttpResponse.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

namespace Http
{
	bool
	Response::mf_prepareStaticFile(const char* path)
	{
		ASSERT_EQUAL(m_file.fd() == Ws::FD_NONE, true, "Response: File should not open");

		if (!m_file.open(path, O_RDONLY, 0666))
		{
			// We need to think this through
			m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
			m_responseData.responseType = ResponseData::ERROR;
			m_responseData.errorMessage = "Could not open file";
			m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, m_responseData.errorMessage);
			m_fillFunctionBody = &Response::mf_fillDefaultPage;
			return (false);
		}

		m_fillFunction = &Response::mf_fillResponseLine;
		return (true);
	}

	Http::IOStatus::Type
	Response::mf_sendStaticFile(BaseBuffer& writeBuffer)
	{
		int      bytesRead;

        ASSERT_EQUAL(m_file.fd() != Ws::FD_NONE, true, "Response: File not open");
        
        bytesRead = writeBuffer.readAppend(m_file.fd(), writeBuffer.available());

        ASSERT_EQUAL(bytesRead >= 0, true, "Response: File read error");

        m_staticReadCounter += bytesRead;

        if (m_staticReadCounter == m_file.size())
        {
            m_file.close();
            m_fillFunction = &Response::mf_fillNothingToSend;
            return (Http::IOStatus::FINISHED);
        }
        return (Http::IOStatus::WRITING);
	}
}
