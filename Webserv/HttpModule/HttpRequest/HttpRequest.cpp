/* *********************************/
/*                                 */
/*   HttpRequest.cpp               */
/*   - implements the HttpRequest  */
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
    : m_status(Http::Status::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_httpConn(NULL)
    , m_session(NULL)
{}

HttpRequest::~HttpRequest()
{}

int HttpRequest::parse(const std::string& rawData)
{
    // Reset state before parsing
    m_status = Http::Status::OK;

    try {
        size_t pos = rawData.find("\r\n"); // goes until the CRLF
        if (pos == std::string::npos)
            return (Http::Status::BAD_REQUEST);

        // Parse request line
        m_status = mf_parseRequestLine(rawData.substr(0, pos));
        if (m_status != Http::Status::OK)
            return m_status;

        // TODO: check if logic below breaks
        // Parse headers
        size_t headerStart = pos + 2;
        pos = rawData.find("\r\n\r\n", headerStart);
        if (pos == std::string::npos)
            return (Http::Status::BAD_REQUEST);

        // Parse headers section
        m_status = mf_parseHeaders(rawData.substr(headerStart, pos - headerStart));
        if (m_status != Http::Status::OK)
            return m_status;

        // On POST request, parse the body
        if (m_method == "POST") {
            size_t bodyStart = pos + 4;
            m_status = mf_parseBody(rawData.substr(bodyStart));
        }

        return m_status;
    }
    catch (const std::exception& e) {
        return Http::Status::INTERNAL_ERROR;
    }
}

// Move this to a wider scope
#define METHOD_GET 0
#define METHOD_POST 1
#define METHOD_DELETE 2

int HttpRequest::generateResponse()
{
	// get the actual thing for the values
	Http::Response response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context);

	// Convert method strings into numbers for switch (could be done at parsing)
	int requestMethod;
	if (m_method == "GET")
		requestMethod = METHOD_GET;
	else if (m_method == "POST")
		requestMethod = METHOD_POST;
	else if (m_method == "DELETE")
		requestMethod = METHOD_DELETE;
	else
		requestMethod = 100;
	
	// Check if there is a path on the request (should be done at parsing)
	if (it == m_uriComponents.end())
		response.load(Http::Status::BAD_REQUEST);

	return response;
}

// Getters
const std::string& HttpRequest::getMethod() const
{
    return m_method;
}

const std::string& HttpRequest::getUri() const
{
    return m_uri;
}

const std::string& HttpRequest::getHttpVersion() const
{
    return m_httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return m_headers;
}

const std::string& HttpRequest::getBody() const
{
    return m_body;
}

const std::map<std::string, std::string>& HttpRequest::getUriComponents() const
{
    return m_uriComponents;
}
