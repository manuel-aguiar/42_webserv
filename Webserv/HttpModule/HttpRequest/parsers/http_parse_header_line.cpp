/* *********************************/
/*                                 */
/*   http_parse_header_line.cpp    */
/*   - the header line parser for  */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"
#include <sstream>

int HttpRequest::mf_parseHeaders(const std::string& data)
{
	std::istringstream	stringStream(data);
	std::string			line;

	while (std::getline(stringStream, line) && !line.empty())
	{
		if (line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		std::size_t colonPos = line.find(':');
		if (colonPos == std::string::npos)
			return (Http::Status::BAD_REQUEST);
		std::string key = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);

		while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) {
			value.erase(0, 1);
		}
		m_headers[key] = value;
	}
	return (Http::Status::OK);
}