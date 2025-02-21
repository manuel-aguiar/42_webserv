

#include "HttpResponse.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

extern const char* getStatusMessage(int statusCode);
extern int	DirectoryListing(const std::string& path, std::string& placeOutput);

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

// Http::Response::Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context):
Http::Response::Response(Http::Connection& conn, Http::Request& myRequest):
	m_myRequest		(myRequest),
	m_connection	(conn),
	m_file			(NULL),
	m_serverBlock	(NULL),
	m_location		(NULL),
	m_status		(Response::WAITING)
{
	/****** These checks go to request ********************************** */
	// Get server block
	//m_serverBlock = context.getBlockFinder()->findServerBlock(myConnection.getAddr(), m_myRequest.getHost());
	// if (m_serverBlock == NULL)
	// {
	// 	std::cerr << "DEBUG: " << "ServerBlock Not Found!\n";
	// 	this->generateResponse(Http::Status::NOT_FOUND);
	// 	return ;
	// }

	// // get location
	// if (m_serverBlock->getLocations().find(m_myRequest.getUri()) == m_serverBlock->getLocations().end())
	// {
	// 	std::cerr << "DEBUG: " << "Location Not Found!\n";
	// 	this->generateResponse(Http::Status::NOT_FOUND);
	// 	return ;
	// }
	// m_location = m_serverBlock->getLocations().find(m_myRequest.getUri())->second;

	// check for file (extension, exists)
	// if (FilesUtils::isFile(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is File\n";
	// 	this->generateResponse(Http::Status::OK);
	// }
	// else if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is Directory\n";
	// 	// check auto_index on serverConfig
	// 	this->generateResponse(Http::Status::FORBIDDEN);
	// }
	// else
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI Not Found\n";
	// 	this->generateResponse(Http::Status::NOT_FOUND);
	// }

	/****************************************************************** */

	// NOTE: generateResponse() will be called at the end of http::request parsing process and not here
	
	// ParsingIncomplete -> Wait
	if (myRequest.getParsingState() != Http::Request::ERROR && myRequest.getParsingState() != Http::Request::COMPLETED)
		return ;
	// Request parsing/execution finished -> Generate response.
	generateResponse(myRequest.getStatus());
}


// NOT IMPLEMENTED YET
void	Http::Response::reset()
{

	if (m_file)
	{
		delete m_file;
		m_file = NULL;
	}
	m_serverBlock = NULL;
	m_location = NULL;
	m_pendingWrite.clear();
	m_headers.clear();
	m_status = WAITING;
}

// Generates Status Line and Headers and body, exept if body is a file or CGI
void	Http::Response::generateResponse(int statusCode)
{
	// response is only generated when there is nothing to be sent
	if (m_status != WAITING)
		return ;

	std::string	statusLine;
	std::string	headers;
	std::string	body;

	statusLine = generateStatusLine(Http::Status::OK);

	// could be a switch
	if (m_myRequest.getMethod() == "GET")
	{
		switch (statusCode)
		{
			// In case of CGI it will be handled differently 
			case Http::Status::OK:
				if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
				{
					std::cerr << "DEBUG: " << "Requested URI is Directory\n";
					// TODO: check auto_index on serverConfig
					DirectoryListing(m_myRequest.getUri().c_str(), body);
				}
				else
				{
					m_file = new File(m_myRequest.getUri().c_str());
					// If cant open file?
					m_headers["Content-Lenght"] = StringUtils::intToStr(m_file->size());
				}
				setGetRqContentType(m_headers, 0);
				break ;
			default:
				// User defined error page or default error page
				if (1 == 2 /*m_serverBlock->getErrorPages().find(statusCode) != m_serverBlock->getErrorPages().end()*/)
					body = generateDefaultErrorPage(statusCode); //readFile(m_serverBlock->getErrorPages().find(statusCode)->second);
				else
					body = generateDefaultErrorPage(statusCode);
				m_headers["Content-Type"] = "text/html";
				break ;
		}

		return ;
	}
	else if (m_myRequest.getMethod() == "POST")
	{
		// Done in a simple way, we may want to improve this a bit
		m_headers["Content-Type"] = "text/plain";

		switch (statusCode)
		{
			case Http::Status::OK:
				body = "Request successful. Resource created or updated.";
				break;
			case Http::Status::NOT_FOUND:
				body = "Resource not found.";
				break;
			case Http::Status::FORBIDDEN:
				body = "Resource cannot be created or updated.";
				break;
			default:
				body = "Request failed.";
				break;
		}
	}
	else if (m_myRequest.getMethod() == "DELETE")
	{
		// Done in a simple way, we may want to improve this a bit
		m_headers["Content-Type"] = "text/plain";

		switch (statusCode)
		{
			case Http::Status::OK:
				body = "Request successful. Resource deleted.";
				break;
			case Http::Status::NOT_FOUND:
				body = "Resource not found.";
				break;
			case Http::Status::FORBIDDEN:
				body = "Resource cannot be deleted.";
				break;
			default:
				body = "Request failed.";
				break;
		}
	}

	// Common headers
	if (!body.empty())
		m_headers["Content-Lenght"] = StringUtils::intToStr(body.size());
	else
		m_headers["Transfer-Encoding"] = "chunked";
	m_headers["Date"]			= getCurrentDate();
	m_headers["Server"] 		= "42_webserv/1.0";
	std::map<std::string, std::string>::const_iterator it = m_myRequest.getHeaders().find("Connection");
	if (it != m_myRequest.getHeaders().end() && it->second == "close")
		m_headers["Connection"]	= "close";

	headers = generateHeaderString();
	if (!body.empty())
		m_pendingWrite = statusLine + headers + body + "\r\n\r\n";
	else 
		m_pendingWrite = statusLine + headers;

	m_status = WRITING;
}

