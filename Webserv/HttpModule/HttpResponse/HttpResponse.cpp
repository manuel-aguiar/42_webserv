

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
extern std::string	DirectoryListing(const std::string& path);
extern std::string 	getCurrentDate();

namespace Http
{
	Response::Response(ServerContext& context):
	m_context		(context),
	m_connAddress	(NULL),
	m_requestData	(NULL),
	m_serverBlock	(NULL),
	m_location		(NULL),
	m_type			(Response::Type::NONE),
	m_status		(Http::ResponseStatus::WAITING),
	m_fillFunction	(&Response::mf_fillNothingToSend),
	m_file			(),
	m_cgiGateway	(*reinterpret_cast<Cgi::Module*>(m_context.getAddonLayer(Ws::AddonLayer::CGI)))
	{}

	Response::~Response() { reset();}

	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_requestData = &data;

		if (m_requestData->status != Http::Status::OK || !mf_validateHeaders())
		{
			m_fillFunction = &Response::mf_fillErrorResponse;
			return ;
		}

		m_fillFunction = &Response::mf_fillResponseLine;
	}

	void	Response::receiveRequestBody(const BufferView& view)
	{
		(void)view;
		// NOT IMPLEMENTED YET
	}


	Http::ResponseStatus
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		// call the current filling function
		return ((this->*m_fillFunction)(writeBuffer));

		// NOT IMPLEMENTED YET
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

	Http::ResponseStatus
	Response::getStatus() const
	{
		return (m_status);
	}

	// NOT IMPLEMENTED YET
	void	Http::Response::reset()
	{
		m_file.reset();
		m_serverBlock = NULL;
		m_location = NULL;
		m_requestData = NULL;
		m_fillFunction = &Response::mf_fillNothingToSend;
		m_pendingWrite.clear();
		m_headers.clear();
		m_status = WAITING;

		m_cgiGateway.reset();
	}

	// Generates Status Line and Headers and body, exept if body is a file or CGI
	void	Response::mf_generateResponse(int statusCode)
	{
		if (m_status != WAITING)
			return ;

		std::string	statusLine;
		std::string	headers;
		std::string	body;

		statusLine = mf_generateStatusLine(statusCode);

		ASSERT_EQUAL(m_requestData != NULL, true, "Response: Request data not set");

		// could be a switch
		if (m_requestData->method == "GET")
		{
			switch (statusCode)
			{
				// In case of CGI it will be handled differently 
				case Http::Status::OK:
					if (FilesUtils::isDirectory(m_requestData->uri.c_str()))
					{
						// TODO on REQUEST: check auto_index on serverConfig
						body = DirectoryListing(m_requestData->uri.c_str());
					}
					else
					{
						m_file.open(m_requestData->uri.c_str());
						if (m_file.fd() == Ws::FD_NONE)
						{
							this->mf_generateResponse(Http::Status::NOT_FOUND);
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
						body = mf_generateDefaultErrorPage(statusCode, getStatusMessage(statusCode), "Get some user defined message in here");
					}
					else
						body = mf_generateDefaultErrorPage(statusCode, getStatusMessage(statusCode), "Error");
					m_headers["Content-Type"] = "text/html";
					break ;
			}
		}
		else if (m_requestData->method == "POST")
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
		else if (m_requestData->method == "DELETE")
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
		std::map<std::string, std::string>::const_iterator it = m_requestData->headers.find("Connection");
		if (it != m_requestData->headers.end() && it->second == "close")
			m_headers["Connection"]	= "close";
		headers = mf_generateHeaderString();

		if (body.empty())
			m_pendingWrite = statusLine + headers; // CRLF included in headers 
		else 
			m_pendingWrite = statusLine + headers + body + "\r\n\r\n";

		m_status = WRITING;
	}

	std::string Http::Response::mf_generateStatusLine(int statusCode)
	{
		return Http::HttpStandard::HTTP_VERSION + " "
				+ StringUtils::intToStr(statusCode) + " "
				+ getStatusMessage(statusCode) + "\r\n";
	}

	std::string Http::Response::mf_generateHeaderString()
	{
		std::ostringstream								headerStream;
		std::map<std::string, std::string>::iterator	it;
		
		for (it = m_headers.begin(); it != m_headers.end(); ++it)
			headerStream << it->first << ": " << it->second << "\r\n";
		headerStream << "\r\n";

		return headerStream.str();
	}

	std::string Http::Response::mf_generateDefaultErrorPage(int statusCode, const std::string& statusText, const std::string& errorMessage)
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

		return (ss.str());
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

	// private copy/assignment
	Response::Response(const Response& other) :
		m_context(other.m_context),
		m_cgiGateway((*reinterpret_cast<Cgi::Module*>(m_context.getAddonLayer(Ws::AddonLayer::CGI)))) {}

	Response&
	Response::operator=(const Response& other) { (void)other; return (*this);}

}
