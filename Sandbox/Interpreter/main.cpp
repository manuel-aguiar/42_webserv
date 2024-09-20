/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 08:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/20 11:08:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <set>
# include <ctime>
# include <algorithm>


# include <unistd.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/socket.h>

# include <queue>

typedef unsigned long int t_session_id;
typedef double t_difftime;
typedef int t_fd;

typedef std::string t_http_header;
typedef std::string t_http_value;

#define COOKIE "Cookie"
#define BUFFER_SIZE 10
#define MAX_IO 100

#define HEADER_ENDLINE "\r\n"
#define HEADER_FINISH "\r\n\r\n"

struct Clock;
struct Session;
struct Request;
struct Interpreter;
struct Connection;
class Buffer;


class Buffer
{
    public:

        // Appends a string (or buffer) of characters to the end of the deque.
        void append(const char* data, std::size_t len)
        {
            _buffer.insert(_buffer.end(), data, data + len);
        }

        // Inserts a string (or buffer) of characters at the beginning of the deque.
        void insert_at_beginning(const char* data, std::size_t len)
        {
            _buffer.insert(_buffer.begin(), data, data + len);
        }

        std::deque<char>::iterator search(const std::string& seq)
        {
            if (seq.empty())
                return _buffer.end();
            return std::search(_buffer.begin(), _buffer.end(), seq.begin(), seq.end());
        }

        bool getline(std::string& placeLine)
        {
            std::deque<char>::iterator newline_pos = search(HEADER_ENDLINE);
            if (newline_pos != _buffer.end())
            {
                placeLine.assign(_buffer.begin(), newline_pos + 1); // Extract until and including '\n'
                _buffer.erase(_buffer.begin(), newline_pos + 1);
                return (true);
            }
            return (false); // No newline found
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

struct Connection
{
    Clock                               clock;

    Request*                            getCurReadConnection()
    {
        if (!curReadRequest)
        {
            Request newRequest;

            PendingRequests.push(newRequest);
            curReadRequest = &PendingRequests.back();
        }
        return (curReadRequest);
    }

    Request*                            getCurWriteConnection()
    {
        if (!curWriteRequest)
            curWriteRequest = &PendingRequests.front();
        return (curWriteRequest);
    }

    Request*                            curReadRequest;
    Request*                            curWriteRequest;        
    std::queue<Request>                 PendingRequests;
};


struct Request
{
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

    bool   hasSessionHeader()
    {
        return (headers.find(COOKIE) != headers.end());
    }

    std::string& getRawRequest()
    {
        return (raw_request);
    }
    
    std::string      raw_request;

    std::map<t_http_header, t_http_value> headers;
};

struct Interpreter
{
    Clock                               clock;

    void addOpenConnection(t_fd fd);
    void closeTimeoutConnections();
    void closeTimeoutSessions();
    
    int readConnection(t_fd fd);

    std::map<t_fd,          Connection>             ConnectionsOpen;
    std::map<t_session_id,  Session>                SessionsStored;
    std::set<Session>                               SessionsUnnassigned;


    char      buffer[BUFFER_SIZE];
    const unsigned long int connectionTimeOut = 100;
    const unsigned long int sessionTimeOut = 100000;
    const unsigned long int maxIO = MAX_IO;
};

void Interpreter::closeTimeoutConnections()
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
    // open Connection associated with fd
    Connection& connection = ConnectionsOpen[fd];

    // current request being read, either unfinished old or a new one
    Request& request = *connection.getCurReadConnection();

    // raw request buffer to keep adding as we read
    std::string& raw_request = request.getRawRequest();

    connection.clock.lastActivity = std::time(0);

    int totalBytes = 0;
    int curReadMax = std::min(maxIO, sizeof(buffer));
    while (1)
    {
        int readBytes = read(fd, buffer, curReadMax);
        if (readBytes < 0)
            return (1);
        else if (readBytes < curReadMax)
        {
            // all bytes were read
            break;
        }
    }
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

int main(void)
{
    int subscribed = 0;
    int epollfd = epoll_create(1);

    Interpreter interpreter;
    interpreter.clock.lastActivity = std::time(0);
    
    int fd1 = open("request1.txt", O_RDONLY);
    fcntl(fd1, F_SETFL, O_NONBLOCK | O_CLOEXEC);

    int fd2 = open("request2.txt", O_RDONLY);
    fcntl(fd2, F_SETFL, O_NONBLOCK | O_CLOEXEC);

    int fd3 = open("request3.txt", O_RDONLY);
    fcntl(fd3, F_SETFL, O_NONBLOCK | O_CLOEXEC);



    // server accept
    interpreter.addOpenConnection(fd1);
    interpreter.addOpenConnection(fd2);
    interpreter.addOpenConnection(fd3);


    addEvent(epollfd, fd1, EPOLLIN, subscribed);
    addEvent(epollfd, fd2, EPOLLIN, subscribed);
    addEvent(epollfd, fd3, EPOLLIN, subscribed);

    struct epoll_event events[100];
    while (subscribed)
    {
        int triggered = epoll_wait(epollfd, events, 100, 1000);

        for(int i = 0; i < triggered; ++i)
        {
            if (events[i].events & EPOLLIN)
            {
                if (interpreter.readConnection(events[i].data.fd))
                {
                    delEvent(epollfd, events[i].data.fd, subscribed);
                }
            }
        }
    }


    return (0);
}