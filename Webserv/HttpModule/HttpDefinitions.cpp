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
    , targetExtension()
    , targetType(FilesUtils::UNDEFINED)
    , indexAppended(false)
    , cgiPass(false)
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
    , targetExtension(copy.targetExtension)
    , targetType(copy.targetType)
    , indexAppended(copy.indexAppended)
    , cgiPass(copy.cgiPass)
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
        targetExtension = assign.targetExtension;
        targetType = assign.targetType;
        closeAfterSending = assign.closeAfterSending;
		indexAppended = assign.indexAppended;
        cgiPass = assign.cgiPass;
	
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
        targetExtension.clear();
        targetType = FilesUtils::UNDEFINED;
        cgiPass = false;
        closeAfterSending = false;
		indexAppended = false;
    }
///////////////////////////////////////////////////////////

    RequestData::RequestData()
        : method()
        , uri()
        , path()
        , queryString()
        , fragment()
        , httpVersion()
        , headers()
        , body()
        , status(Http::Status::OK)
        , bodyType(Http::RequestData::NONE)
        , contentType(Http::RequestData::RAW)
        , expectedLength(0) {}

    RequestData::~RequestData() {}
    RequestData::RequestData(const RequestData& copy)
        : method(copy.method)
        , uri(copy.uri)
        , path(copy.path)
        , queryString(copy.queryString)
        , fragment(copy.fragment)
        , httpVersion(copy.httpVersion)
        , headers(copy.headers)
        , body(copy.body)
        , status(copy.status)
        , bodyType(copy.bodyType)
        , contentType(copy.contentType)
        , expectedLength(copy.expectedLength) {}

    RequestData& RequestData::operator=(const RequestData& assign)
    {
        if (this == &assign)
            return (*this);

        method = assign.method;
        uri = assign.uri;
        path = assign.path;
        queryString = assign.queryString;
        fragment = assign.fragment;
        httpVersion = assign.httpVersion;
        headers = assign.headers;
        body = assign.body;
        status = assign.status;
        bodyType = assign.bodyType;
        contentType = assign.contentType;
        expectedLength = assign.expectedLength;

        return (*this);
    }

    void RequestData::reset()
    {
        method.clear();
        uri.clear();
        path.clear();
        queryString.clear();
        fragment.clear();
        httpVersion.clear();
        headers.clear();
        body.clear();
        status = Http::Status::OK;
        bodyType = Http::RequestData::NONE;
        contentType = Http::RequestData::RAW;
        expectedLength = 0;
    }
}
