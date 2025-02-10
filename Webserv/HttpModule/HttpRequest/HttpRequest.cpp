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


#define METHOD_GET 0
#define METHOD_POST 1
#define METHOD_DELETE 2

class HttpResponse;

int HttpRequest::generateResponse()
{
	HttpResponse response;
	std::map<std::string, std::string>::iterator it = m_uriComponents.find("path");

	// If there were errors during parsing, respond with error page
	if (m_errorStatus != Http::Status::OK)
		// Set response to m_errorStatus, retrieve error page data into response.
		response.load(m_errorStatus);

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


	switch (requestMethod)
	{
		case METHOD_GET:
			if (/* Method not alowed */)
				response.load(Http::Status::METHOD_NOT_ALLOWED);
			// Check if path is a file
			if (FilesUtils::isFile(it->second.c_str()))
				// read file into buffer
				response.load(Http::Status::OK, it->second.c_str());
			// Check if path is a directory
			else if (FilesUtils::isDirectory(it->second.c_str()))
				response.load(Http::Status::FORBIDDEN);
			else
				response.load(Http::Status::NOT_FOUND);
			break;
		case METHOD_POST:
			if (/* Method not alowed */)
				response.load(Http::Status::METHOD_NOT_ALLOWED);
			break;
		case METHOD_DELETE:
			if (/* Method not alowed */)
				response.load(Http::Status::METHOD_NOT_ALLOWED);
			/* DELETE CASES
				200 OK
				204 NO_CONTENT
				404 NOT_FOUND
				405 METHOD_NOT_ALLOWED
			*/
			break;
		default:
			// 501 NOT IMPLEMENTED or 405 METHOD NOT ALLOWED (normally used in implemented methods)
			break;
	}
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
