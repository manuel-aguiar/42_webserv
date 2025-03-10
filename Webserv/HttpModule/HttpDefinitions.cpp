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

/*
		const Http::RequestData*	        requestData;
		Http::Status::Number		        requestStatus;
		const ServerBlock*			        serverBlock;
		const ServerLocation*		        serverLocation;
		std::map<HeaderKey, HeaderValue>	headers;
		ResponseType				        responseType;
		std::string                         targetPath;
		std::string							targetExtension;
		FilesUtils::FileType                targetType;
		bool								indexAppended;
		std::string							errorMessage;
		bool								cgiPass;			

		bool                                closeAfterSending;
*/
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
    , errorMessage(DEFAULT_ERROR_MESSAGE)
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
    , errorMessage(copy.errorMessage)
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
		errorMessage = assign.errorMessage;
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
        errorMessage = DEFAULT_ERROR_MESSAGE;
        cgiPass = false;
        closeAfterSending = false;
		indexAppended = false;
		errorMessage.clear();
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
