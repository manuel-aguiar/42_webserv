

#include "../../HttpResponse.hpp"
#include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Files/FilesUtils.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

extern const char*	getStatusMessage(int statusCode);
extern std::string	DirectoryListing(const std::string& path);
extern std::string 	getCurrentDate();

bool	g_returnValue;
int		g_requestStatus = Http::Status::OK;

namespace Http
{


	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		// mock the caller, not the target!!!!!!!
		g_returnValue = mf_validateHeaders();
		g_requestStatus = m_responseData.requestStatus;		
	}

	BufferView
	Response::receiveRequestBody(const BufferView& view)
	{
		return ((this->*m_processFunction)(view));
	}


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{

		// call the current filling function
		return ((this->*m_fillFunction)(writeBuffer));

		return (m_status);
	}

	Http::ResponseStatus::Type
	Response::getStatus() const
	{
		return (m_status);
	}

	void
	Response::reset()
	{
		m_responseData.reset();
		m_fillFunction = &Response::mf_fillNothingToSend;
		m_processFunction = &Response::mf_processBodyNone;
		m_pendingWrite.clear();
		m_status = ResponseStatus::WAITING;
		m_staticReadCounter = 0;
		m_file.reset();
		if (m_cgiResponse)
			m_cgiResponse->reset();
	}


	void
	Response::close()
	{
		reset();
		m_listenAddress = NULL;
	}	

	void
	Response::setListenAddress(const Ws::Sock::addr& addr)
	{
		m_listenAddress = &addr;
	}

	void
	Response::setTcpConnection(const Conn::Connection& tcpConn)
	{
		m_tcpConn = &tcpConn;
	}

}
