# include "HttpResponse.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

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
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillHeaders(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillRedirect(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        // writeBuffer.push(errorPage.c_str(), errorPage.size());
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillErrorResponse(BaseBuffer& writeBuffer)
    {

        // NOT IMPLEMENTED YET





        (void)writeBuffer;

        std::string codeStr = StringUtils::to_string(m_responseData.requestStatus);
        std::string errorPage = mf_generateDefaultErrorPage(m_responseData.requestStatus, "YOU SUCK");

		writeBuffer.push("HTTP/1.1 ", 9);
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
}
