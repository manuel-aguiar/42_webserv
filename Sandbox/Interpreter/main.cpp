/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 08:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/20 12:23:18 by mmaria-d         ###   ########.fr       */
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

        void print() const
        {
            std::string output(_buffer.begin(), _buffer.end());
            std::cout << output << std::endl; // Print the whole string at once
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
        raw_request.print();
    }

    bool   hasSessionHeader()
    {
        return (headers.find(COOKIE) != headers.end());
    }

    Buffer& getRawRequest()
    {
        return (raw_request);
    }
    
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
        curReadRequest = NULL;
    }

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

    void                                setCurWriteNull()
    {
        curWriteRequest = NULL;
    }

    Request*                            curReadRequest;
    Request*                            curWriteRequest;        
    std::queue<Request>                 PendingRequests;
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
    Connection&     connection = ConnectionsOpen[fd];
    Request*        thisRequest = connection.getCurReadConnection();
    Buffer*         thisBuffer = &thisRequest->getRawRequest();

    connection.clock.lastActivity = std::time(0);
    thisRequest->clock.lastActivity = connection.clock.lastActivity;
    
    int totalBytes = 0;
    
    int curReadMax = 1;

    while (curReadMax)
    {
        curReadMax = std::min(maxIO - totalBytes, sizeof(buffer) - 1);
        int readBytes = read(fd, buffer, curReadMax);
        buffer[readBytes] = '\0';
        std::cout << "readBytes: " << readBytes << "; buffer has: '" << buffer << "'" << std::endl;
        if (readBytes < 0)
            return (1); // client disconnected mid reading, close connection at the next epoll round
        totalBytes += readBytes;
        if (readBytes)
            thisBuffer->append(buffer, readBytes);

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
            //we read less bytes than the allowance, nothing left to read
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


int main()
{
    Interpreter interpreter;

    int fd1 = open("connection1.txt", O_RDONLY);
    fcntl(fd1, F_SETFL, O_NONBLOCK | O_CLOEXEC);
    interpreter.addOpenConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    interpreter.readConnection(fd1);
    close(fd1);

    return (0);
}

/*
int main(void)
{
    int subscribed = 0;
    int epollfd = epoll_create(1);

    Interpreter interpreter;
    
    interpreter.clock.lastActivity = std::time(0);
    
    int fd1 = open("connection1.txt", O_RDONLY);
    fcntl(fd1, F_SETFL, O_NONBLOCK | O_CLOEXEC);

    int fd2 = open("connection2.txt", O_RDONLY);
    fcntl(fd2, F_SETFL, O_NONBLOCK | O_CLOEXEC);

    int fd3 = open("connection3.txt", O_RDONLY);
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
        std::cout << "triggered: " << triggered << std::endl;   
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

    close(fd1);
    close(fd2);
    close(fd3);
    close(epollfd);

    return (0);
}

*/