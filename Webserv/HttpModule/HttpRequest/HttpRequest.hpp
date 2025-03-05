/* *********************************/
/*                                 */
/*   Http::Request.hpp             */
/*   - defines the Http::Request    */
/*    class.                       */
/*                                 */
/* *********************************/

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C++ Headers
# include <string>
# include <map>
# include <set>
# include <iostream>
# include "../HttpDefinitions.hpp"
// # include "../HttpResponse/HttpResponse.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"

// Forward declarations
class BaseBuffer;
class BufferView;
class ServerContext;

namespace Http { class Response;}

namespace Http
{
	class Request
	{
		public:
			Request(ServerContext &serverContext);
			~Request();

			Request(const Request& copy);
			Request& operator=(const Request& assign);

			void										reset(); // reset the request to its initial state
			void										close();

			// Main parsing interface
			Http::IOStatus::Type						read();
			Http::IOStatus::Type 						parse();

			// my response
			Http::Response&								getResponse();
			void										setResponse(Http::Response& response);
			void										setBuffer_ReadFd(BaseBuffer& buffer, const Ws::fd fd);

			const Http::RequestData&					getData() const;

			enum ParsingState
			{
				IDLE,
				REQLINE,
				HEADERS,
				BODY,
				COMPLETED,
				ERROR
			};

			// parsing states (computed)
			bool 										isStarted() const;
			bool 										isIncompleted() const;
			bool 										isCompleted() const;
			bool 										isError() const;

			const ParsingState&							getParsingState() const;

			// http status for this request
			Http::Status::Number                        getStatus() const;

			// Getters for request data components
			const std::string& 							getMethod() const;
			const std::string& 							getUri() const;
			const std::string& 							getHttpVersion() const;
			const RequestData::headerContainer&			getHeaders() const;
			const std::string& 							getBody() const;

			const std::string&                          getPath() const;
			const std::string&                          getQueryString() const;
			const std::string&                          getFragment() const;

			// Encoding exception
			class EncodingException : public std::exception {
				public:
					EncodingException(const std::string& message) : m_message(message) {}
					virtual ~EncodingException() throw() {}
					virtual const char* what() const throw() { return m_message.c_str(); }
				private:
					std::string m_message;
				};

		private:
			Request();

			typedef BufferView (Request::*ParsingFunction)(const BufferView& currentView);
			// all global module's context
			ServerContext&								m_serverContext;

			// my response
			Http::Response*								m_httpResponse;
			BaseBuffer*									m_readBuffer;
			Ws::fd										m_readFd;

			// internal parsing state
			ParsingState								m_parsingState;

			// parseHandler functions
			ParsingFunction								m_parsingFunction;

			// Components
			Http::RequestData 							m_data;	// holds request data

			// state helpers
			int											m_findPivot;
			int											m_curChunkSize;
			int											m_curChunkPos;
			int											m_curContentLength;
			int											m_curContentPos;

			// Parsing functions, return  BufferView of the unconsumed part
			BufferView									mf_handleNothing		(const BufferView& currentView);
			BufferView									mf_handleRequestLine	(const BufferView& currentView);
			BufferView									mf_handleHeaders		(const BufferView& currentView);

			// distributes game to the body parsers
			BufferView									mf_prepareBodyParser(const BufferView& receivedView);

			BufferView									mf_parseLengthBody		(const BufferView& currentView);

			// chunked body intermediaries......
			BufferView									mf_parseChunkedBody_GetChunk	(const BufferView& currentView);
			BufferView									mf_parseChunkedBody_ParseChunk	(const BufferView& currentView);
			BufferView									mf_parseChunkedBody_EndChunk	(const BufferView& currentView);

			// multi part intermediaries........
			BufferView 									mf_parseMultipartBody_Start		(const BufferView& currentView);
			BufferView 									mf_parseMultipartBody_Headers	(const BufferView& currentView);
			BufferView 									mf_parseMultipartBody_Content	(const BufferView& currentView);
			BufferView 									mf_parseMultipartBody_End		(const BufferView& currentView);

			BufferView									mf_parseBodyExitError	(const BufferView& remaining, const Http::Status::Number status);

			// main parsers
			Http::Status::Number						mf_parseRequestLine	(const BufferView& currentView);
			Http::Status::Number						mf_parseHeaders		(const BufferView& currentView);
			Http::Status::Number						mf_parseBody		(const BufferView& currentView);
			



			Http::Status::Number						mf_parseUriComponents(const std::string& uri);
			std::string									mf_decodeUriComp(const std::string& encoded) const;

			Http::Status::Number						mf_parseChunkedBody(const std::string& data);
			Http::Status::Number						mf_parseMultipartData(const std::string& data);
			Http::Status::Number						mf_parseLengthBody(const std::string& data);
			Http::RequestData::BodyType					mf_bodyType();
			Http::RequestData::ContentType				mf_contentType();


			size_t										mf_findHeaderEnd(const BufferView& currentView);
			void										mf_handleExitFailure(Http::Status::Number status);
			BufferView									mf_handleExitFailure(BufferView& remaining, Http::Status::Number status);
	};

} // namespace Http

#endif
