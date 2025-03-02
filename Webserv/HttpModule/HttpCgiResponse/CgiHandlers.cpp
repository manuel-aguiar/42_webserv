

# include "CgiHandlers.hpp"
# include "HttpCgiResponse.hpp"


namespace Http
{
    void
    CgiHandlers::onSuccess(Cgi::User user)
    {
        reinterpret_cast<Http::CgiResponse*>(user)->onCgiSuccess();
    }

    void 
    CgiHandlers::onError(Cgi::User user)
    {
        reinterpret_cast<Http::CgiResponse*>(user)->onCgiError();
    }

    Cgi::IO::State 		
    CgiHandlers::onRead(Cgi::User user, const Ws::fd readFd)
    {
        return (reinterpret_cast<Http::CgiResponse*>(user)->onCgiRead(readFd));
    }

    Cgi::IO::State 		
    CgiHandlers::onWrite(Cgi::User user, const Ws::fd writeFd)
    {
        return (reinterpret_cast<Http::CgiResponse*>(user)->onCgiWrite(writeFd));
    }

    Cgi::IO::State		
    CgiHandlers::onReceiveHeaders(Cgi::User user, const Cgi::HeaderData& headers)
    {
        return (reinterpret_cast<Http::CgiResponse*>(user)->onCgiReceiveHeaders(headers));
    }

}
