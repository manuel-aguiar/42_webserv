/* *********************************/
/*                                 */
/*   http_parse_header_line.cpp    */
/*   - the header line parser for  */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"

int HttpRequest::mf_parseHeaders(const std::string& data)
{
    (void)data;
    return Http::Status::OK;
}