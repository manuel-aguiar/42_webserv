/* *********************************/
/*                                 */
/*   http_parse_header_line.cpp    */
/*   - the header line parser for  */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"
#include <sstream>

int HttpRequest::mf_parseHeaders(const std::string& data)
{
	std::istringstream	stringStream(data);
	std::string			line;

	while (std::getline(stringStream, line) && !line.empty())
	{
		if (line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		// Check for semicolons (there should only be one)
		std::size_t colonPos = line.find(':');
		if (colonPos == std::string::npos || line.find(':', colonPos + 1) != std::string::npos)
			return (Http::Status::BAD_REQUEST);
		// Trim key and value into separate variables
		std::string key = StringUtils::strtrim(line.substr(0, colonPos));
		std::string value = StringUtils::strtrim(line.substr(colonPos + 1));
		if (key.empty() || value.empty())
			return (Http::Status::BAD_REQUEST);
		m_headers[key] = value;
	}
	
	return (Http::Status::OK);
}