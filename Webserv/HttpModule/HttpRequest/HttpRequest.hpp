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
#include "../../GenericUtils/Buffer/Buffer.hpp"
#include "../../GenericUtils/BufferView/BufferView.hpp"

// Forward declarations
class BaseBuffer;
class BufferView;

namespace Http { class Connection; } // servercontext (blockfinder), sockaddr,
class ServerBlock;
class ServerLocation;

// TODOs:
// Correct handling for the buffer
// Chunk transfer parsing
// Streaming to disk
// Finding block/location based on host (needs expansion)
// Validating request components against the configs

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
			enum ParsingState
			{
				IDLE,
				REQLINE,
				HEADERS,
				BODY,
				COMPLETED,
				ERROR
			};

			Request();
			Request(Http::Connection& conn); //blockfinder, Conn::Connection (Http::Connection->Conn::Connection->ServerContext->BLockfinder)
			~Request();

			Request(const Request& copy);
			Request& operator=(const Request& assign);

			void	reset(); // reset the request to its initial state
			void	done(); // inform Http::Connection this request and response can be removed from queue

			// Main parsing interface
			void parse(const BaseBuffer& buffer);

			// parsing states (computed)
			bool isStarted() const;
			bool isIncompleted() const;
			bool isCompleted() const;
			bool isError() const;

			const ParsingState& getParsingState() const;

			ServerBlock*    getServerBlock() const; //Http::connection needs, http timeouts
			ServerLocation* getServerLocation() const;

			// http status for this request
			Http::Status::Number                        getStatus() const;

			// Getters for components
			const std::string& 							getMethod() const;
			const std::string& 							getUri() const;
			const std::string& 							getHttpVersion() const;
			const std::map<HeaderKey, HeaderValue>&	    getHeaders() const;
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
			// main parsers
			Http::Status::Number	mf_parseRequestLine(const BufferView& buffer);
			Http::Status::Number	mf_parseHeaders(const BufferView& buffer);
			Http::Status::Number	mf_parseBody(const BufferView& buffer);

			// internal variables
			Http::Connection&						m_httpConn;
			ServerBlock*							m_serverBlock;
			ServerLocation*							m_serverLocation;

			Http::Status::Number    				m_status;
			size_t                  				m_timeout;
			ParsingState							m_parsingState;

			// Components
			std::string								m_method;
			std::string								m_uri;
			std::string								m_httpVersion;
			std::map<HeaderKey, HeaderValue>		m_headers;
			std::string								m_path;
			std::string								m_queryString;
			std::string								m_fragment;

			enum BodyType {
				NONE,
				REGULAR,
				CHUNKED
			};

			enum ContentType {
				RAW,
				MULTIPART
			};

			// data sent by client, also used as internal buffer
			std::string								m_body;
			BodyType								m_bodyType;
			ContentType								m_contentType;
			size_t									m_expectedLength;

			// Helper functions for parsing
			void		mf_handleRequestLine(const BufferView& buffer);
			void		mf_handleHeaders(const BufferView& buffer);
			void		mf_handleBody(const BufferView& buffer);

			Http::Status::Number		mf_parseUriComponents(const std::string& uri);
			std::string					mf_decodeUriComp(const std::string& encoded) const;

			ChunkInfo					mf_parseChunkHeader(const std::string& data, size_t pos);
			Http::Status::Number		mf_parseChunkedBody(const std::string& data);
			bool						mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos,std::string& assembled_body);
			Http::Status::Number		mf_parseMultipartData(const std::string& data);
			Http::Status::Number		mf_parseRegularBody(const std::string& data);
			Http::Request::BodyType		mf_bodyType();
			Http::Request::ContentType	mf_contentType();

			Http::Status::Number		mf_findBlock(); //internally checks Blockfinder for the block true:ok false:error,
	};

}; // namespace Http

#endif
