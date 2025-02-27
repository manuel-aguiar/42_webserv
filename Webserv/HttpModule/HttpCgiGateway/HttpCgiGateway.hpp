

#ifndef HTTP_CGIGATEWAY_HPP

# define HTTP_CGIGATEWAY_HPP

# include "../../CgiModule/CgiModule.h"
# include "../../Ws_Namespace.h"
# include "../HttpDefinitions.hpp"
# include "../HttpDefinitions.hpp"

# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"

namespace Http { class Response; }

namespace Conn { class Connection; }

namespace Http
{
	class CgiGateway
	{
		public:
			CgiGateway(Cgi::Module& module);
			~CgiGateway();

			void					reset();
			void					close();

			bool					initiateRequest(const Http::RequestData& data, 
													const Conn::Connection* connection,
													const std::string& script,
													const std::string& interpreterPath);

			Http::ResponseStatus::Type
									fillWriteBuffer(BaseBuffer& writeBuffer);

			BufferView				receiveRequestBody(const BufferView& view);

			// execution after callbacks
			void 					onSuccess();
			void 					onError();
			Cgi::IO::State 			onRead(const Ws::fd readFd);
			Cgi::IO::State 			onWrite(const Ws::fd writeFd);
			Cgi::IO::State 			onReceiveHeaders(const Cgi::HeaderData& headers);

		private:
			
			typedef Http::ResponseStatus::Type (CgiGateway::*FillFunction)(BaseBuffer& writeBuffer);

			Http::ResponseStatus::Type	mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyTemp(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillErrorResponse(BaseBuffer& writeBuffer);

			Cgi::Module& 			m_module;
			Cgi::Request* 			m_cgiRequest;

			// Cgi IO
			bool 					m_canRead;
			bool 					m_canWrite;
			Ws::fd 					m_readFd;
			Ws::fd 					m_writeFd;

			int						m_statusCode;
			const Cgi::HeaderData* 	m_headers;
			int						m_currentHeader;
			FillFunction			m_fillFunction;


			CgiGateway(const CgiGateway& other);
			CgiGateway& operator=(const CgiGateway& other);


	};


}

#endif
