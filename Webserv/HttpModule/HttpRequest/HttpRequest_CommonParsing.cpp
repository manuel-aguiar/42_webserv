/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"

# include <cstdlib> //atoi
# include  <algorithm> //max/min


static const BufferView contentLengthFind	("Content-Length");
static const BufferView contentTypeFind		("Content-Type");
static const BufferView transferEncodingFind("Transfer-Encoding");
static const BufferView multipartFind		("multipart/form-data");
static const BufferView multipartBoundaryFind("boundary=");
static const BufferView chunkedFind			("chunked");

#ifndef NDEBUG

	static int testHeadersOfInterest();

	static const int g_testHeadersOfInterest = testHeadersOfInterest();

	static int testHeadersOfInterest()
	{
		std::string test = contentLengthFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == contentLengthFind, true, "contentLengthFind is not correctly formated");
		test = contentTypeFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == contentTypeFind, true, "contentTypeFind is not correctly formated");
		test = transferEncodingFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == transferEncodingFind, true, "transferEncodingFind is not correctly formated");
		test = multipartFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() == multipartFind, true, "multipartFind is not correctly formated");
		test = multipartBoundaryFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() == multipartBoundaryFind, true, "multipartBoundaryFind is not correctly formated");
		
		return (0);
	}

#endif

