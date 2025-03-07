
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include "HttpResponse.hpp"

static const std::pair<BufferView, BufferView> mimeTypeMap[] = 
{
	std::pair<BufferView, BufferView>(".css",		"text/css"),
	std::pair<BufferView, BufferView>(".gif",		"image/gif"),
	std::pair<BufferView, BufferView>(".htm",		"text/html"),
	std::pair<BufferView, BufferView>(".html",		"text/html"),
	std::pair<BufferView, BufferView>(".ico",		"image/x-icon"),
	std::pair<BufferView, BufferView>(".jpeg",	 	"image/jpeg"),
	std::pair<BufferView, BufferView>(".jpg",		"image/jpeg"),
	std::pair<BufferView, BufferView>(".js",		"text/javascript"),
	std::pair<BufferView, BufferView>(".json",	 	"application/json"),
	std::pair<BufferView, BufferView>(".png",		"image/png"),
	std::pair<BufferView, BufferView>(".txt",		"text/plain"),
	std::pair<BufferView, BufferView>(".xml",		"text/xml")
};

#ifndef NDEBUG

	static int testMimeTypeMap();

	static const int g_testMimeTypeMap = testMimeTypeMap();

	static int testMimeTypeMap()
	{
		for (size_t i = 0; i < sizeof(mimeTypeMap) / sizeof(mimeTypeMap[0]); ++i)
		{
			if (i > 0)
				ASSERT_EQUAL(mimeTypeMap[i].first > mimeTypeMap[i - 1].first, true, "mimeTypeMap is repeated/not sorted");
		}
		return (0);
	}
#endif

// binary search into headersOfInterest to see if we find our target
static int binSearch(const std::pair<BufferView, BufferView> mimeTypeMap[], size_t sizeOfLookup, const BufferView& target)
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
		midView = mimeTypeMap[mid].first;
		if (midView == target)
			return (mid);
		else if (midView > target)
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (target != mimeTypeMap[low].first)
		return (-1);

	return (low);
}

std::string getMimeType(const std::string &path)
{
	size_t	dotPos = path.rfind('.');

	if (dotPos != std::string::npos)
	{
		BufferView extView = BufferView(path).substr(dotPos, path.size() - dotPos);
		int searchPos = binSearch(mimeTypeMap, sizeof(mimeTypeMap) / sizeof(mimeTypeMap[0]), extView);
		if (searchPos != -1)
			return (mimeTypeMap[searchPos].second.to_string());
	}

	return ("application/octet-stream");
}

namespace Http
{
	struct MediaRange
	{
		std::string type;
		std::string subtype;
		float q;
	};

	

	bool doesAccept(const std::vector<MediaRange>& ranges, const std::string& mediaType)
	{
		// Split mediaType into type and subtype
		size_t slashPos = mediaType.find('/');
		if (slashPos == std::string::npos)
			return false; // Invalid media type format

		std::string	type = mediaType.substr(0, slashPos);
		std::string	subtype = mediaType.substr(slashPos + 1);

		for (size_t i = 0; i < ranges.size(); ++i)
		{
			const MediaRange&	range = ranges[i];

			// Check for exact match
			if (range.type == type && range.subtype == subtype)
				return (true);
			// Check for type/* match
			if (range.type == type && range.subtype == "*")
				return (true);
			// Check for */* wildcard (accepts everything)
			if (range.type == "*" && range.subtype == "*")
				return (true);
		}

		return false;
	}

	std::string trimHeader(const std::string& s)
	{
		size_t start = s.find_first_not_of(" \t\r\n");
		if (start == std::string::npos)
			return "";
		size_t end = s.find_last_not_of(" \t\r\n");
		return s.substr(start, end - start + 1);
	}

	std::vector<MediaRange> parseAcceptHeader(const std::string& header)
	{
		std::vector<MediaRange>	mediaRanges;
		std::istringstream		headerStream(header);
		std::string				token;
		float					q;

		// Split the header
		while (std::getline(headerStream, token, ','))
		{
			token = trimHeader(token);
			if (token.empty())
				continue;

			// Split the token
			std::istringstream	tokenStream(token);
			std::string			mediaType;
			if (!std::getline(tokenStream, mediaType, ';'))
				continue;
			mediaType = trimHeader(mediaType);

			q = 1.0f;

			// Process any additional parameters (like q-value).
			std::string	param;
			while (std::getline(tokenStream, param, ';'))
			{
				param = trimHeader(param);
				if (param.length() > 2 && (param.substr(0, 2) == "q=" || param.substr(0, 2) == "Q="))
				{
					std::string qValue = trimHeader(param.substr(2));
					q = static_cast<float>(atof(qValue.c_str()));
				}
			}

			// Split the media type into type and subtype.
			size_t slashPos = mediaType.find('/');
			MediaRange range;
			if (slashPos != std::string::npos)
			{
				range.type = trimHeader(mediaType.substr(0, slashPos));
				range.subtype = trimHeader(mediaType.substr(slashPos + 1));
			}
			else
			{
				// If there's no slash, treat the entire string as the type.
				range.type = mediaType;
				range.subtype = "";
			}
			range.q = q;
			mediaRanges.push_back(range);
		}
		return mediaRanges;
	}

	// Check if the given media type is accepted by the given ranges.
	bool Response::mf_validateAcceptType(const std::string& header, const std::string& path)
	{
		std::vector<MediaRange>	mediaRange;
		std::string mimeType;
		
		mediaRange = parseAcceptHeader(header);
		// We could implement magic number detection
		mimeType = getMimeType(path);

		return (doesAccept(parseAcceptHeader(header), mimeType));
	}
}
