

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

	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		//std::cout << "\t\tResponse received header data" << std::endl;
		m_responseData.requestData = &data;
	}

	BufferView	Response::receiveRequestBody(const BufferView& view)
	{
		const std::string& targetFile = m_responseData.requestData->multipart_Filename;
		if (g_mockMsgBody.find(targetFile) == g_mockMsgBody.end())
			g_mockMsgBody[targetFile] = "";
		g_mockMsgBody[targetFile].append(view.data(), view.size());

		return (BufferView()); // consumes all
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
	Response::close()
	{}
	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{(void)addr;}

}
