
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include "HttpResponse.hpp"

std::string getMimeType(const std::string &path)
{
	// optimize this map however you see fit
	std::map<std::string, std::string>	mimeMap;

	mimeMap[".html"]	= "text/html";
	mimeMap[".htm"]		= "text/html";
	mimeMap[".css"]		= "text/css";
	mimeMap[".js"]		= "text/javascript";
	mimeMap[".xml"]		= "text/xml";
	mimeMap[".txt"]		= "text/plain";
	mimeMap[".json"]	= "application/json";
	mimeMap[".jpg"]		= "image/jpeg";
	mimeMap[".jpeg"]	= "image/jpeg";
	mimeMap[".png"]		= "image/png";
	mimeMap[".gif"]		= "image/gif";
	mimeMap[".ico"]		= "image/x-icon";

	size_t	dotPos = path.rfind('.');

	if (dotPos != std::string::npos)
	{
		std::string ext = path.substr(dotPos);
		if (mimeMap.find(ext) != mimeMap.end())
			return (mimeMap[ext]);
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
