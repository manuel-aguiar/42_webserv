

# include "HttpResponse.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

namespace Http
{

    Http::ResponseStatus::Type
	Response::mf_prepareStaticFile(BaseBuffer& writeBuffer)
    {
        ASSERT_EQUAL(m_file.fd() == Ws::FD_NONE, true, "Response: File should not open");

        if (!m_file.open(m_responseData.targetPath.c_str(), O_RDONLY, 0666))
        {
            m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
            m_fillFunction = &Response::mf_fillErrorResponse;
            return (Http::ResponseStatus::FINISHED);
        }

        m_fillFunction = &Response::mf_sendStaticFile;
        return (mf_sendStaticFile(writeBuffer));
    }

	Http::ResponseStatus::Type
	Response::mf_sendStaticFile(BaseBuffer& writeBuffer)
	{
		int      bytesRead;

        ASSERT_EQUAL(m_file.fd() != Ws::FD_NONE, true, "Response: File not open");
        
        bytesRead = writeBuffer.readAppend(m_file.fd());

        ASSERT_EQUAL(bytesRead >= 0, true, "Response: File read error");

        m_staticReadCounter += bytesRead;

        if (m_staticReadCounter == m_file.size())
        {
            m_file.close();
            m_fillFunction = &Response::mf_fillNothingToSend;
            return (Http::ResponseStatus::FINISHED);
        }
        return (Http::ResponseStatus::WRITING);
	}
}
