/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 08:45:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/20 11:43:41 by mmaria-d         ###   ########.fr       */
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

    bool extract_request(Buffer& new_buffer)
    {
        const char* delimiter = "\r\n\r\n";
        std::size_t delimiter_length = 4; // Length of "\r\n\r\n"

        // Search for the delimiter in the current buffer
        
        std::deque<char>::iterator it = search(HEADER_FINISH, HEADER_FINISH_LEN);

        // If the delimiter is found
        if (it != buffer_.end()) {
            // Calculate the position after the delimiter
            std::deque<char>::iterator start_of_next_request = it + delimiter_length;

            // Extract the remaining part of the buffer after the delimiter
            std::string extracted_data(start_of_next_request, buffer_.end());
            
            // Remove the processed part from the current buffer
            buffer_.erase(buffer_.begin(), start_of_next_request);

            // Remove any leading whitespace from extracted data (optional)
            extracted_data.erase(0, extracted_data.find_first_not_of(" \n\r\t"));

            // Populate the new buffer with the extracted data
            new_buffer.append(extracted_data.c_str(), extracted_data.size());

            return true; // Indicate that a new request was extracted
        }

        return false; // Indicate that no delimiter was found
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

    Buffer& getRawRequest()
    {
        return (raw_request);
    }
    
    Buffer      raw_request;

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
    Connection& connection = ConnectionsOpen[fd];
    Buffer* raw_request = &connection.getCurReadConnection()->getRawRequest();

    connection.clock.lastActivity = std::time(0);

    int totalBytes = 0;
    int curReadMax = std::min(maxIO, sizeof(buffer));
    while (1)
    {
        int readBytes = read(fd, buffer, curReadMax);
        if (readBytes < 0)
            return (1); // client disconnected
        totalBytes += readBytes;
        raw_request->append(buffer, readBytes);

        if (raw_request->search(HEADER_FINISH, HEADER_FINISH_LEN) != raw_request->end())
        {
            // there is a complete request at hand. One must switch the current read request to a new empty one
            // switch the current raw_request buffer to that one and keep reading.
            connection.setCurReadNull();
            Buffer *nextRawRequest = &connection.getCurReadConnection()->getRawRequest();

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