

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
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		if (data.status != Http::Status::OK || !mf_validateHeaders())
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


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{

		// call the current filling function
		return ((this->*m_fillFunction)(writeBuffer));

		// NOT IMPLEMENTED YET
		if (m_status == ResponseStatus::WAITING)
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
				m_status = ResponseStatus::FINISHED;
				return (m_status);
			}
		}

		// Read file to buffer
		// assuming read will not read if buffer is already full
		if (m_file.fd() != Ws::FD_NONE)
			writeBuffer.read(m_file.fd());
		
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
		m_pendingWrite.clear();
		m_status = ResponseStatus::WAITING;
		m_file.reset();
		m_cgiGateway.reset();
	}

	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{
		m_connAddress = &addr;
	}

	// private copy/assignment
	Response::Response(const Response& other) :
		m_context(other.m_context),
		m_cgiGateway((*reinterpret_cast<Cgi::Module*>(m_context.getAddonLayer(Ws::AddonLayer::CGI)))) {}

	Response&
	Response::operator=(const Response& other) { (void)other; return (*this);}

}
