/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 08:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/20 15:53:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <set>
# include <ctime>
# include <algorithm>
# include <cassert>
# include <cstring>

# include <unistd.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <netinet/in.h>

# include <queue>
# include <deque>
# include <list>

# include "ANSIColours.hpp"

const char* requestLiteral =
"GET /dashboard HTTP/1.1\r\nHost: exasgasge.com\r\nCookie: sessionid=alolol9\r\n\r\nGET /dashboard HTTP/1.1\r\nHost: examplfuckthise.com\r\nCookie: sessionid=abcomg123456789\r\n\r\nGET /dashboard HTTP/1.1\r\nHost: example.com\r\nCookie: sessionid=abc123456789\r\n\r\n";

const char* responseLiteral = " epa foda eu so quero uma (newlines!! \n)string mesmo longa para testar IO em que as respostas sao lidas mais rapidamente do que escritas, newlines!! \n mas nao muitas que depois fica uma granda cowboyada. Bom, sem masi a dizer, \nhoje é sexta feira, o país tá a arder, boa já passámos o tamnho da outra request literal size, \nnem tudo é mau. idelamente queria que fosse o dobro ou o triplo do tamanho, o \nobjectivo é testar como é que a request class a connection reagem sob \nmultiplos requests pedidos na mesma conexão e a gestão de input \ne output. de acordo com http 1.1, a ordem de saida tem de ser igual à \nordem de entrada, por isso mesmo que alguma resposta posterior \nesteja pronta, nao pode ser enviada ainda, tem de ser em \nforma de queue \n\n\n\n\n";



typedef unsigned long int t_session_id;
typedef double t_difftime;
typedef int t_fd;

typedef std::string t_http_header;
typedef std::string t_http_value;

#define COOKIE "Cookie"
#define BUFFER_SIZE 10
#define MAX_IO 100

#define HEADER_ENDLINE "\r\n"
#define HEADER_ENDLINE_LEN 2
#define HEADER_FINISH "\r\n\r\n"
#define HEADER_FINISH_LEN 4

struct   Clock;
struct   Session;
struct   Request;
struct   Interpreter;
struct   Connection;
class    Buffer;


class Buffer
{
    public:

        // Appends a string (or buffer) of characters to the end of the deque.
        void append(const char* data, std::size_t len)
        {
            assert(data && len > 0);
            _buffer.insert(_buffer.end(), data, data + len);
        }

        // Inserts a string (or buffer) of characters at the beginning of the deque.
        void insert_at_beginning(const char* data, std::size_t len)
        {
            assert(data && len > 0);
            _buffer.insert(_buffer.begin(), data, data + len);
        }

        std::deque<char>::iterator search(const char* data, std::size_t len)
        {
            assert(data && len > 0);
            return std::search(_buffer.begin(), _buffer.end(), data, data + len);
        }

        bool getline(std::string& placeLine)
        {
            std::deque<char>::iterator newline_pos = search(HEADER_ENDLINE, HEADER_ENDLINE_LEN);
            if (newline_pos != _buffer.end())
            {
                placeLine.assign(_buffer.begin(), newline_pos + 1); // Extract until and including '\n'
                _buffer.erase(_buffer.begin(), newline_pos + 1);
                return (true);
            }
            return (false); // No newline found
        }

        int write_to_fd(int fd, int numChars)
        {
            if (_buffer.empty() || numChars == 0)   
                return (0); // Nothing to write

            numChars = (numChars > (int)_buffer.size() ? _buffer.size() : numChars);

            std::vector<char> temp_buffer(_buffer.begin(), _buffer.begin() + numChars);
            int bytes_written = ::write(fd, &temp_buffer[0], temp_buffer.size());

            if (bytes_written == -1)
                return (-1);        // client disconnected mid writting
            
            if (bytes_written > 0)
                _buffer.erase(_buffer.begin(), _buffer.begin() + bytes_written);

            return (bytes_written); // Return number of bytes written, or -1 if there was an error
        }

        std::deque<char>::iterator begin()
        {
            return _buffer.begin();
        }

        std::deque<char>::iterator end()
        {
            return _buffer.end();
        }

        std::deque<char>& getBuffer()
        {
            return _buffer;
        }

