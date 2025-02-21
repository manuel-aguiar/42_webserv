

#ifndef HTTP_CGIGATEWAY_HPP

# define HTTP_CGIGATEWAY_HPP

# include "../../CgiModule/CgiModule.h"
# include "../../Ws_Namespace.h"
# include "../HttpResponse/HttpResponse.hpp"
# include "../HttpRequest/HttpRequest.hpp"

namespace Http { class Connection; class Request; class Response;}

namespace Http
{
	class CgiGateway
	{
		public:
			CgiGateway(Cgi::Module& module, Http::Request& request, Http::Response& response);
			~CgiGateway();


			void			reset();

			Response::Status 	fillWriteBuffer(BaseBuffer& writeBuffer);



			// execution after callbacks
			void 			onSuccess();
			void 			onError();
			Cgi::IO::State 	onRead(const Ws::fd readFd);
			Cgi::IO::State 	onWrite(const Ws::fd writeFd);
			Cgi::IO::State 	onReceiveHeaders(Cgi::HeaderData& headers);

		private:

			typedef enum
			{
				IDLE,
				PROCESSING,
				RESPONSE_LINE,
				HEADERS,
				BODY_TEMP,
				BODY_STREAM,
				COMPLETE,
				ERROR,
			}	ResponseState;

			Cgi::Module& 		m_module;
			Cgi::Request* 		m_cgiRequest;
			Http::Request& 		m_httpRequest;
			Http::Response& 	m_httpResponse;

			// Cgi IO
			bool 				m_canRead;
			bool 				m_canWrite;
			Ws::fd 				m_readFd;
			Ws::fd 				m_writeFd;

			int					m_statusCode;
			Cgi::HeaderData* 	m_headers;
			ResponseState		m_responseState;

			Response::Status	mf_bodyStream(BaseBuffer& writeBuffer);
			
			
			Response::Status 	mf_fillErrorResponse(BaseBuffer& writeBuffer);
			// disallow copy

			CgiGateway(const CgiGateway& other);
			CgiGateway& operator=(const CgiGateway& other);


	};


}



#endif