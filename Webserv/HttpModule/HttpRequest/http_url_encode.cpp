

#include <string>
#include <exception>
#include <stdexcept>

/*

	Characters that need encoding:
		utf-8


*/

#define URI_QUERYSTRING '?'
#define URI_END '\0'
#define URI_FRAGMENT '#'
#define URI_PERCENT '%'


bool isHexDigit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}



std::string	http_uri_resource_decode(const std::string& url)
{
	std::string result;
	unsigned char c;

	result.reserve(url.size());
	size_t i = 0;

	enum {
		URI_NORMAL,
		URI_PERCENT_FIRST,
		URI_PERCENT_SECOND
	} state = URI_NORMAL;

	for (;i < url.size(); ++i)
	{
		switch (state)
		{
			case URI_NORMAL:
			{
				if (url[i] == URI_QUERYSTRING || url[i] == URI_END || url[i] == URI_FRAGMENT)
					break;
				if (url[i] == URI_PERCENT)
					state = URI_PERCENT_FIRST;
				else
					result.push_back(url[i]);
			}
			break ;

			case URI_PERCENT_FIRST:
			{
				if (!isHexDigit(url[i]))
					throw std::runtime_error("invalid uri encoding");

			}
			break ;
		}

	}

	while (url[i])
		result.push_back(url[i++]);

	return (result);
}
