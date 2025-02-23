

#include "HttpResponse.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"


extern const char*	getStatusMessage(int statusCode);
extern int			DirectoryListing(const std::string& path, std::string& placeOutput);
extern std::string getCurrentDate();


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

void	Http::Response::receiveRequestData(const Http::RequestData& data)
{
	if (data.status != Http::Status::OK)
	{
		// PREPARE ERROR MESAGE
		//PREPARE CLOSE CONNECTION
		return ;
	}
	// NOT IMPLEMENTED YET
}

void	Http::Response::receiveRequestBody(const BufferView& view)
{
	// NOT IMPLEMENTED YET
}

// NOT IMPLEMENTED YET
void	Http::Response::reset()
{
	m_file.close();
	m_serverBlock = NULL;
	m_location = NULL;
	m_pendingWrite.clear();
	m_headers.clear();
	m_status = WAITING;
}

// Generates Status Line and Headers and body, exept if body is a file or CGI
void	Http::Response::generateResponse(int statusCode)
{
	if (m_status != WAITING)
		return ;

	std::string	statusLine;
	std::string	headers;
	std::string	body;

	statusLine = generateStatusLine(statusCode);

	// could be a switch
	if (m_myRequest.getMethod() == "GET")
	{
		switch (statusCode)
		{
			// In case of CGI it will be handled differently 
			case Http::Status::OK:
				if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
				{
					// TODO on REQUEST: check auto_index on serverConfig
					DirectoryListing(m_myRequest.getUri().c_str(), body);
				}
				else
				{
					m_file.open(m_myRequest.getUri().c_str());
					if (m_file.fd() == Ws::FD_NONE)
					{
						this->generateResponse(Http::Status::NOT_FOUND);
						return ;
					}
					m_headers["Content-Lenght"] = StringUtils::intToStr(m_file.size());
				}
				setGetRqContentType(m_headers, 0);
				break ;
			default:
				// User defined error page or default error page
				if (1 == 2 /*m_serverBlock->getErrorPages().find(statusCode) != m_serverBlock->getErrorPages().end()*/)
				{
					// m_file = File(m_serverBlock->getErrorPages().find(statusCode)->second);
					// m_headers["Content-Lenght"] = StringUtils::intToStr(m_file.size());
					body = generateDefaultErrorPage(statusCode, getStatusMessage(statusCode), "Get some user defined message in here");
				}
				else
					body = generateDefaultErrorPage(statusCode, getStatusMessage(statusCode), "Error");
				m_headers["Content-Type"] = "text/html";
				break ;
		}
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
	if (body.empty() && m_file.fd() == Ws::FD_NONE) // CGI
		m_headers["Transfer-Encoding"] = "chunked";
	else 
		m_headers["Content-Length"] = StringUtils::intToStr(body.size());
	m_headers["Date"]			= getCurrentDate();
	m_headers["Server"] 		= SERVER_NAME_VERSION;
	std::map<std::string, std::string>::const_iterator it = m_myRequest.getHeaders().find("Connection");
	if (it != m_myRequest.getHeaders().end() && it->second == "close")
		m_headers["Connection"]	= "close";
	headers = generateHeaderString();

	if (body.empty())
		m_pendingWrite = statusLine + headers; // CRLF included in headers 
	else 
		m_pendingWrite = statusLine + headers + body + "\r\n\r\n";

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

std::string Http::Response::generateDefaultErrorPage(int statusCode, const std::string& statusText, const std::string& errorMessage)
{
    std::stringstream ss;

    ss << "<!DOCTYPE html>\n"
       << "<html>\n"
       << "<head>\n"
       << "<title>" << statusCode << " " << statusText << "</title>\n"
       << "<style>\n"
       << "body {\n"
       << "    font-family: system-ui, sans-serif;\n"
       << "    margin: 40px auto;\n"
       << "    max-width: 1750px;\n"
       << "    padding: 0 10px;\n"
       << "    color: #444;\n"
       << "    text-align: center;\n"
       << "}\n"
       << ".server {\n"
       << "    font-size: 20px;\n"
       << "    font-weight: bold;\n"
       << "}\n"
       << ".status-code {\n"
       << "    font-size: 48px;\n"
       << "    font-weight: bold;\n"
       << "    color: #e74c3c;\n"
       << "    margin: 30px 0 10px 0;\n"
       << "}\n"
       << "h1 {\n"
       << "    margin: 10px 0;\n"
       << "}\n"
       << "hr {\n"
       << "    width: 100%;\n"
       << "    margin: 10px auto;\n"
       << "    border: none;\n"
       << "    border-top: 1px solid #ddd;\n"
       << "}\n"
       << "</style>\n"
       << "</head>\n"
       << "<body>\n"
       << "    <div class=\"server\">" << SERVER_NAME_VERSION << "</div>\n"
       << "    <hr>\n"
       << "    <div class=\"status-code\">" << statusCode << "</div>\n"
       << "    <h1>" << statusText << "</h1>\n"
       << "    <p>" << errorMessage << "</p>\n"
       << "</body>\n"
       << "</html>\n";

    return ss.str();
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
		m_file.close();
	}

	Response&
	Response::operator=(const Response& other)
	{
		if (this == &other)
			return (*this);
		
		// m_file = other.m_file;
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
		// Read file to buffer
		// assuming read will not read if buffer is already full
		if (m_file.fd() != Ws::FD_NONE)
			writeBuffer.read(m_file.fd());
		
		return (m_status);
	}

	Response::Status 
	Response::getStatus() const
	{
		return (m_status);
	}
}

// Big swich case with possible content-types for GET requests
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