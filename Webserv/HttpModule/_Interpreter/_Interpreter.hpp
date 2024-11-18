


#ifndef BS

# define BS

#include <set>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <queue>

class Connection;
class HttpInterpreter;



class HttpSession
{
    public:


    private:

        int         session_id;
        size_t      logged_times;
};

typedef std::string Headers;
typedef std::string Value;

class HttpResponse;

class HttpConnection
{
    Connection&                 ioConnection;
    HttpInterpreter&            interpreter;

    std::queue<HttpRequest>     requests;
    std::queue<HttpResponse>        responses;
};

class HttpRequest
{
    public:
        HttpRequest(const Connection& conn);

    private:

        std::string	                m_method;
        std::string	                m_path;
        std::string	                m_http_version;
        std::map<Headers, Value>    m_headers;
        std::string	                m_body;

        HttpConnection& conn;
};


class HttpInterpreter
{

    public:
        typedef int t_sessionId;

    private:
        std::vector<HttpRequest*>               requests;
        std::map<t_sessionId, HttpSession>      sessions;

        const std::set<Headers>                 requestHeaders; // headers we interpret
        const std::set<Headers>                 responseHeaders; // headers we may send
};


int main (void)
{
    std::string request("GET BGAASGASGAS");

}

#endif
