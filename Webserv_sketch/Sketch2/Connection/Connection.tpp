/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:24:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 15:46:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include <sys/epoll.h>
# include <sys/socket.h>
# include <netinet/in.h>


/*
    IPV4 will use Connection sockaddr_in, IPV6 sockaddr_in6... and so on

    Connection is responsible for its own establishment (accept) and closure;
*/

template <
    typename addressClass
>
class Connection
{
    public:
        Connection(const int listenerfd) : 
        _connfd     (-1),
        _listenfd   (listenerfd),
        _addr       ((addressClass) {}),
        _addrlen    (sizeof(_addr))
        {
            _connfd = accept(listenerfd, (struct sockaddr*)&_addr, &_addrlen);
            if (_connfd == -1)
                throw std::runtime_error (std::string("Connection construction failed, accept(): ") + std::strerror(errno));
            //no need to adjust flags...? listening socket will already be o_nonblock | o_cloexec
        }

        ~Connection()
        {
            if (_connfd != -1)
                close(_connfd);
        };
        
        Connection(const Connection& copy) :
            _connfd     (copy._connfd),
            _listenfd   (copy._listenfd),
            _addr       (copy._addr),
            _addrlen    (copy._addrlen)
        {

        }
        Connection& operator=(const Connection& assign)
        {
            if (this == &assign)
                return (*this);
            *this = assign;
            return (*this);
        };

        int             getConnFd() const {return (_connfd);}
        void            setConnFd(const int sockfd) {_connfd = sockfd;};

        addressClass&   getAddr() const {return  (_addr);};
        void            setAddr(const addressClass& addr) {_addr = addr;};

    private:
        int                 _connfd;
        int                 _listenfd;
        addressClass        _addr;
        socklen_t           _addrlen;

        
        Connection() {};
};


#endif