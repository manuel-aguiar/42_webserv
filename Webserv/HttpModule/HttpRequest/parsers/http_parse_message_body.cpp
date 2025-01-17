/* *********************************/
/*                                 */
/*   http_parse_message_body.cpp   */
/*   - the message body parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"


int HttpRequest::mf_parseBody(const std::string& data)
{
    (void)data;
    return Http::Status::OK;
}

int HttpRequest::mf_parseChunkedBody(const std::string& data)
{
    (void)data;
    return Http::Status::OK;
}