std::string Http::Response::generateStatusLine(int statusCode)
{
	return Http::HttpStandard::HTTP_VERSION + " "
			+ StringUtils::intToStr(statusCode) + " "
			+ getStatusMessage(statusCode) + "\r\n";
}

std::string Http::Response::generateHeaderString()
{
	std::ostringstream								headerStream;
	std::map<std::string, std::string>::iterator	it;
	
	for (it = m_headers.begin(); it != m_headers.end(); ++it)
		headerStream << it->first << ": " << it->second << "\r\n";
	headerStream << "\r\n";

	return headerStream.str();
}

std::string Http::Response::generateDefaultErrorPage(int StatusCode)
{
	/*** TEMP */
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

	return simpleErrorpage;

	/*** TEMP */

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


//NOT IMPLEMENTED YET
namespace Http
{
	Response::Response(const Response& other) :
		m_myRequest(other.m_myRequest),
		m_connection(other.m_connection),
		m_file(other.m_file),
		m_serverBlock(other.m_serverBlock),
		m_location(other.m_location),
		m_status(other.m_status) {}

	Response::~Response()
	{
		if (m_file)
			delete m_file;
	}

	Response&
	Response::operator=(const Response& other)
	{
		if (this == &other)
			return (*this);
		
		//m_file = other.m_file;
		m_serverBlock = other.m_serverBlock;
		m_location = other.m_location;

		return (*this);
	}
	
	Response::Status
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		if (m_status == WAITING)
			return (m_status);

		// amount of bytes to push
		int	pushed = 0;

		// push pending data to buffer
		if (!m_pendingWrite.empty())
		{
			pushed = (writeBuffer.available() < m_pendingWrite.size()) ? writeBuffer.available() : m_pendingWrite.size();
			writeBuffer.push(m_pendingWrite.c_str(), pushed);
			m_pendingWrite.erase(0, pushed);
			if (m_pendingWrite.empty())
			{
				m_myRequest.done();
				m_status = FINISHED;
				
				return (m_status);
			}
		}
		if (m_file)
			writeBuffer.read(m_file->fd(), m_file->offset());
		
		return (m_status);
	}

	Response::Status 
	Response::getStatus() const
	{
		return (m_status);
	}
}


void	Http::Response::setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension)
{
	(void)fileExtension;
	m_headers["Content-Type"] = "text/plain";

	// switch (fileExtension)
	// {
	// 	// TEXT FILES
	// 	case TXT:
	// 		m_headers["Content-Type"] = "text/plain";
	// 		break ;
	// 	case HTM:
	// 		m_headers["Content-Type"] = "text/html";
	// 		break ;
	// 	case HTML:
	// 		m_headers["Content-Type"] = "text/html";
	// 		break ;
	// 	case CSS:
	// 		m_headers["Content-Type"] = "text/css";
	// 		break ;
	// 	case JS:
	// 		m_headers["Content-Type"] = "text/javascript";
	// 		break ;
	// 	case JPG:
	// 		m_headers["Content-Type"] = "image/jpeg";
	// 		break ;
	// 	case PNG:
	// 		m_headers["Content-Type"] = "image/png";
	// 		break ;
	// 	case GIF:
	// 		m_headers["Content-Type"] = "image/gif";
	// 		break ;
	// 	case ICO:
	// 		m_headers["Content-Type"] = "image/x-icon";
	// 		break ;
	// }
}