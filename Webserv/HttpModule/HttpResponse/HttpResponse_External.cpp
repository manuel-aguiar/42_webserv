

#include "HttpResponse.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

namespace Http
{
	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		if (data.status != Http::Status::OK || !mf_validateHeaders())
		{
			if (m_responseData.responseType == ResponseData::REDIRECT) {
				m_fillFunction = &Response::mf_fillRedirect;
				return ;
			}

			m_fillFunction = &Response::mf_fillErrorResponse;
			return ;
		}

		if (m_fillFunction == &Response::mf_fillNothingToSend)
		{
			if (m_responseData.requestStatus == Http::Status::OK)
				m_responseData.requestStatus = Http::Status::NOT_IMPLEMENTED;
			m_fillFunction = &Response::mf_fillErrorResponse;
		}

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
		m_cgiGateway.reset();
	}

	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{
		m_connAddress = &addr;
	}

}
