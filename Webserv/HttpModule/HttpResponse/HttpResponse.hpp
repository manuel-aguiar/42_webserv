
#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// Project headers
# include "../HttpDefinitions.hpp"
# include "../HttpCgiGateway/HttpCgiGateway.hpp"
# include "../../Ws_Namespace.h"
# include "../../GenericUtils/Files/File.hpp"

// C++ headers
# include <string>

// forward declarations
class ServerBlock;
class ServerLocation;
class ServerContext;
class BaseBuffer;

namespace Http
{
	class Response
	{

		public:
			Response(ServerContext& context);
			Response(const Response& other);
			~Response();
			Response& operator=(const Response& other);

			Http::ResponseStatus::Type	fillWriteBuffer(BaseBuffer& writeBuffer); // give me all data you can, until Buffer::capacity()
			
			void    					reset(); // reset the response to its initial state

			Http::ResponseStatus::Type	getStatus() const;

			void						receiveRequestData(const Http::RequestData& data); 	// request sends headers
			void						receiveRequestBody(const BufferView& view);			// request send body

			void						setConnectionAddress(const Ws::Sock::addr& addr);	// called by http::connection

		private:

			typedef enum
			{
				NONE,
				STATIC,
				CGI,
				REDIRECT
			}	Type;


			bool						mf_validateHeaders();

			void						mf_generateResponse(int statusCode);
			std::string					mf_generateStatusLine(int statusCode);
			std::string					mf_generateHeaderString();
			std::string 				mf_generateDefaultErrorPage(int statusCode, const std::string& statusText, const std::string& errorMessage);
			void						mf_setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension);

			typedef Http::ResponseStatus::Type (Response::*FillFunction)(BaseBuffer& writeBuffer);

			Http::ResponseStatus::Type	mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillErrorResponse(BaseBuffer& writeBuffer);

			// Debatable

			ServerContext&       		m_context;
			const Ws::Sock::addr*		m_connAddress;
			ResponseData				m_responseData;

			Http::ResponseStatus::Type	m_status;
			std::string					m_pendingWrite;		// cache data that you generated but couldn't write
			FillFunction				m_fillFunction;
			File						m_file;
			Http::CgiGateway			m_cgiGateway;
	};
};

#endif
