/* *********************************/
/*                                 */
/*   http_parse_request_line.cpp   */
/*   - the request line parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

// Project headers
#include "HttpRequest.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

namespace Http
{
    static BufferView allowedMethods[] = {
        BufferView("DELETE"),
        BufferView("GET"),
        BufferView("POST")
    };
}

#ifndef NDEBUG

    static int testHeadersOfInterest();

    static const int g_testHeadersOfInterest = testHeadersOfInterest();

    static int testHeadersOfInterest()
    {
        for (size_t i = 0; i < sizeof(Http::allowedMethods) / sizeof(Http::allowedMethods[0]); ++i)
        {
            if (i > 0)
                ASSERT_EQUAL(Http::allowedMethods[i] > Http::allowedMethods[i - 1], true, "allowedmethods are repeated/not sorted");
            
            std::string copy = Http::allowedMethods[i].to_string();
            ASSERT_EQUAL(BufferView(copy).trim(" \t\v\n\r").modify_ToUpperCase() == Http::allowedMethods[i], 
            true, "allowedmethods is not correctly formated, must have no leading/trailing spaces and be capitalized");
        }
        return (0);
    }

#endif

// binary search into headersOfInterest to see if we find our target
static int binSearch(const BufferView lookup[], size_t sizeOfLookup, const BufferView& target)
{
	int        		low = 0;
	int        		high = sizeOfLookup - 1;
	int        		mid;
	BufferView 		midView;

	if (sizeOfLookup <= 0)
		return (-1);

	while (low < high)
	{
		mid = low + ((high - low) / 2);
		midView = lookup[mid];
		if (midView == target)
			return (mid);
		else if (midView > target)
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (target != lookup[low])
		return (-1);

	return (low);
}

// URL decoding utilities
static int hexToInt(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

// not confident
std::string Http::Request::mf_decodeUriComp(const std::string& encoded) const
{
    std::string decodedString;

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 >= encoded.length())
                throw EncodingException("Invalid URL encoding");

            // not confident
            int hi = hexToInt(encoded[i + 1]);
            int lo = hexToInt(encoded[i + 2]);
            if (hi == -1 || lo == -1)
                throw EncodingException("Invalid URL encoding");

            decodedString += static_cast<char>((hi << 4) | lo);
            i += 2;
            continue;
        }

        decodedString += encoded[i];
    }
    return decodedString;
}

// URI parsing utilities
Http::Status::Number
Http::Request::mf_parseUriComponents(const std::string& uri)
{
    size_t queryStart = uri.find('?');
    size_t fragmentStart = uri.find('#');

    // get path
    size_t pathStart = queryStart != std::string::npos ? queryStart : fragmentStart;
    m_data.path = mf_decodeUriComp(uri.substr(0, pathStart));

    // get query string
    if (queryStart != std::string::npos) {
        size_t queryEnd = (fragmentStart != std::string::npos) ? fragmentStart : uri.length();
        m_data.queryString = uri.substr(queryStart + 1, queryEnd - (queryStart + 1));
    }

    // get fragment
    if (fragmentStart != std::string::npos) {
        m_data.fragment = uri.substr(fragmentStart + 1);
    }

    return Http::Status::OK;
}

Http::Status::Number
Http::Request::mf_parseRequestLine(const BufferView& line)
{
    std::string lineString;
    line.to_string(lineString);

    try {
        size_t firstSpace = lineString.find(' ');
        size_t lastSpace = lineString.rfind(' ');

        // this check is important because it ensures that the request line is valid
        // and that it has at least three components
        if (firstSpace == std::string::npos || \
            lastSpace == std::string::npos || \
            firstSpace == lastSpace)
            return Http::Status::BAD_REQUEST;

        // also only one space between components
        if (lineString[firstSpace + 1] == ' ' || lineString[lastSpace - 1] == ' ')
            return Http::Status::BAD_REQUEST;

        m_data.method = lineString.substr(0, firstSpace);
        m_data.uri = lineString.substr(firstSpace + 1, lastSpace - firstSpace - 1);
        m_data.httpVersion = lineString.substr(lastSpace + 1);

        // line.substr(0, firstSpace).to_string(m_method);
        // line.substr(firstSpace + 1, lastSpace - firstSpace - 1).to_string(m_uri);
        // line.substr(lastSpace + 1).to_string(m_httpVersion);

        // Check if method is allowed using HttpDefinitions
        // TODO: later get this from server config

        if (binSearch(allowedMethods, sizeof(allowedMethods) / sizeof(allowedMethods[0]), m_data.method) == -1)
            return Http::Status::METHOD_NOT_ALLOWED;

        // TODO: later get this from server config
        if (m_data.uri.length() > Http::HttpStandard::MAX_URI_LENGTH)
            return Http::Status::URI_TOO_LONG;

        if (m_data.httpVersion != Http::HttpStandard::HTTP_VERSION)
            return Http::Status::HTTP_VERSION_NOT_SUPPORTED;

        return mf_parseUriComponents(m_data.uri);
    }
    catch (const EncodingException& e) {
        return Http::Status::BAD_REQUEST;
    }
    catch (const std::exception& e) {
        return Http::Status::INTERNAL_ERROR;
    }
}
