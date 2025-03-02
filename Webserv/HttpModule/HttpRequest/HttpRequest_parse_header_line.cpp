/* *********************************/
/*                                 */
/*   http_parse_header_line.cpp    */
/*   - the header line parser for  */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

// Project headers
#include "HttpRequest.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <sstream>

// Edge cases:
// split \r \n between two reads "http/1.1 \r" + "\n"
// "http " + "1.1 \r\n"
// add a filter for headers of interest here
// probably add a byte buffer to cache intermediary leftover data
// ideally, nginx way of having multiple buffers


// headers we care, all others are discarded

namespace Http
{
	// must be sorted or WILL ABORT
	static const char* headersOfInterest[] = 
	{
		"accept",
		"accept-encoding",
		"accept-language",
		"connection",
		"content-length",
		"content-type",
		"host",
		"proxy-connection",
		"transfer-encoding",
	};
}

static bool isOnlySpaces(const BufferView& view)
{
	for (size_t i = 0; i < view.size(); ++i)
	{
		if (view[i] != ' ' && view[i] != '\t')
			return (false);
	}

	return (true);
}

static bool keyIsValid(const BufferView& key)
{
	if (key.size() == 0)
		return (false);

	if (isOnlySpaces(key))
		return (false);

	// more validation stuff here

	return (true);
}

static bool valueIsValid(const BufferView& key)
{
	if (key.size() == 0)
		return (false);

	if (isOnlySpaces(key))
		return (false);

	// more validation stuff here

	return (true);
}


// binary search into headersOfInterest to see if we find our target
static int binSearch(const char** lookup, size_t sizeOfLookup, const BufferView& target)
{
	int        		low = 0;
	int        		high = sizeOfLookup - 1;
	int        		mid;
	BufferView 		midView;

	if (sizeOfLookup <= 0)
		return (-1);

	while (low <= high)
	{
		mid = low + ((high - low) / 2);
		midView = BufferView(lookup[mid]);
		if (midView == target)
			return (mid);
		else if (midView > target)
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (target != BufferView(lookup[low]))
		return (-1);

	return (low);
}

Http::Status::Number
Http::Request::mf_parseHeaders(const BufferView &thisHeader)
{
	#ifndef NDEBUG
		// check if headersOfInterest is sorted and unique
		for (size_t i = 1; i < sizeof(Http::headersOfInterest) / sizeof(Http::headersOfInterest[0]); ++i)
			ASSERT_EQUAL(BufferView(Http::headersOfInterest[i]) > BufferView(Http::headersOfInterest[i - 1]), true, "headersOfInterest are repeated/not sorted");
	#endif

	size_t colonPos = thisHeader.find(": ");

	if (colonPos == BufferView::npos)
		return (Http::Status::BAD_REQUEST);					// bad header format, correct error code?

	BufferView key = thisHeader.substr(0, colonPos);
	BufferView value = thisHeader.substr(colonPos + 2, thisHeader.size() - colonPos - 2);

	if (!keyIsValid(key) || !valueIsValid(value))			
		return (Http::Status::BAD_REQUEST);					// bad key or value, replace with correct error code

	std::string lowerCaseKey = StringUtils::strToLower(key.to_string());

	int index = binSearch(Http::headersOfInterest, sizeof(Http::headersOfInterest) / sizeof(Http::headersOfInterest[0]), 
				BufferView(lowerCaseKey));
	
	if (index == -1)
		return (Http::Status::OK);							// header not in the interest list, ignore and return

	m_data.headers[lowerCaseKey] = value.to_string();			// dangerous, may include null bytes.........

	return (Http::Status::OK);
}
