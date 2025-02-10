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
# include "../../GenericUtils/Files/FilesUtils.hpp"

// Forward declarations
class HttpConnection;
class HttpSession;

class HttpRequest {
    public:
        HttpRequest();
        ~HttpRequest();

        // Main parsing interface
        int parse(const std::string& rawData);

		/* IDEA: instead of returning int, parse stores an "errcode value that when
		 *	generateResponse() is called it is checked and if it isnt the default value,
		 *	instantly generates the error " */
		int generateResponse();

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
        // Parsers
        int mf_parseRequestLine(const std::string& line);
        int mf_parseHeaders(const std::string& line);
        int mf_parseBody(const std::string& data);
        int mf_parseChunkedBody(const std::string& data);

        // Validations
        bool mf_validateMethod() const;
        bool mf_validateUri() const;
        bool mf_validateHttpVersion() const;
        bool mf_validateHeaders() const;

        // States
        int m_status;
        size_t m_timeout;

        // Connections
        HttpConnection* m_httpConn;
        HttpSession* m_session;

        // Components
        std::string m_method;
        std::string m_uri;
        std::string m_httpVersion;
        std::map<std::string, std::string> m_headers;
        std::map<std::string, std::string> m_uriComponents;
        std::string m_body;

        // Prevent copying
        HttpRequest(const HttpRequest&);
        HttpRequest& operator=(const HttpRequest&);

        // Helper functions for parsing
        int mf_parseUriComponents(const std::string& uri);
        std::string mf_decodeUri(const std::string& encoded, bool strict = true) const;
};

#endif