        bool extract_request(Buffer& new_buffer, const char* delimiter, std::size_t delimiter_length)
        {
            std::deque<char>& newBuf = new_buffer.getBuffer();
            std::deque<char>::iterator it = search(delimiter, delimiter_length);

            if (it != _buffer.end())
            {
                std::deque<char>::iterator start_of_next_request = it + delimiter_length;
                newBuf.insert(newBuf.end(), start_of_next_request, _buffer.end());
                _buffer.erase(start_of_next_request, _buffer.end());
                return (true); // Indicate that a new request was extracted
            }

            return (false); // Indicate that no delimiter was found
        }

        void print(const char* fontColour, const char* backgroundColour) const
        {
            std::string output(_buffer.begin(), _buffer.end());
            std::cout << fontColour << backgroundColour << output << COLOR_RESET << std::endl; // Print the whole string at once
        }

        void clear()
        {
            _buffer.clear();
        }

        size_t size() const
        {
            return _buffer.size();
        }

    private:
        std::deque<char> _buffer;
};

struct Clock
{
    std::time_t lastActivity;
};


struct Session
{
    Clock                               clock;

    t_session_id                        id;
    int                                 loggedTimes;
};

struct Request
{
    Request(int status = 0) : status(status) {}
    void    parseRequest(std::string request)
    {
        std::string header;
        std::string value;
        std::string::size_type pos = 0;
        std::string::size_type end = 0;

        while (pos != std::string::npos)
        {
            end = request.find(":", pos);
            header = request.substr(pos, end - pos);
            pos = end + 1;
            end = request.find("\n", pos);
            value = request.substr(pos, end - pos);
            pos = end + 1;
            headers[header] = value;
        }
    }

    void process()
    {
        raw_request.print(COLOR_GREEN, "");
        raw_response.append(responseLiteral, std::strlen(responseLiteral));
        raw_request.clear();
        status = 1;
    }

    bool   hasSessionHeader()
    {
        return (headers.find(COOKIE) != headers.end());
    }

    Buffer& getRawRequest()
    {
        return (raw_request);
    }

    Buffer& getRawResponse()
    {
        return (raw_response);
    }
    
    int         status;
    Buffer      raw_response;
    Buffer      raw_request;
    Clock       clock;
    std::map<t_http_header, t_http_value> headers;
};

struct Connection
{
    Connection() : curReadRequest(NULL), curWriteRequest(NULL) {}
    Clock                               clock;

    void                                setCurReadNull()
    {
        //std::cout << "set read null, total: " << PendingRequests.size() << std::endl;
        curReadRequest = NULL;
    }

    Request*                            getCurReadConnection()
    {
        if (!curReadRequest)
        {
            Request newRequest;

            PendingRequests.push(newRequest);
            curReadRequest = &PendingRequests.back();
            //std::cout << "new request added, total: " << PendingRequests.size() << std::endl;
        }
        return (curReadRequest);
    }

    Request*                            getCurWriteConnection()
    {
        if (!curWriteRequest)
        {
            if (PendingRequests.empty())
                return (NULL);
            curWriteRequest = &PendingRequests.front();
        }
            
        return (curWriteRequest);
    }

    void                                setCurWriteNull()
    {
        //std::cout << "set write null, total: " << PendingRequests.size() << std::endl;
        curWriteRequest = NULL;
    }

    Request*                            curReadRequest;
    Request*                                             curWriteRequest;        
    std::queue<Request, std::list<Request> >                 PendingRequests;
};




struct Interpreter
{
    Interpreter(time_t connectionTimeOut = 100, time_t sessionTimeOut = 100000, unsigned long int maxIO = MAX_IO) : 
        connectionTimeOut(connectionTimeOut), 
        sessionTimeOut(sessionTimeOut),
        maxIO(maxIO) {}
    
    Clock                               clock;

    void addOpenConnection(t_fd fd);
    void closeTimeoutConnections();
    void closeTimeoutSessions();
    
    int readConnection(t_fd fd);
    int writeConnection(t_fd fd);
    
    std::map<t_fd,          Connection>             ConnectionsOpen;
    std::map<t_session_id,  Session>                SessionsStored;
    std::set<Session>                               SessionsUnnassigned;


    char      buffer[BUFFER_SIZE + 1];
    const time_t connectionTimeOut;
    const time_t sessionTimeOut;
    const unsigned long int maxIO;
};

