

#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Files/FilesUtils.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"



// MOCK IMPLEMENTATION, HELPS IN TESTS
// just to extract the message body via global, deactivate all the helper stuff, etc
// allows Request to communicate with Response as if the latter was the real deal

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

extern const char*	getStatusMessage(int statusCode);
extern std::string	DirectoryListing(const std::string& path);
extern std::string 	getCurrentDate();

std::map<std::string, std::string > g_mockMsgBody;

namespace Http
{
	Response::Response(ServerContext& context):
	m_context			(context),
	m_connAddress		(NULL),
	m_responseData		(),
	m_status			(Http::ResponseStatus::WAITING),
	m_fillFunction		(&Response::mf_fillNothingToSend),
	m_file				(),
	m_cgiGateway		(*reinterpret_cast<Cgi::Module*>(m_context.getAddonLayer(Ws::AddonLayer::CGI))) {}

	Response::~Response() { reset();}

	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		std::cout << "\t\tResponse received header data" << std::endl;
		m_responseData.requestData = &data;
	}

	void	Response::receiveRequestBody(const BufferView& view)
	{
		std::cout << "\t\tResponse received view: '" << view << "'" << std::endl;

		const std::string& targetFile = m_responseData.requestData->multipart_Filename;
		if (g_mockMsgBody.find(targetFile) == g_mockMsgBody.end())
			g_mockMsgBody[targetFile] = "";
		g_mockMsgBody[targetFile].append(view.data(), view.size());
	}


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		(void)writeBuffer;
		
		return (m_status);
	}

	Http::ResponseStatus::Type
	Response::getStatus() const
	{
		return (m_status);
	}
	void
	Response::reset()
	{}
	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{(void)addr;}

	// private copy/assignment
	Response::Response(const Response& other) :
		m_context(other.m_context),
		m_cgiGateway((*reinterpret_cast<Cgi::Module*>(m_context.getAddonLayer(Ws::AddonLayer::CGI)))) {}

	Response&
	Response::operator=(const Response& other) { (void)other; return (*this);}

}
