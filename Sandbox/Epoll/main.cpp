/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:32:00 by manuel            #+#    #+#             */
/*   Updated: 2024/09/02 11:29:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/epoll.h>

/*
epoll (epoll_create,
epoll_ctl, epoll_wait), 
*/


/*
    This time we will open a pipe where child writes to parent

    Child will execve a command with args passed via main;
    dup2 write end to stdout;

    parent will use epoll to monitor traffic, marking the read end of the pipe
    to fire an event when it is ready to be read from

    epoll hanging indeffinitely, for our purposes (setting timout -1);

    once the pipe is ready to read, parent writes the message received from child.

    compile: c++ -Wall -Wextra -Werror main.cpp
    example usage: ./a.out /bin/cat main.cpp

    valgrind --leak-check=full --track-fds=yes --trace-children=yes ./a.out /bin/cat main.cpp



    Notes:
        using epoll_create1, we can set the close on exec flag directly without an extra call to fcntl
            -> child doesn't leak fd on execve
*/


#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>


#include <iostream>
#include <cstring>



#define MAX_EVENTS 10


int main(int ac, char **av, char **env)
{
    if (ac == 1)
        return (EXIT_FAILURE);
    int epollfd;
    int pipefd[2];
    int pid;
    int status;

    char readbuf[100];
    int bytesRead;

    struct epoll_event event;
    struct epoll_event wait[MAX_EVENTS];
    int numWait;

    //create epoll instance
    epollfd = epoll_create1(EPOLL_CLOEXEC);
    /*
        or:

        epollfd = epoll_create(1);
        if (fcntl(epollfd, F_SETFD, FD_CLOEXEC) == -1)
        {
            std::cerr << "fcntl() " << std::strerror(errno) << std::endl;
            close(epollfd);
            return (EXIT_FAILURE);
        }
    */

    if (epollfd == -1)
    {
        std::cerr << "epoll_create1() " << std::strerror(errno) << std::endl;
        return (EXIT_FAILURE);
    }


    //open pipe    
    if (pipe(pipefd) == -1)
    {
        std::cerr << "epoll_create1() " << std::strerror(errno) << std::endl;
        close(epollfd);
        return (EXIT_FAILURE);
    }

    //subscribe the piperead fd to the epoll
    event = (struct epoll_event){};     //required, event.data is a union, if you use int (4bytes) you have another 4bytes of garbage (union with void *ptr);
    event.events = EPOLLIN;
    event.data.fd = pipefd[0];
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, pipefd[0], &event) == -1)
    {
        std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
        close(epollfd);
        close(pipefd[0]);
        close(pipefd[1]);
        return (EXIT_FAILURE);
    }
    pid = fork();

    //child
    if (!pid)
    {
        close(pipefd[0]);                               //child closes pipe-read
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)       // redirect write-end of the pipe to stdout;
        {
            std::cerr << "dup2() " << std::strerror(errno) << std::endl;
            close(epollfd);
            close(pipefd[1]);
            return (EXIT_FAILURE);
        }

        close(pipefd[1]);

        sleep(1);

        
        //exec the command;
        if (execve(av[1], &av[1], env) == -1)
        {
            std::cerr << "execve() " << std::strerror(errno) << std::endl;
            close(epollfd);
            close(pipefd[1]);
            return (EXIT_FAILURE);
        }
    }
    else
    {
        close(pipefd[1]); /// close pipe-write
        std::memset(wait, 0, sizeof(wait));
        std::cout << "Parent is going to wait... " << std::endl;
        numWait = epoll_wait(epollfd, wait, MAX_EVENTS, -1);    //timeout eent -1, make it hang
        if (numWait == -1)
        {
            std::cerr << "epoll_wait() " << std::strerror(errno) << std::endl;
            close(epollfd);
            close(pipefd[0]);
            return (EXIT_FAILURE);
        }
        std::cout << "Parent: epoll found active fds: " << numWait << std::endl;
        for (int i = 0; i < numWait; ++i)
        {
            if (wait[i].data.fd == pipefd[0])
            {   
                std::cout << "Child told parent: \n\n" << std::endl;
                while (true)
                {
                    bytesRead = read(pipefd[0], readbuf, sizeof(readbuf) - 1);
                    if (bytesRead == -1)
                    {
                        std::cerr << "read() " << std::strerror(errno) << std::endl;
                        close(epollfd);
                        close(pipefd[0]);
                        return (EXIT_FAILURE);
                    }
                    if (bytesRead == 0)
                        break ;
                    readbuf[bytesRead] = '\0';
                    std::cout << readbuf;
                }
                std::cout << "\n\nChild message finished" << std::endl;
                break ;
            }
        }


        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            std::cout << "Child process finished with exit status: " << WEXITSTATUS(status) << std::endl;
        close(epollfd);
        close(pipefd[0]);   //parent closes piperead;
    }

    return (EXIT_SUCCESS);

}
