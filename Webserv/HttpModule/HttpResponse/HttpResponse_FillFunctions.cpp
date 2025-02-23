

# include "HttpResponse.hpp"

namespace Http
{
    Http::ResponseStatus
    Response::mf_fillNothingToSend(BaseBuffer& writeBuffer)
    {
        (void)writeBuffer;
        return (Http::ResponseStatus::WAITING);
    }

    Http::ResponseStatus
    Response::mf_fillResponseLine(BaseBuffer& writeBuffer)
    {

    }

    Http::ResponseStatus
    Response::mf_fillHeaders(BaseBuffer& writeBuffer)
    {

    }

    Http::ResponseStatus
    Response::mf_fillBodyStream(BaseBuffer& writeBuffer)
    {

    }

    Http::ResponseStatus
    Response::mf_fillErrorResponse(BaseBuffer& writeBuffer)
    {

    }
}