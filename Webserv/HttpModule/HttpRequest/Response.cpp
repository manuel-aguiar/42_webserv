#include "./Response.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

/* GET CURRENT DATE **********************/
// move to relevant place

#include <iostream>
#include <sstream>
#include <iomanip>  // For std::setw and std::setfill
#include <ctime>    // For std::time_t, std::tm, and std::gmtime
#include <string>

std::string getCurrentDate() {
	// Get current time
	time_t rawTime;
	std::time(&rawTime);

	// Convert time to UTC (GMT)
	std::tm* gmtTime = std::gmtime(&rawTime);

	// Days of the week and months as required by HTTP format
	const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	// Format the date string in the desired HTTP format
	std::stringstream dateStream;
	dateStream << days[gmtTime->tm_wday] << ", "
			<< std::setw(2) << std::setfill('0') << gmtTime->tm_mday << " "
			<< months[gmtTime->tm_mon] << " "
			<< (1900 + gmtTime->tm_year) << " "
			<< std::setw(2) << std::setfill('0') << gmtTime->tm_hour << ":"
			<< std::setw(2) << std::setfill('0') << gmtTime->tm_min << ":"
			<< std::setw(2) << std::setfill('0') << gmtTime->tm_sec << " GMT";

	return dateStream.str();
}

/************************************/

// move to relevant place
#define METHOD_GET 0
#define METHOD_POST 1
#define METHOD_DELETE 2

Http::Response::Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context):
	m_myConnection(myConnection),
	m_myRequest(myRequest),
	m_context(context)
{
	// If there were errors during parsing, respond with error page
	if (myRequest.getStatus() != Http::Status::OK)
	{
		this->generateResponse(myRequest.getStatus());
		return ;
	}

	// Get server block
	m_serverBlock = context.getBlockFinder()->findServerBlock(myConnection.getAddr(), m_myRequest.getHost());
	if (m_serverBlock == NULL)
	{
		this->generateResponse(Http::Status::NOT_FOUND);
		return ;
	}

	// get location (we will need to strip possible extra stuff on the uri)
	if (m_serverBlock->getLocations().find(m_myRequest.getURI()) == m_serverBlock->getLocations().end())
	{
		this->generateResponse(Http::Status::NOT_FOUND);
		return ;
	}
	m_location = m_serverBlock->getLocations().find(m_myRequest.getURI())->second;

	// check for file (extension, exists)
	if (FilesUtils::isFile(m_myRequest.getURI().c_str()))
		this->generateResponse(Http::Status::OK);
	else if (FilesUtils::isDirectory(m_myRequest.getURI().c_str()))
		this->generateResponse(Http::Status::FORBIDDEN);
	else
		this->generateResponse(Http::Status::NOT_FOUND);
}


// 	if (m_myRequest.getMethod() /* NOT IN ALLOWED METHODS */)
// 		this->generateResponse(Http::Status::METHOD_NOT_ALLOWED);
// 	switch (m_myRequest.getMethod())
// 	{
// 		case METHOD_GET:
// 			// Check if path is a file
// 			if (FilesUtils::isFile(it->second.c_str()))
// 				// read file into buffer
// 				this->generateResponse(Http::Status::OK, it->second.c_str());
// 			// Check if path is a directory
// 			else if (FilesUtils::isDirectory(it->second.c_str()))
// 				this->generateResponse(Http::Status::FORBIDDEN);
// 			else
// 				this->generateResponse(Http::Status::NOT_FOUND);
// 			break;
// 		case METHOD_POST:
// 			break;
// 		case METHOD_DELETE:
// 			/* DELETE CASES
// 				200 OK
// 				204 NO_CONTENT
// 				404 NOT_FOUND
// 				405 METHOD_NOT_ALLOWED
// 			*/
// 			break;
// 		default:
// 			// 501 NOT IMPLEMENTED or 405 METHOD NOT ALLOWED (normally used in implemented methods)
// 			break;
// 	}
// }

int	Http::Response::generateResponse(int statusCode)
{
	std::string simpleHTMLpage = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Simple Debug Page</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Hello, Debugging World!</h1>\n"
        "    <p>This is a simple webpage for debugging purposes.</p>\n"
        "    <p>Current date and time: <strong>2025-02-15 14:30</strong></p>\n"
        "    <p>Feel free to modify this page to suit your needs.</p>\n"
        "</body>\n"
        "</html>";

	std::string	statusLine;
	std::string	headers;
	std::string	body;

	// Status line
	statusLine = Http::HttpStandard::HTTP_VERSION +  " " + StringUtils::intToStr(statusCode) + " " + getMessage(statusCode) + "\r\n";

	// Body
	if (statusCode == Http::Status::OK)
	{
		body = simpleHTMLpage; //readFile(m_myRequest.getURI()); // strip down extra stuff from URI
	}
	else
	{
		if (m_serverBlock->getErrorPages().find(statusCode) != m_serverBlock->getErrorPages().end())
			body = simpleHTMLpage; //readFile(m_serverBlock->getErrorPages().find(statusCode)->second); // where will the user error pages be located?
		else
			body = simpleHTMLpage; //readFile(DEFAULT_ERROR_PAGES_PATH + StringUtils::intToStr(statusCode));
	}

	// Headers
	std::string response_headers =
		"Date: " + getCurrentDate() + "\r\n"
		"Content-Length: " + StringUtils::intToStr(body.size()) + "\r\n"
		"Content-Type: text/html; charset=UTF-8" + "\r\n"
		"Server: 42_webserv/1.0" + "\r\n\r\n" ;
}
std::string	&Http::Response::getMessage(size_t statusCode)
{
	static std::map<size_t, std::string>	messages;

	if (messages.empty())
	{
		// 2xx Success
		messages[200] = "OK";
		messages[201] = "Created";
		messages[202] = "Accepted";
		messages[204] = "No Content";
		messages[205] = "Reset Content";
		messages[206] = "Partial Content";
		// 3xx Redirection
		messages[300] = "Multiple Choices";
		messages[301] = "Moved Permanently";
		messages[302] = "Found"; 
		messages[303] = "See Other";
		messages[304] = "Not Modified";
		messages[307] = "Temporary Redirect";
		messages[308] = "Permanent Redirect";
		// 4xx Client Errors
		messages[400] = "Bad Request";
		messages[403] = "Forbidden";
		messages[404] = "Not Found";
		messages[405] = "Method Not Allowed";
		messages[412] = "Precondition Failed";
		messages[413] = "Payload Too Large";
		messages[414] = "URI Too Long";
		messages[415] = "Unsupported Media Type";
		messages[429] = "Too Many Requests";
		messages[431] = "Request Header Fields Too Large";
		messages[451] = "Unavailable For Legal Reasons";
		// 5xx Server Errors
		messages[500] = "Internal Server Error";
		messages[501] = "Not Implemented";
		messages[502] = "Bad Gateway";
		messages[503] = "Service Unavailable";
		messages[504] = "Gateway Timeout";
		messages[505] = "HTTP Version Not Supported";
		messages[506] = "Variant Also Negotiates";
		messages[507] = "Insufficient Storage";
		messages[508] = "Loop Detected";
		messages[510] = "Not Extended";
		messages[511] = "Network Authentication Required";
	}

	return messages[statusCode];
}

//    MEETING
// Status message map
// Connection Close
// Upload file

// Blockfinder to get server
// identify location given path
// identify extension
// DELETE \/ 
// unlink a file syscall
