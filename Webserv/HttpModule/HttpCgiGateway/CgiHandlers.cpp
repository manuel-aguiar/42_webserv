

# include "CgiHandlers.hpp"
# include "HttpCgiGateway.hpp"


namespace Http
{
    void
    CgiHandlers::onSuccess(Cgi::User user)
    {
        reinterpret_cast<Http::CgiGateway*>(user)->onCgiSuccess();
    }

    void 
    CgiHandlers::onError(Cgi::User user)
    {
        reinterpret_cast<Http::CgiGateway*>(user)->onCgiError();
    }

    Cgi::IO::State 		
    CgiHandlers::onRead(Cgi::User user, const Ws::fd readFd)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onCgiRead(readFd));
    }

    Cgi::IO::State 		
    CgiHandlers::onWrite(Cgi::User user, const Ws::fd writeFd)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onCgiWrite(writeFd));
    }

    Cgi::IO::State		
    CgiHandlers::onReceiveHeaders(Cgi::User user, const Cgi::HeaderData& headers)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onCgiReceiveHeaders(headers));
    }

}