void Interpreter::closeTimeoutSessions()
{
    clock.lastActivity = std::time(0);

    for (std::map<t_session_id, Session>::iterator it = SessionsStored.begin(); it != SessionsStored.end(); ++it)
    {
        if (clock.lastActivity - it->second.clock.lastActivity > sessionTimeOut)
            SessionsStored.erase(it);
    }
    for (std::set<Session>::iterator it = SessionsUnnassigned.begin(); it != SessionsUnnassigned.end(); ++it)
    {
        if (clock.lastActivity - it->clock.lastActivity > sessionTimeOut)
            SessionsUnnassigned.erase(it);
    }
}

void Interpreter::closeTimeoutConnections()
{
    clock.lastActivity = std::time(0);

    for (std::map<t_fd, Connection>::iterator it = ConnectionsOpen.begin(); it != ConnectionsOpen.end(); ++it)
    {
        if (clock.lastActivity - it->second.clock.lastActivity > connectionTimeOut)
            ConnectionsOpen.erase(it);
    }
}

void    Interpreter::addOpenConnection(t_fd fd)
{
    Connection newConnection;
    newConnection.clock.lastActivity = std::time(0);
    ConnectionsOpen[fd] = newConnection;
}

int   Interpreter::readConnection(t_fd fd)
{
    //std::cout << COLOR_RED << "         reading connection" << COLOR_RESET << std::endl;
    Connection&     connection = ConnectionsOpen[fd];
    Request*        thisRequest = connection.getCurReadConnection();
    Buffer*         thisBuffer = &thisRequest->getRawRequest();

    connection.clock.lastActivity = std::time(0);
    thisRequest->clock.lastActivity = connection.clock.lastActivity;
    
    int totalBytes = 0;
    
    int curReadMax = 1;

    while (curReadMax)
    {
        curReadMax = std::min(maxIO - totalBytes, sizeof(buffer));
        int readBytes = read(fd, buffer, curReadMax);
        //std::cout << "readBytes: " << readBytes << "; buffer has: '" << buffer << "'" << std::endl;
        if (readBytes < 0)
            return (1); // client disconnected mid reading, close connection at the next epoll round
        totalBytes += readBytes;
        if (readBytes)
            thisBuffer->append(buffer, readBytes);
            
        //std::cout << "Buffer now is: " << std::endl << std::endl;
        //thisBuffer->print();
        //std::cout  << std::endl << std::endl;
        
        if (thisBuffer->search(HEADER_FINISH, HEADER_FINISH_LEN) != thisBuffer->end())
        {
            // there is a complete request at hand. One must switch the current read request to a new empty one
            // switch the current raw_request buffer to that one and keep reading.
            
            connection.setCurReadNull();
            Request*    nextRequest = connection.getCurReadConnection();
            Buffer*     nextBuffer = &nextRequest->getRawRequest();
            
            thisBuffer->extract_request(*nextBuffer, HEADER_FINISH, HEADER_FINISH_LEN);
            thisRequest->process();

            //switch requests and buffers to the new ones
            thisRequest = nextRequest;
            thisBuffer = nextBuffer;

            thisRequest->clock.lastActivity = connection.clock.lastActivity; //update clock for the new request as well
        }
        if (readBytes < curReadMax)
        {
            //std::cout << "nothing left to read" << std::endl;
            break;
        }
            

    }
    std::cout << COLOR_RED << "         FINISHED reading connection" << COLOR_RESET << std::endl;
    return (1);
}

