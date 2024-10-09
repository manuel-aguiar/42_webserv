#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <sys/epoll.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"
#define REQUEST "Hello Server!"

int main()
{
    int total = 1000;
    std::map<int, int> connections;

    int bytesRead;
    char readBuff[1024];
    struct epoll_event single;
    struct epoll_event events[1000];
    int epollfd = epoll_create1(EPOLL_CLOEXEC);
    if (epollfd == -1) {
        std::cerr << "Failed to create epoll" << std::endl;
        return EXIT_FAILURE;
    }

    int connected = 0;

    for (size_t i = 0; i < total; i++)
    {
        struct sockaddr_in connAddress;
        std::memset(&connAddress, 0, sizeof(connAddress));
        connAddress.sin_family = AF_INET;
        connAddress.sin_addr.s_addr = inet_addr(LOCALHOST);
        connAddress.sin_port = htons(PORT);

        connections[i] = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
        if (connections[i] == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return EXIT_FAILURE;
        }

        struct linger so_linger;
        so_linger.l_onoff = 1; // Enable linger option
        so_linger.l_linger = 0; // Timeout of 0, discard unsent data
        setsockopt(connections[i], SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

        std::cout << connections[i] << ": Sending Request...." << std::endl;

        if (connect(connections[i], (struct sockaddr *)&connAddress, sizeof(connAddress)) == -1) {
            if (errno != EINPROGRESS) {
                std::cerr << "Failed to connect" << std::endl;
                close(connections[i]);
                continue; // Skip to the m_next iteration
            }
        }

        single.events = EPOLLOUT;
        single.data.fd = connections[i];
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connections[i], &single) == -1) {
            std::cerr << "Failed to add socket to epoll" << std::endl;
            close(connections[i]);
            continue; // Skip to the m_next iteration
        }
    }

    size_t active = connections.size();

    while (active)
    {
        std::cout << "active " << active <<"., waiting for events..." << std::endl;
        int wait = epoll_wait(epollfd, events, 1000, -1);
        std::cout << "      DONE events arrived: " << wait << std::endl;    
        for (int i = 0; i < wait; ++i)
        {
            if (events[i].events & EPOLLOUT)
            {
                int err = 0;
                socklen_t len = sizeof(err);
                if (getsockopt(events[i].data.fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
                    // getsockopt failed, handle error
                } else if (err != 0) {
                     std::cout << "Connection failed: " << strerror(err) << std::endl;
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        std::cout << "erasing connection " << events[i].data.fd << std::endl;
                        for (std::map<int, int>::iterator iter = connections.begin(); iter != connections.end(); ++iter)
                        {
                            if (iter->second == events[i].data.fd)
                            {
                                connections.erase(iter);
                                break;
                            }
                        }
                        std::cout << "      DONE erasing connection " << events[i].data.fd << std::endl;
                        close(events[i].data.fd);
                        std::cout << "closed connection fd " << events[i].data.fd << std::endl;
                        active--;
                        continue ;
                } else {
                    connected++;
                    std::cout << "Connection successful! total connected: " << connected << std::endl;
                }
                std::cout << "sending request to fd: " << events[i].data.fd << std::endl;
                if (write(events[i].data.fd, REQUEST, std::strlen(REQUEST) + 1) == -1) {
                    std::cerr << "Failed to send request" << std::endl;
                }
                std::cout << "      DONE request sent to fd: " << events[i].data.fd << std::endl;

                single.events = EPOLLIN;
                single.data.fd = events[i].data.fd;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &single);
            }
            else if (events[i].events & EPOLLIN)
            {
                std::memset(&readBuff, 0, sizeof(readBuff));
                std::cout << "reading response from fd: " << events[i].data.fd << std::endl;
                bytesRead = read(events[i].data.fd, readBuff, sizeof(readBuff) - 1);
                if (bytesRead > 0) {
                    std::cout << events[i].data.fd << " received: " << readBuff << std::endl;
                } else if (bytesRead == -1) {
                    std::cerr << "Failed to read response " << events[i].data.fd << std::endl;
                }
                std::cout << "      DONE reading response from fd: " << events[i].data.fd << std::endl;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                std::cout << "erasing connection " << events[i].data.fd << std::endl;
                for (std::map<int, int>::iterator iter = connections.begin(); iter != connections.end(); ++iter)
				{
					if (iter->second == events[i].data.fd)
					{
						connections.erase(iter);
						break;
					}
				}
                std::cout << "      DONE erasing connection " << events[i].data.fd << std::endl;
                close(events[i].data.fd);
                std::cout << "closed connection fd " << events[i].data.fd << std::endl;
                active--;
            }
        }
    }

    close(epollfd); // Close epoll file descriptor
    return (EXIT_SUCCESS);
}
