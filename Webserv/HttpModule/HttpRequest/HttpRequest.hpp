/* *********************************/
/*                                 */
/*   Http::Request.hpp               */
/*   - defines the Http::Request      */
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
class Buffer;
namespace Http { class Connection;} // servercontext (blockfinder), sockaddr,
class ServerBlock;
class ServerLocation;

struct ChunkInfo {
    size_t size;
    size_t headerEnd;
};

namespace Http
{
    typedef std::string HeaderKey;
    typedef std::string HeaderValue;
}

namespace Http
{
    class Request
    {
        public:
            enum ParsingState
            {
                IDLE,
                STARTED,
                ERROR,
                INCOMPLETE,
                COMPLETED
            };
            
            Request();

            Request(Http::Connection& conn); //blockfinder, Conn::Connection (Http::Connection->Conn::Connection->ServerContext->BLockfinder)
            ~Request();

            // Main parsing interface

            int parse(const Buffer& buffer);

            int parse(const std::string& rawData);

            // parsing states, as a state machine
            bool isStarted() const;
            bool isError() const;
            bool isIncomplete() const;
            bool isCompleted() const;

            const ParsingState& getParsingState() const;

            ServerBlock*    getServerBlock() const; //Http::connection needs, http timeouts
            ServerLocation* getServerLocation() const;

            void            done(); // inform Http::Connection this request and response can be removed from queue

            // http status for this request
            int                                         getStatus() const;

            // Getters for components
            const std::string& 							getMethod() const;
            const std::string& 							getUri() const;
            const std::string& 							getHttpVersion() const;
            const std::map<HeaderKey, HeaderValue>&	    getHeaders() const;
            const std::string& 							getBody() const;
            const std::map<std::string, std::string>&   getUriComponents() const;

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
            int mf_parseRequestLine(const std::string& line);
            int mf_parseHeaders(const std::string& line);
            int mf_parseBody(const std::string& data);

            // internal variables
            int m_status;
            size_t m_timeout;
            Http::Connection& m_httpConn;
            ParsingState m_parsingState;

            // Components
            std::string m_method;
            std::string m_uri;
            std::string m_httpVersion;
            std::map<HeaderKey, HeaderValue> m_headers;
            std::map<std::string, std::string> m_uriComponents;

            std::string m_path;
            std::string m_queryString; //no decoding/breaking down -> cgi module (responbility of the script)
            std::string m_fragment;

            // data sent by client, also used as internal buffer
            std::string m_body;

            ServerBlock*        m_serverBlock;      // ServerBlock specific http specs
            ServerLocation*     m_serverLocation;

            // Prevent copying
            Request(const  Request& copy);
            Request& operator=(const Request& assign);

            // Helper functions for parsing
            int mf_parseFirstIncomming(const std::string& rawData);
            int mf_handleStreamedBody(const std::string& rawData);
            int mf_handlePostBody(const std::string& rawData, size_t bodyStart);

            int mf_parseUriComponents(const std::string& uri);
            std::string mf_decodeUri(const std::string& encoded, bool strict = true) const;

            ChunkInfo mf_parseChunkHeader(const std::string& data, size_t pos);
            int mf_parseChunkedBody(const std::string& data);
            bool mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos,std::string& assembled_body);
            int mf_parseRegularBody(const std::string& data);

            //
            bool mf_findBlock(); //internally checks Blockfinder for the block true:ok false:error,
    };

}; // namespace Http

#endif