int   Interpreter::writeConnection(t_fd fd)
{
    Connection& connection = ConnectionsOpen[fd];
    //std::cout << "pending requests: " << connection.PendingRequests.size() << std::endl;
    Request*    thisRequest = connection.getCurWriteConnection();
    //std::cout << "thisRequest: " << thisRequest << std::endl;
    if (!thisRequest || !thisRequest->status) //all requests have been written
    {
        //std::cout << "no request to write" << std::endl;
        return (0);
    }

    Buffer*     thisBuffer = &thisRequest->getRawResponse();
    
    connection.clock.lastActivity = std::time(0);
    thisRequest->clock.lastActivity = connection.clock.lastActivity;
    
    int totalBytes = 0;
    
    int curWriteMax = 1;

    while (curWriteMax)
    {
        curWriteMax = std::min(maxIO - totalBytes, sizeof(buffer));

        int writtenBytes = thisBuffer->write_to_fd(fd, curWriteMax);
        //std::cout << "write Bytes: " << writtenBytes << "; curWriteMax: "<< curWriteMax << " ; total bytes: " << totalBytes << std::endl;

        if (writtenBytes < 0)
        {
            //std::cout << "client disconnected mid writing" << std::endl;
            return (0); // client disconnected mid writing, close connection at the next epoll round
        }
            
            
        totalBytes += writtenBytes;

        if (writtenBytes < curWriteMax)
        {
            //std::cout << "written less than max" << std::endl;
            if (thisBuffer->getBuffer().empty())
            {
                //std::cout << " request written, remove and swap" << std::endl;

                connection.PendingRequests.pop();
                connection.setCurWriteNull();
                thisRequest = connection.getCurWriteConnection();
                if (!thisRequest || !thisRequest->status) //all requests have been written
                    break;
                thisBuffer = &thisRequest->getRawResponse();
            }
            else    
                break;      
        }
        //std::cout << "alive" << std::endl;
    }
    //std::cout << "connection finished writting this time" << std::endl;
    return (1);

}

void    addEvent(int epollfd, int fd, int events, int& totalSubscribed)
{
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    totalSubscribed++;
}

void    delEvent(int epollfd, int fd, int& totalSubscribed)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
    totalSubscribed--;
}


int main2()
{
    Interpreter interpreter;

    int fd1 = open("connection1.txt", O_TRUNC | O_RDWR);
    write(fd1, requestLiteral, strlen(requestLiteral));
    close(fd1);
    fd1 = open("connection1.txt", O_RDONLY);
    fcntl(fd1, F_SETFL, O_NONBLOCK | O_CLOEXEC);
    interpreter.addOpenConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);

    close(fd1);

    return (0);
}


int main(void)
{
    int subscribed = 0;
    int epollfd = epoll_create(1);
    int sockpair[2];

    char buffer[1024];

    if(socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0, sockpair) == -1)
    {
        std::cout << "ops" << std::endl;
        exit(1);
    }
    fcntl(sockpair[0], F_SETFL, O_NONBLOCK | O_CLOEXEC);
    fcntl(sockpair[1], F_SETFL, O_NONBLOCK | O_CLOEXEC);
    
    Interpreter interpreter;
    
    interpreter.clock.lastActivity = std::time(0);
    




    // server accept
    interpreter.addOpenConnection(sockpair[0]);

    //std::cout << "pending requests: " << interpreter.ConnectionsOpen[sockpair[0]].PendingRequests.size() << std::endl;
    
    addEvent(epollfd, sockpair[0], EPOLLIN | EPOLLOUT, subscribed);
    addEvent(epollfd, sockpair[1], EPOLLIN, subscribed);

    //send data
    write(sockpair[1], requestLiteral, std::strlen(requestLiteral));

    int count = 0;

    struct epoll_event events[100];
    while (subscribed)
    {
        int triggered = epoll_wait(epollfd, events, 100, 1000);
        //std::cout << "triggered: " << triggered << std::endl;   
        for(int i = 0; i < triggered; ++i)
        {
            if (events[i].events & EPOLLIN)
            {
                if (events[i].data.fd == sockpair[0])
                    interpreter.readConnection(events[i].data.fd);
                else
                {
                        //std::cout << "sockpair[1] is ready to write" << std::endl;
                    std::memset(buffer, 0, sizeof(buffer));
                    int bytesRead = read(sockpair[1], buffer, sizeof(buffer));
                    buffer[bytesRead] = '\0';
                    std::cout << COLOR_BLUE << buffer << COLOR_RESET << std::endl;
                }
            }
            if (events[i].events & EPOLLOUT)
            {
                if (!interpreter.writeConnection(events[i].data.fd))
                {
                    delEvent(epollfd, sockpair[0], subscribed);
                    close(sockpair[0]);
                }
            }
        }
        if (triggered == 0)
        {
            std::cout << " zero triggered" << std::endl;
            break ;
        }
            
        count++;
        if (count > 30)
        {
            std::cout << "exceeded, exiting" << std::endl;
            break ;
        }

    }

    std::cout << "left: " << interpreter.ConnectionsOpen[sockpair[0]].PendingRequests.size() << std::endl;

    close(sockpair[0]);
    close(sockpair[1]);
    close(epollfd);

    return (0);
}

