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
#include "../../../Toolkit/Assert/StaticAssert/StaticAssert.h"
// C++ headers

namespace Http
{
	// must be sorted or WILL ABORT
	static const BufferView headersOfInterest[] = 
	{
		"Accept",
		"Accept-Encoding",
		"Accept-Language",
		"Connection",
		"Content-Length",
		"Content-Type",
		"Host",
		"If-Modified-Since",
		"If-None-Match",
		"Proxy-Connection",
		"Transfer-Encoding",
	};

	#ifndef NDEBUG

		static int testHeadersOfInterest();

		static const int g_testHeadersOfInterest = testHeadersOfInterest();

		static int testHeadersOfInterest()
		{
			for (size_t i = 0; i < sizeof(Http::headersOfInterest) / sizeof(Http::headersOfInterest[0]); ++i)
			{
				if (i > 0)
					ASSERT_EQUAL(Http::headersOfInterest[i] > Http::headersOfInterest[i - 1], true, "headersOfInterest are repeated/not sorted");
				
				std::string copy = Http::headersOfInterest[i].to_string();
				ASSERT_EQUAL(BufferView(copy).trim(" \t\v\n\r").modify_ToCapitalized() == Http::headersOfInterest[i], 
				true, "headersOfInterest is not correctly formated, must have no leading/trailing spaces and be capitalized");
			}
			return (0);
		}
	#endif

}


static bool keyIsValid(const BufferView& key)
{
	if (key.size() == 0)
		return (false);

	if (key.isOnlyTheseChars(" \t"))
		return (false);

	// more validation stuff here

	return (true);
}

static bool valueIsValid(const BufferView& key)
{
	if (key.size() == 0)
		return (false);

	if (key.isOnlyTheseChars(" \t"))
		return (false);

	// more validation stuff here

	return (true);
}


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

Http::Status::Number
Http::Request::mf_parseHeaders(const BufferView &thisHeader)
{
	size_t colonPos = thisHeader.find(": ");

	if (colonPos == BufferView::npos)
		return (Http::Status::BAD_REQUEST);					// bad header format, correct error code?

	BufferView key = thisHeader.substr(0, colonPos).trim("\t\v\r\n ");
	BufferView value = thisHeader.substr(colonPos + 2, thisHeader.size() - colonPos - 2).trim("\t\v\r\n ");

	if (!keyIsValid(key) || !valueIsValid(value))			
		return (Http::Status::BAD_REQUEST);					// bad key or value, replace with correct error code

	key.modify_ToCapitalized();								// capitalize the key

	int index = binSearch(Http::headersOfInterest, sizeof(Http::headersOfInterest) / sizeof(Http::headersOfInterest[0]), 
				key);
	
	if (index == -1)
		return (Http::Status::OK);							// header not in the interest list, ignore and return

	m_data.headers[key.to_string()] = value.to_string();			// dangerous, may include null bytes.........

	return (Http::Status::OK);
}
