
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

			BufferView					receiveRequestBody(const BufferView& view);			// request send body

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
			bool						mf_validateAcceptType(const std::string& header, const std::string& path);
			void						mf_findLocation(ResponseData& responseData);
			void						mf_assembleTargetPath();
			std::string					mf_getCurrentDate();

			std::string 				mf_generateDefaultErrorPage(int statusCode, const std::string& errorMessage);
			void						mf_setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension);

			typedef Http::ResponseStatus::Type (Response::*FillFunction)(BaseBuffer& writeBuffer);
				
			Http::ResponseStatus::Type	mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillErrorResponse(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillDirectoryListing(BaseBuffer& writeBuffer);
			
			Http::ResponseStatus::Type	mf_prepareStaticFile(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_sendStaticFile(BaseBuffer& writeBuffer);
			
			// call the Cgi Gateway to fill the response
			Http::ResponseStatus::Type	mf_fillCgiResponse(BaseBuffer& writeBuffer);

			typedef BufferView (Response::*ProcessBodyFunction)(const BufferView& receivedView);
			
			BufferView					mf_processBodyIgnore(const BufferView& receivedView);	
			BufferView					mf_processBodyNone(const BufferView& receivedView);
			BufferView					mf_processBodyUpload(const BufferView& receivedView);

			// pass the body to the CgiGateway
			BufferView					mf_processBodyCgi(const BufferView& receivedView);

			// Debatable

			ServerContext&       		m_context;
			const Ws::Sock::addr*		m_connAddress;
			ResponseData				m_responseData;

			Http::ResponseStatus::Type	m_status;
			std::string					m_pendingWrite;		// cache data that you generated but couldn't write
			FillFunction				m_fillFunction;
			FillFunction				m_fillFunctionBody;
			ProcessBodyFunction			m_processFunction;
			size_t						m_staticReadCounter;
			File						m_file;
			Http::CgiGateway*			m_cgiGateway;
			std::map<std::string, std::string>::const_iterator*	m_currentHeader; // index of the current header to be writter
	};
}

#endif
