

#include "HttpResponse.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

extern const char*	getStatusMessage(int statusCode);
extern std::string	DirectoryListing(const std::string& path);
extern std::string 	getCurrentDate();
extern std::string 	getMimeType(const std::string &path);

namespace Http
{
	Response::Response(ServerContext& context):
		m_context			(context),
		m_listenAddress		(NULL),
		m_tcpConn			(NULL),
		m_responseData		(),
		m_status			(Http::ResponseStatus::WAITING),
		m_fillFunction		(&Response::mf_fillNothingToSend),
		m_fillFunctionBody	(NULL),
		m_processFunction	(&Response::mf_processBodyNone),
		m_staticReadCounter	(0),
		m_file				(),
		m_cgiResponse		(NULL),
		m_currentHeader		(m_responseData.headers.begin()) {}

	Response::~Response() { reset();}

	
	// private copy/assignment
	Response::Response(const Response& other) :
		m_context(other.m_context),
		m_cgiResponse(NULL) {}

	Response&
	Response::operator=(const Response& other) { (void)other; return (*this);}

}
