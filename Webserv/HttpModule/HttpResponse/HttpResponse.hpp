#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../HttpDefinitions.hpp"
# include "../../GenericUtils/Files/File.hpp"

// TESTING
# include "../../ServerContext/ServerContext.hpp"
# include "../HttpCgiGateway/HttpCgiGateway.hpp"

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

			Http::ResponseStatus		fillWriteBuffer(BaseBuffer& writeBuffer); // give me all data you can, until Buffer::capacity()
			
			void    					reset(); // reset the response to its initial state

			Http::ResponseStatus		getStatus() const;

			void						receiveRequestData(const Http::RequestData& data); 	// request sends headers
			void						receiveRequestBody(const BufferView& view);			// request send body

		private:

			typedef enum
			{
				NONE,
				STATIC,
				CGI,
				REDIRECT
			}	Type;


			void						mf_validateHeaders();
			void						mf_generateResponse(int statusCode);
			std::string					mf_generateStatusLine(int statusCode);
			std::string					mf_generateHeaderString();
			std::string 				mf_generateDefaultErrorPage(int statusCode, const std::string& statusText, const std::string& errorMessage);

			typedef Http::ResponseStatus (Response::*FillFunction)(BaseBuffer& writeBuffer);

			Http::ResponseStatus		mf_fillNothingToSend(BaseBuffer& writeBuffer);
			Http::ResponseStatus		mf_fillResponseLine(BaseBuffer& writeBuffer);
			Http::ResponseStatus		mf_fillHeaders(BaseBuffer& writeBuffer);
			Http::ResponseStatus		mf_fillBodyStream(BaseBuffer& writeBuffer);
			Http::ResponseStatus		mf_fillErrorResponse(BaseBuffer& writeBuffer);

			// Debatable
			void						setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension);

			ServerContext&       		m_context;
			const Http::RequestData*	m_requestData;
			const ServerBlock*			m_serverBlock;
			const ServerLocation*		m_location;
			std::map<std::string, std::string>
										m_headers;

			std::string					m_pendingWrite;		// cache data that you generated but couldn't write

			Response::Type				m_type;
			Http::ResponseStatus		m_status;
			FillFunction				m_fillFunction;
			File						m_file;
			Http::CgiGateway			m_cgiGateway;
	};
};

#endif
