#include "../HttpRequest.hpp"


int HttpRequest::mf_parseBody(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}

int HttpRequest::mf_parseChunkedBody(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}
