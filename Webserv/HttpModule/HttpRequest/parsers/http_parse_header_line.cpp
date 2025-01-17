#include "../HttpRequest.hpp"

int HttpRequest::mf_parseHeaders(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}