/* *********************************/
/*                                 */
/*   HttpDefinitions.cpp            */
/*   - a namespace for whole http  */
/*   module to consume from.       */
/*                                 */
/* *********************************/

#include "HttpDefinitions.hpp"

namespace Http
{
    namespace AllowedRequestMethods
    {
        static std::set<std::string> initAllowedMethods()
        {
            std::set<std::string> methods;
            methods.insert("GET");
            methods.insert("POST");
            methods.insert("DELETE");
            return methods;
        }

        const std::set<std::string>& getAllowedMethods()
        {
            static const std::set<std::string> ALLOWED_METHODS = initAllowedMethods();
            return ALLOWED_METHODS;
        }
    }

    // Uncomment when needed
    /*namespace AllowedHeaders
    {
        static std::set<std::string> initAllowedHeaders()
        {
            std::set<std::string> headers;
            headers.insert("Content-Type");
            headers.insert("Content-Length");
            headers.insert("Host");
            headers.insert("User-Agent");
            headers.insert("Accept");
            headers.insert("Accept-Encoding");
            headers.insert("Accept-Language");
            headers.insert("Accept-Charset");
            headers.insert("Accept-Datetime");
            headers.insert("Authorization");
            headers.insert("Cookie");
            headers.insert("Set-Cookie");
            return headers;
        }

        const std::set<std::string>& getAllowedHeaders()
        {
            static const std::set<std::string> ALLOWED_HEADERS = initAllowedHeaders();
            return ALLOWED_HEADERS;
        }
    }*/
}