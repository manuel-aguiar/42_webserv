/* *********************************/
/*                                 */
/*   HttpDefinitions.hpp            */
/*   - a namespace for whole http  */
/*   module to consume from.       */
/*                                 */
/* *********************************/

#ifndef HTTP_DEFINITIONS_HPP
#define HTTP_DEFINITIONS_HPP

#include <string>
#include <set>

namespace Http
{
    // All http status codes
    namespace Status
    {
        enum
        {
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
            METHOD_NOT_ALLOWED = 405,
            URI_TOO_LONG = 414,
            HEADERS_TOO_LARGE = 431,
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
        };
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

    // Supported Headers, un comment when used
    // namespace AllowedHeaders
    // {
    //     const std::set<std::string>& getAllowedHeaders();
    // }
}

#endif