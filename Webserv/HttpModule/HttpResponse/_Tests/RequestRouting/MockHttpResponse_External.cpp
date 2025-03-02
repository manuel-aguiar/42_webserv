

#include "../../HttpResponse.hpp"
#include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Files/FilesUtils.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../HttpModule/HttpModule.hpp"
#include "../../../HttpCgiInterface/HttpCgiInterface.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

Http::ResponseData g_responseData;

# include <cstdlib>

namespace Http
{
	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;
		mf_requestRouting();
		g_responseData = m_responseData;
	}

	BufferView
	Response::receiveRequestBody(const BufferView& view)
	{
		return ((this->*m_processFunction)(view));
	}


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		return ((this->*m_fillFunction)(writeBuffer));
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
		m_fillFunctionBody = NULL;
		m_processFunction = &Response::mf_processBodyNone;
		m_pendingWrite.clear();
		m_status = ResponseStatus::WAITING;
		m_staticReadCounter = 0;
		m_file.reset();
		if (m_cgiResponse)
		{
			Http::CgiInterface& cgiInterface =
			reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
			cgiInterface.releaseGateway(*m_cgiResponse);
		}
		m_cgiResponse = NULL;
		m_connAddress = NULL;
	}

	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{
		m_connAddress = &addr;
	}

}
