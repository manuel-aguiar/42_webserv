

# include "HttpCgiGateway.hpp"

static const char* forbiddenHeaders[] =
{
	"Connection"
	"Transfer-Encoding",
};

static const char* ignoredHeaders[] =
{
	"Content-Length",
};

int binSearch(const char** lookup, size_t size, const BufferView& target)
{
	int        		low = 0;
	int        		high = size - 1;
	int        		mid;
	BufferView 		midView;

	if (size <= 0)
		return (-1);

	while (low < high)
	{
		mid = low + ((high - low) / 2);
		midView = BufferView(lookup[mid]);
		if (midView == target)
			return (mid);
		else if (midView < lookup[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (target != BufferView(lookup[low]))
		return (-1);

	return (low);
}

bool validateHeaders(std::vector<Cgi::Header>& headers)
{
	// confirm lookups are sorted for binary search
	#ifndef NDEBUG
		for (size_t i = 1; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
			ASSERT_EQUAL(forbiddenHeaders[i] > forbiddenHeaders[i - 1], true, "forbidden headers must be sorted and have unique values");
		for (size_t i = 1; i < sizeof(ignoredHeaders) / sizeof(ignoredHeaders[0]); ++i)
			ASSERT_EQUAL(ignoredHeaders[i] > ignoredHeaders[i - 1], true, "ignored headers must be sorted and have unique values");
	#endif

	for (size_t i = 0; i < headers.size(); ++i)
	{
		if (binSearch(forbiddenHeaders, sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]), headers[i].key) != -1)
			return (false);
		if (binSearch(ignoredHeaders, sizeof(ignoredHeaders) / sizeof(ignoredHeaders[0]), headers[i].key) != -1)
		{
			// "remove" the key/value
			headers[i].key = BufferView();
			headers[i].value = BufferView();
		}
	}
	return (true);
}



