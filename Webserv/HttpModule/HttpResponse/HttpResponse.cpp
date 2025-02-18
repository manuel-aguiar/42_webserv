

#include "HttpResponse.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

/* GET CURRENT DATE **********************/
// move to relevant place

#include <iostream>
#include <sstream>
#include <iomanip>  // For std::setw and std::setfill
#include <ctime>    // For std::time_t, std::tm, and std::gmtime
#include <string>

extern const char* getStatusMessage(int statusCode);

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

// // move to relevant place
// #define METHOD_GET 0
// #define METHOD_POST 1
// #define METHOD_DELETE 2

// Http::Response::Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context):
Http::Response::Response(Http::Request &myRequest, ServerContext &context):
	m_myRequest		(myRequest),
	m_context		(context),
	m_file			(NULL),
	m_serverBlock	(NULL),
	m_location		(NULL)
{
	// If there were errors during parsing, respond with error page
	if (myRequest.getStatus() != Http::Status::OK)
	{
		std::cerr << "DEBUG: " << "Errors During Parsing!\n";
		this->generateResponse(myRequest.getStatus());
		return ;
	}

	// Get server block
	//m_serverBlock = context.getBlockFinder()->findServerBlock(myConnection.getAddr(), m_myRequest.getHost());
	m_serverBlock = NULL;
	// if (m_serverBlock == NULL)
	// {
	// 	std::cerr << "DEBUG: " << "ServerBlock Not Found!\n";
	// 	this->generateResponse(Http::Status::NOT_FOUND);
	// 	return ;
	// }

	// // get location
	m_location = NULL;
	// if (m_serverBlock->getLocations().find(m_myRequest.getUri()) == m_serverBlock->getLocations().end())
	// {
	// 	std::cerr << "DEBUG: " << "Location Not Found!\n";
	// 	this->generateResponse(Http::Status::NOT_FOUND);
	// 	return ;
	// }
	// m_location = m_serverBlock->getLocations().find(m_myRequest.getUri())->second;

	// check for file (extension, exists)
	if (FilesUtils::isFile(m_myRequest.getUri().c_str()))
	{
		std::cerr << "DEBUG: " << "Requested URI is File\n";
		this->generateResponse(Http::Status::OK);
	}
	else if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
	{
		std::cerr << "DEBUG: " << "Requested URI is Directory\n";
		this->generateResponse(Http::Status::FORBIDDEN);
	}
	else
	{
		std::cerr << "DEBUG: " << "Requested URI Not Found\n";
		this->generateResponse(Http::Status::NOT_FOUND);
	}
}

void	Http::Response::generateResponse(int statusCode)
{
		std::string simpleErrorpage = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Simple Debug Page</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Error!</h1>\n"
        "</body>\n"
        "</html>";

	std::string	statusLine;
	std::string	headers;
	std::string	body;

	// should extracting file extension go to HttpRequest parsing?
	std::string fileExtension;
	size_t dotPos = m_myRequest.getUri().find_last_of('.');
	if (dotPos != std::string::npos)
		fileExtension = m_myRequest.getUri().substr(m_myRequest.getUri().find_last_of('.') + 1);
	else
		fileExtension = "";

	// Status line
	statusLine = Http::HttpStandard::HTTP_VERSION +  " " + StringUtils::intToStr(statusCode) + " " + getMessage(statusCode) + "\r\n";

	// Body
	if (statusCode == Http::Status::OK)
	{
		File file(m_myRequest.getUri().c_str());
		char testBuffer[1024];
		memset(&testBuffer, '\0', 1024);
		file.read(&testBuffer, 1024);
		body = testBuffer;
	}
	else
	{
		// // where will the user error pages be located?
		// if (m_serverBlock->getErrorPages().find(statusCode) != m_serverBlock->getErrorPages().end())
		// 	readFile(m_serverBlock->getErrorPages().find(statusCode)->second);
		// else
		// 	readFile(DEFAULT_ERROR_PAGES_PATH + StringUtils::intToStr(statusCode));
		body = simpleErrorpage;
	}

	// Headers
	headers =
		"Date: " + getCurrentDate() + "\r\n"
		"Content-Length: " + StringUtils::intToStr(body.size()) + "\r\n"
		"Content-Type: text/html; charset=UTF-8" + "\r\n"
		"Server: 42_webserv/1.0" + "\r\n\r\n" ;

	std::string	fullResponse = statusLine + headers + body;
	std::cerr << "DEBUG: " << "FULL RESPONSE:" << "\n" << fullResponse << "\n*** END OF RESPONSE ***\n";
}


//NOT IMPLEMENTED YET
namespace Http
{
	Response::Response(const Response& other) :
		m_myRequest(other.m_myRequest),
		m_context(other.m_context),
		m_file(other.m_file),
		m_serverBlock(other.m_serverBlock),
		m_location(other.m_location) {}

	Response::~Response() {}

	Response&
	Response::operator=(const Response& other)
	{
		if (this == &other)
			return (*this);
		
		m_file = other.m_file;
		m_serverBlock = other.m_serverBlock;
		m_location = other.m_location;

		return (*this);
	}

	void
	Response::fillWriteBuffer(Buffer& writeBuffer)
	{
		int pushed = 0;

		// push pending data to buffer
		if (!m_pendingWrite.empty())
		{
			pushed = (writeBuffer.available() < m_pendingWrite.size()) ? writeBuffer.available() : m_pendingWrite.size();
			writeBuffer.push(m_pendingWrite.c_str(), pushed);
			m_pendingWrite.erase(0, pushed);
			if (m_pendingWrite.empty())
			{
				// ?? m_request.done(); ??
				
				// could be the end of the response, depends on cgi as well
				// sent everything i had...
				return ;
			}
		}
		
		// tell the buffer to read from file directly buffer.read(file.fd())
		// or push directory listing to buffer
		// or push error page to buffer
	}
}



//    MEETING
// Status message map
// Connection Close
// Upload file

// Blockfinder to get server
// identify location given path
// identify extension




void generateMessage(int StatusCode)
{
	// use the template below to generate the error message
	(void)StatusCode;
//	std::stringstream ss;
//
//	ss << "<!DOCTYPE html>
//		<html>
//
//		<head>
//			<title>{{STATUS_CODE}} {{STATUS_TEXT}}</title> //here
//			<style>
//				body {
//					font-family: system-ui, sans-serif;
//					margin: 40px auto;
//					max-width: 1750px;
//					padding: 0 10px;
//					color: #444;
//					text-align: center;
//				}
//				.server {
//					font-size: 20px;
//					font-weight: bold;
//				}
//				.status-code {
//					font-size: 48px;
//					font-weight: bold;
//					color: #e74c3c;
//					margin: 30px 0 10px 0;
//				}
//				h1 {
//					margin: 10px 0;
//				}
//				hr {
//					width: 100%;
//					margin: 10px auto;
//					border: none;
//					border-top: 1px solid #ddd;
//				}
//			</style>
//		</head>
//
//		<body>
//			<div class="server">{{SERVER_NAME}}/{{SERVER_VERSION}}</div> //change valeus
//			<hr>
//			<div class="status-code">{{STATUS_CODE}}</div>
//			<h1>{{STATUS_TEXT}}</h1>
//			<p>{{ERROR_MESSAGE}}</p>
//		</body>
//
//		</html>"

}