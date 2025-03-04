
#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// Project headers
# include "../HttpDefinitions.hpp"
# include "../HttpCgiResponse/HttpCgiResponse.hpp"
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
			void						close(); // close the response, no more data to be sent

			Http::ResponseStatus::Type	getStatus() const;
			ResponseData				getResponseData() const; // mainly for testing

			void						receiveRequestData(const Http::RequestData& data); 	// request sends headers

			BufferView					receiveRequestBody(const BufferView& view);			// request send body

			void						setListenAddress(const Ws::Sock::addr& addr);	// called by http::connection
			void						setTcpConnection(const Conn::Connection& addr);

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
			bool						mf_checkRedirect();
			bool						mf_assembleTargetPath();
			std::string					mf_getCurrentDate();

			std::string					mf_generateRedirectPage(int statusCode, const std::string& redirectPath);
			std::string 				mf_generateDefaultErrorPage(int statusCode, const std::string& errorMessage);
			std::string					mf_generateEtag(File& file, time_t last_modified);
			void						mf_setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension);

			typedef Http::ResponseStatus::Type (Response::*FillFunction)(BaseBuffer& writeBuffer);

			Http::ResponseStatus::Type	mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillRedirect(BaseBuffer& writeBuffer);
			Http::ResponseStatus::Type	mf_fillDefaultPage(BaseBuffer& writeBuffer);

			void						mf_addContentHeaders(const size_t size, const std::string mimeType);
			bool						mf_addCacheControlHeaders();
			bool						mf_addHeader(const std::string& key, const std::string& value);
			Http::ResponseStatus::Type	mf_fillFinish();

			bool						mf_prepareStaticFile(const char* path);

			Http::ResponseStatus::Type	mf_sendStaticFile(BaseBuffer& writeBuffer);

			// call the Cgi Gateway to fill the response
			Http::ResponseStatus::Type	mf_fillCgiResponse(BaseBuffer& writeBuffer);

			typedef BufferView (Response::*ProcessBodyFunction)(const BufferView& receivedView);

			BufferView					mf_processBodyIgnore(const BufferView& receivedView);
			BufferView					mf_processBodyNone(const BufferView& receivedView);
			BufferView					mf_processBodyUpload(const BufferView& receivedView);

			// pass the body to the CgiGateway
			BufferView					mf_processBodyCgi(const BufferView& receivedView);
			
			void						mf_prepareErrorMessage();
			void						mf_prepareCgiExecution();
			// Debatable

			ServerContext&       		m_context;
			const Ws::Sock::addr*		m_listenAddress;
			const Conn::Connection*		m_tcpConn;
			ResponseData				m_responseData;

			Http::ResponseStatus::Type	m_status;
			std::string					m_pendingWrite;		// cache data that you generated but couldn't write
			std::string					m_defaultPageContent; // Load the default pages in here (Directory Listing, Error Page)

			FillFunction				m_fillFunction;
			FillFunction				m_fillFunctionBody;
			ProcessBodyFunction			m_processFunction;

			size_t						m_staticReadCounter;
			File						m_file;
			Http::CgiResponse*			m_cgiResponse;
			std::map<std::string, std::string>::iterator	m_currentHeader; // index of the current header to be writter
	};
}

#endif
