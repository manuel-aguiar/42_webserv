/* *********************************/
/*                                 */
/*   HttpDefinitions.hpp            */
/*   - a namespace for whole http  */
/*   module to consume from.       */
/*                                 */
/* *********************************/

#ifndef HTTP_DEFINITIONS_HPP
#define HTTP_DEFINITIONS_HPP

# include <string>
# include <set>
# include <map>

# include "../GenericUtils/Files/FilesUtils.hpp"

class ServerBlock;
class ServerLocation;

namespace Http
{
    // All http status codes
    namespace Status
    {
        typedef enum
        {
            _TOTAL_CODES = 37,                    // UPDATE THIS NUMBER HERE IF YOU ADD MORE STATUS CODES (no c++98 way to count enums at compile time)


			// 2xx
			OK = 200,
			CREATED = 201,
			ACCEPTED = 202,
			NO_CONTENT = 204,
			RESET_CONTENT = 205,
			PARTIAL_CONTENT = 206,

			// 3xx
			MULTIPLE_CHOICES = 300,
			MOVED_PERMANENTLY = 301,
			FOUND = 302,
			SEE_OTHER = 303,
			NOT_MODIFIED = 304,
			TEMPORARY_REDIRECT = 307,
			PERMANENT_REDIRECT = 308,

            // 4xx
            BAD_REQUEST = 400,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            METHOD_NOT_ALLOWED = 405,
			NOT_ACCEPTABLE = 406,
			LENGTH_REQUIRED = 411,
            URI_TOO_LONG = 414,
            PRECONDITION_FAILED = 412,
            PAYLOAD_TOO_LARGE = 413,
            UNSUPPORTED_MEDIA_TYPE = 415,
            TOO_MANY_REQUESTS = 429,
            REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
            UNAVAILABLE_FOR_LEGAL_REASONS = 451,

			// 5xx
			INTERNAL_ERROR = 500,
			NOT_IMPLEMENTED = 501,
			BAD_GATEWAY = 502,
			SERVICE_UNAVAILABLE = 503,
			GATEWAY_TIMEOUT = 504,
			HTTP_VERSION_NOT_SUPPORTED = 505,
			VARIANT_ALSO_NEGOTIATES = 506,
			INSUFFICIENT_STORAGE = 507,
			LOOP_DETECTED = 508,
			NOT_EXTENDED = 510,
			NETWORK_AUTHENTICATION_REQUIRED = 511
		}   Number;
	}


	// standard values for http 1.1
	namespace HttpStandard
	{
		const size_t MAX_URI_LENGTH = 2048;
		const size_t MAX_HEADERS_LENGTH = 8192;
		const size_t MAX_BODY_LENGTH = 1024 * 1024; // 1MB
		const size_t MAX_CHUNK_SIZE = 1024 * 1024; // 1MB
		const std::string HTTP_VERSION = "HTTP/1.1"; // Supported http versions
	}

	// Supported request methods
	namespace AllowedRequestMethods
	{
		const std::set<std::string>& getAllowedMethods();
	}


	namespace ResponseStatus
	{
		typedef enum
		{
			WAITING, 				// have nothing to push, no sign transaction is finished
			WRITING, 				// pushed data to buffer
			FINISHED,				// transaction is finished
			MARK_TO_CLOSE 			// tell the Http::Connection to close the connection after writing
		}	Type;
	}

	struct RequestData
	{
		RequestData();
		~RequestData();
		RequestData(const RequestData& copy);
		RequestData& operator=(const RequestData& assign);

		void reset();

		enum BodyType {
			NONE,
			REGULAR,
			CHUNKED
		};

		enum ContentType {
			RAW,
			MULTIPART
		};

		typedef std::string HeaderKey;
		typedef std::string HeaderValue;
		typedef std::map<Http::RequestData::HeaderKey, Http::RequestData::HeaderValue> headerContainer;

		std::string							method;
		std::string							uri;
		std::string							path;
		std::string							queryString;
		std::string							fragment;
		std::string							httpVersion;
		std::map<HeaderKey, HeaderValue>	headers;
		std::string							body;
		std::string							multipart_Boundary;
		std::string							multipart_Name;
		std::string							multipart_Filename;
		Status::Number						status;
		BodyType							bodyType;
		ContentType							contentType;
		size_t								expectedLength;

	};


	struct ResponseData
	{
		ResponseData();
		~ResponseData();
		ResponseData(const ResponseData& copy);
		ResponseData& operator=(const ResponseData& assign);

		void reset();

		typedef enum
		{
			UNDEFINED,
			STATIC,
			CGI,
			REDIRECT,
			DIRECTORY_LISTING,
			FILE_UPLOAD,
			ERROR,
			NO_CONTENT
		}	ResponseType;

		typedef std::string HeaderKey;
		typedef std::string HeaderValue;

		const Http::RequestData*	        requestData;
		Http::Status::Number		        requestStatus;
		const ServerBlock*			        serverBlock;
		const ServerLocation*		        serverLocation;
		std::map<HeaderKey, HeaderValue>	headers;
		ResponseType				        responseType;
		std::string                         targetPath;
		std::string							targetExtension;
		FilesUtils::FileType                targetType;

		bool                                closeAfterSending;

	};

}

#endif
