/* *********************************/
/*                                 */
/*   HttpRequest.hpp               */
/*   - defines the HttpRequest      */
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

// Forward declarations
class HttpConnection;

struct ChunkInfo {
    size_t size;
    size_t headerEnd;
};

class HttpRequest {
    public:
        enum ParsingState
        {
            IDLE,
            STARTED,
            ERROR,
            INCOMPLETE,
            COMPLETED
        };

        HttpRequest();
        ~HttpRequest();

        // Main parsing interface
        int parse(const std::string& rawData);

        // parsing states, as a state machine
        bool isStarted() const;
        bool isError() const;
        bool isIncomplete() const;
        bool isCompleted() const;

        const ParsingState& getParsingState() const;

        // http status for this request
        int getStatus() const;

        // Getters for components
        const std::string& 							getMethod() const;
        const std::string& 							getUri() const;
        const std::string& 							getHttpVersion() const;
        const std::map<std::string, std::string>&	getHeaders() const;
        const std::string& 							getBody() const;
        const std::map<std::string, std::string>&   getUriComponents() const;

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
        int mf_parseRequestLine(const std::string& line);
        int mf_parseHeaders(const std::string& line);
        int mf_parseBody(const std::string& data);

        // Validations
        bool mf_validateMethod() const;
        bool mf_validateUri() const;
        bool mf_validateHttpVersion() const;
        bool mf_validateHeaders() const;

        // internal variables
        int m_status;
        size_t m_timeout;
        HttpConnection* m_httpConn;
        ParsingState m_parsingState;

        // Components
        std::string m_method;
        std::string m_uri;
        std::string m_httpVersion;
        std::map<std::string, std::string> m_headers;
        std::map<std::string, std::string> m_uriComponents;

        // data sent by client, also used as internal buffer
        std::string m_body;

        // Prevent copying
        HttpRequest(const HttpRequest&);
        HttpRequest& operator=(const HttpRequest&);

        // Helper functions for parsing
        int mf_parseFirstIncomming(const std::string& rawData);
        int mf_handleStreamedBody(const std::string& rawData);
        int mf_handlePostBody(const std::string& rawData, size_t bodyStart);

        int mf_parseUriComponents(const std::string& uri);
        std::string mf_decodeUri(const std::string& encoded, bool strict = true) const;

        ChunkInfo mf_parseChunkHeader(const std::string& data, size_t pos);
        int mf_parseChunkedBody(const std::string& data);
        bool mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos,std::string& assembled_body);
        int mf_parsePlainBody(const std::string& data);
};

#endif
