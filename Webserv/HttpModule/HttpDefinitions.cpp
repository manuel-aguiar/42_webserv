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

    ResponseData::ResponseData() 
    : requestData(NULL)
    , requestStatus(Http::Status::OK)
    , serverBlock(NULL)
    , serverLocation(NULL)
    , headers()
    , responseType(UNDEFINED)
    , targetPath()
    , targetType(FilesUtils::UNDEFINED),
    , closeAfterSending(false) {}

    ResponseData::~ResponseData() {}
    ResponseData::ResponseData(const ResponseData& copy)
    : requestData(copy.requestData)
    , requestStatus(copy.requestStatus)
    , serverBlock(copy.serverBlock)
    , serverLocation(copy.serverLocation)
    , headers(copy.headers)
    , responseType(copy.responseType)
    , targetPath(copy.targetPath)
    , targetType(copy.targetType)
    , closeAfterSending(copy.closeAfterSending)
     {}

    ResponseData& ResponseData::operator=(const ResponseData& assign)
    {
        if (this == &assign)
            return (*this);

        requestData = assign.requestData;
        requestStatus = assign.requestStatus;
        serverBlock = assign.serverBlock;
        serverLocation = assign.serverLocation;
        headers = assign.headers;
        responseType = assign.responseType;
        targetPath = assign.targetPath;
        targetType = assign.targetType;
        closeAfterSending = assign.closeAfterSending;

        return (*this);
    }

    void
    ResponseData::reset()
    {
        requestData = NULL;
        requestStatus = Http::Status::OK;
        serverBlock = NULL;
        serverLocation = NULL;
        headers.clear();
        responseType = UNDEFINED;
        targetPath.clear();
        targetType = FilesUtils::UNDEFINED;
        closeAfterSending = false;
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