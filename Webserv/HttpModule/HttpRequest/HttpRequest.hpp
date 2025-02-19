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
# include "../../GenericUtils/Buffer/Buffer.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"
# include "../../ServerContext/ServerContext.hpp"

// Forward declarations
class BaseBuffer;
class BufferView;
class ServerContext;

typedef std::map<Http::RequestData::HeaderKey, Http::RequestData::HeaderValue> headerContainer;

struct ChunkInfo
{
	size_t size;
	size_t headerEnd;
};

namespace Http
{
	class Request
	{
		public:
			Request(ServerContext &serverContext);
			~Request();

			Request(const Request& copy);
			Request& operator=(const Request& assign);

			void	reset(); // reset the request to its initial state

			// Main parsing interface
			void parse(const BaseBuffer& buffer);

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
			bool isStarted() const;
			bool isIncompleted() const;
			bool isCompleted() const;
			bool isError() const;

			const ParsingState &getParsingState() const;

			// http status for this request
			Http::Status::Number                        getStatus() const;

			// Getters for request data components
			const std::string& 							getMethod() const;
			const std::string& 							getUri() const;
			const std::string& 							getHttpVersion() const;
			const headerContainer&						getHeaders() const;
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

			// all global module's context
			ServerContext&					m_serverContext;

			// internal parsing state
			ParsingState					m_parsingState;

			// Components
			Http::RequestData 				m_data;	// holds request data

			// main parsers
			Http::Status::Number			mf_parseRequestLine(const BufferView& buffer);
			Http::Status::Number			mf_parseHeaders(const BufferView& buffer);
			Http::Status::Number			mf_parseBody(const BufferView& buffer);

			// Helper functions for parsing
			void							mf_handleRequestLine(const BufferView& buffer);
			void							mf_handleHeaders(const BufferView& buffer);
			void							mf_handleBody(const BufferView& buffer);

			Http::Status::Number			mf_parseUriComponents(const std::string& uri);
			std::string						mf_decodeUriComp(const std::string& encoded) const;

			ChunkInfo						mf_parseChunkHeader(const std::string& data, size_t pos);
			Http::Status::Number			mf_parseChunkedBody(const std::string& data);
			bool							mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos,std::string& assembled_body);
			Http::Status::Number			mf_parseMultipartData(const std::string& data);
			Http::Status::Number			mf_parseRegularBody(const std::string& data);
			Http::RequestData::BodyType		mf_bodyType();
			Http::RequestData::ContentType	mf_contentType();
	};

}; // namespace Http

#endif
