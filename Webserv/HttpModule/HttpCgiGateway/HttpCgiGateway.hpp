

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

			bool					initiateRequest(const Http::ResponseData& data);

			Http::ResponseStatus::Type
									fillWriteBuffer(BaseBuffer& writeBuffer);

			BufferView				sendHttpBody(const BufferView& view);

			// execution after callbacks
			void 					onCgiSuccess();
			void 					onCgiError();
			Cgi::IO::State 			onCgiRead(const Ws::fd readFd);
			Cgi::IO::State 			onCgiWrite(const Ws::fd writeFd);
			Cgi::IO::State 			onCgiReceiveHeaders(const Cgi::HeaderData& headers);

		private:
			
			typedef Http::ResponseStatus::Type 	(CgiGateway::*FillFunction)(BaseBuffer& writeBuffer);
			typedef BufferView					(CgiGateway::*ProcessHttpBody)(const BufferView& view);

			Http::ResponseStatus::Type	mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyTemp(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillErrorResponse(BaseBuffer& writeBuffer);

			BufferView					mf_HttpBodyNone(const BufferView& view);
			BufferView					mf_HttpBodyIgnore(const BufferView& view);
			BufferView					mf_HttpBodySend(const BufferView& view);	

			Cgi::Module& 				m_module;
			Cgi::Request* 				m_cgiRequest;

			bool 						m_canRead;
			bool 						m_canWrite;
			Ws::fd 						m_readFd;
			Ws::fd 						m_writeFd;

			int							m_statusCode;
			const Cgi::HeaderData* 		m_headers;
			int							m_currentHeader;

			ProcessHttpBody				m_processHttpBody;
			FillFunction				m_fillFunction;


			CgiGateway(const CgiGateway& other);
			CgiGateway& operator=(const CgiGateway& other);


	};


}

#endif
