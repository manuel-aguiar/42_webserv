

# include "HttpResponse.hpp"

namespace Http
{
    Http::ResponseStatus::Type
    Response::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillResponseLine(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillHeaders(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus::Type
    Response::mf_fillErrorResponse(BaseBuffer& writeBuffer)
    {
        // NOT IMPLEMENTED YET
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }
}