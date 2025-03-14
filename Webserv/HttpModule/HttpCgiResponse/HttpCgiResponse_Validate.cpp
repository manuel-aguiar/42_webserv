

# include "HttpCgiResponse.hpp"

// C++ headers
# include <vector>

static const BufferView forbiddenHeaders[] =
{
	"Connection",
	"Transfer-Encoding",
};

static const BufferView ignoredHeaders[] =
{
	"Content-Length",
};

#ifndef NDEBUG
	static int testHeadersOfInterest();
	static const int g_testHeadersOfInterest = testHeadersOfInterest();
	static int testHeadersOfInterest()
	{
		for (size_t i = 0; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
		{
			if (i > 0)
				ASSERT_EQUAL(forbiddenHeaders[i] > forbiddenHeaders[i - 1], true, "forbidden headers must be sorted and have unique values");
			std::string copy = forbiddenHeaders[i].to_string();
			ASSERT_EQUAL(BufferView(copy).trim(" \r\n\t\v").modify_ToCapitalized() == forbiddenHeaders[i], true, "forbidden headers are not correctly formated");
		}
		for (size_t i = 0; i < sizeof(ignoredHeaders) / sizeof(ignoredHeaders[0]); ++i)
		{
			if (i > 0)
				ASSERT_EQUAL(ignoredHeaders[i] > ignoredHeaders[i - 1], true, "ignored headers must be sorted and have unique values");
			std::string copy = ignoredHeaders[i].to_string();
			ASSERT_EQUAL(BufferView(copy).trim(" \r\n\t\v").modify_ToCapitalized() == ignoredHeaders[i], true, "ignored headers are not correctly formated");
		}
		return (0);
	}
#endif

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
		else if (midView < lookup[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (target != lookup[low])
		return (-1);

	return (low);
}

bool checkForbiddenHeaders(const std::vector<Cgi::Header>& headers)
{
	// confirm lookups are sorted for binary search


	for (size_t i = 0; i < headers.size(); ++i)
	{
		if (binSearch(forbiddenHeaders, sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]), headers[i].key) != -1)
			return (false);
	}
	return (true);
}

bool isHeaderIgnored(const Cgi::Header& header)
{
	return (binSearch(ignoredHeaders, sizeof(ignoredHeaders) / sizeof(ignoredHeaders[0]), header.key) != -1);
}
