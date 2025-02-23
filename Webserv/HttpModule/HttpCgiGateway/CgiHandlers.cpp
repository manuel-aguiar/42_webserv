

# include "CgiHandlers.hpp"
# include "HttpCgiGateway.hpp"


namespace Http
{
    void
    CgiHandlers::onSuccess(Cgi::User user)
    {
        reinterpret_cast<Http::CgiGateway*>(user)->onSuccess();
    }

    void 
    CgiHandlers::onError(Cgi::User user)
    {
        reinterpret_cast<Http::CgiGateway*>(user)->onError();
    }

    Cgi::IO::State 		
    CgiHandlers::onRead(Cgi::User user, const Ws::fd readFd)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onRead(readFd));
    }

    Cgi::IO::State 		
    CgiHandlers::onWrite(Cgi::User user, const Ws::fd writeFd)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onWrite(writeFd));
    }

    Cgi::IO::State		
    CgiHandlers::onReceiveHeaders(Cgi::User user, const Cgi::HeaderData& headers)
    {
        return (reinterpret_cast<Http::CgiGateway*>(user)->onReceiveHeaders(headers));
    }

}