namespace Http
{
    /*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Request::mf_handleNothing(const BufferView& remaining)
{
	if (m_parsingState != IDLE)
		return (remaining); // already started, do nothing

	m_parsingState = REQLINE;
	m_parsingFunction = &Request::mf_handleRequestLine;
	return (mf_handleRequestLine(remaining));
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseBodyExitError(const BufferView& remaining, const Http::Status::Number status)
{
	(void)remaining;
    m_parsingState = COMPLETED;
    m_data.status = status;

    if (m_httpResponse)
        m_httpResponse->receiveRequestBody(BufferView()); // send "eof" to signal end of body

    m_parsingFunction = &Request::mf_handleNothing;
    return (BufferView());
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Request::mf_handleRequestLine(const BufferView& receivedView)
{
	BufferView remaining = receivedView;

	if (remaining.size() == 0)
		return (BufferView());

	size_t reqLineEnd;

	BufferView target("\r\n", 2);
	reqLineEnd = remaining.find(target, m_findPivot);
	if (reqLineEnd == BufferView::npos)
	{
		m_findPivot = std::max((int)remaining.size() - (int)target.size(), 0);
		// HARD LIMIT, request line cannot be bigger than buffer size
		if (remaining.size() >= m_readBuffer->capacity())
		{
			m_parsingState = ERROR;
			m_data.status = Http::Status::URI_TOO_LONG;         // URI too long
			m_httpResponse->receiveRequestData(m_data);         // inform response right away
		}
		return (remaining); // not enough to go through yet
	}

	m_findPivot = 0;

	BufferView requestLine(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + 2, remaining.size() - reqLineEnd - 2); // move view forward

	m_data.status = mf_parseRequestLine(requestLine);							// call parser, error checking


	if (m_data.status != Http::Status::OK)
	{
		m_parsingState = ERROR;
		if (m_httpResponse)
			m_httpResponse->receiveRequestData(m_data);                 // inform response right away
		return (remaining);
	}

	// transition to headers
	m_parsingState = HEADERS;

	m_parsingFunction = &Request::mf_handleHeaders; // next handler is headers
	return (mf_handleHeaders(remaining));
}

BufferView
Request::mf_handleHeaders(const BufferView& receivedView)
{
	BufferView remaining = receivedView;
	BufferView delimiter("\r\n", 2);

	if (receivedView.size() == 0)
		return (BufferView()); // not enough to go through yet

	while (remaining.size() > 0)
	{
		size_t headerEnd = remaining.find(delimiter, m_findPivot);
		if (headerEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
			if (remaining.size() >= m_readBuffer->capacity())
				return mf_handleExitFailure(remaining, Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE);
			return (remaining); // push the remaining data back to the beginning
		}

		m_findPivot = 0;

		if (headerEnd == 0)
		{
			remaining = remaining.substr(delimiter.size(), remaining.size() - delimiter.size()); // move to body
			m_parsingState = BODY;

			if (m_httpResponse)
				m_httpResponse->receiveRequestData(m_data);

            return (Request::mf_prepareBodyParser(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd).trim(" \r\v\t\n"); // segregate this header

		remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move to next header

		m_data.status = mf_parseHeaders(thisHeader);
		if (m_data.status != Http::Status::OK)
			return mf_handleExitFailure(remaining, m_data.status);
	}

	return (remaining);
}

/*

    File Upload: multipart, has a route and a file name
    Content-Disposition: form-data; name="file"; filename="file.txt"


    POST /upload, sends a message body to /upload, but that is not an actual upload to a file.
    The response

*/
BufferView
Request::mf_prepareBodyParser(const BufferView& receivedView)
{
    RequestData::headerContainer::iterator contentLength = m_data.headers.find(contentLengthFind.to_string());
    RequestData::headerContainer::iterator transferEncoding = m_data.headers.find(transferEncodingFind.to_string());
    RequestData::headerContainer::iterator contentType = m_data.headers.find(contentTypeFind.to_string());
    bool isCgi = (m_httpResponse && m_httpResponse->getResponseData().responseType == Http::ResponseData::CGI);

	if (contentLength == m_data.headers.end() 
	&& transferEncoding == m_data.headers.end())
	{
		if (m_data.method == "POST")
			return (mf_parseBodyExitError(receivedView, Http::Status::LENGTH_REQUIRED));

		m_parsingState = COMPLETED;
		m_parsingFunction = &Request::mf_handleNothing;
		if (m_httpResponse)
			m_httpResponse->receiveRequestBody(BufferView()); // send empty buffer, end of message body
	}

    else if (contentLength != m_data.headers.end() && transferEncoding != m_data.headers.end())
	{
		return (mf_parseBodyExitError(receivedView, Http::Status::BAD_REQUEST));
	}

    else if (contentLength != m_data.headers.end())
    {
        m_curContentLength = std::atoi(contentLength->second.c_str());
        m_curContentPos = 0;
        m_parsingFunction = &Request::mf_parseLengthBody;
		if (contentType != m_data.headers.end()
		&& contentType->second.find(multipartFind.to_string()) != std::string::npos)
		{
			size_t boundaryPos = contentType->second.find(multipartBoundaryFind.to_string());
			if (boundaryPos == std::string::npos)
				return (mf_parseBodyExitError(receivedView, Http::Status::BAD_REQUEST));
			m_data.multipart_Boundary = contentType->second.substr(boundaryPos + 9);
			m_parsingFunction = &Request::mf_parseMultipartBody_Start;
			
			// if response is CGI, wait for it to signal that is time to parse
			if (isCgi)
				m_parsingFunction = &Request::mf_parseLengthBody; // cgi treats multipart as regular
		}
    }
    else if (transferEncoding->second == chunkedFind.to_string())
        m_parsingFunction = &Request::mf_parseChunkedBody_GetChunk;
    else
		return (mf_parseBodyExitError(receivedView, Http::Status::NOT_IMPLEMENTED));

	// if cgi stop the parsing, wait for cgi to signal
	if (isCgi)
		return (receivedView);

	return ((this->*m_parsingFunction)(receivedView));
}

void Request::mf_handleExitFailure(Http::Status::Number status)
{
    m_parsingState = ERROR;
    m_data.status = status;
    m_parsingFunction = &Request::mf_handleNothing;
    if (m_httpResponse) {
		m_httpResponse->receiveRequestData(m_data);
	}

	return;
}

BufferView Request::mf_handleExitFailure(BufferView& remaining, Http::Status::Number status)
{
	mf_handleExitFailure(status);
	return (remaining);
}
}
