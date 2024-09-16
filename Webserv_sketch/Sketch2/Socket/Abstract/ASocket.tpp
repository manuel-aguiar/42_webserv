/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:14:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:19:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ASOCKET_TPP

#define ASOCKET_TPP

# include "../../../LibftCpp/libftcpp.hpp"
# include "ISocket.hpp"

template <
    typename SockAddr
>
class ASocket : public ISocket
{
    public:
        virtual ~ASocket() {};

        //inherited from ISocketAddress
        struct sockaddr*                    getSockAddr();
        socklen_t*                          getAddrLen();
        int                                 getAddrFamily();
        UniquePtr<ISocketAddress>           clone();

    protected:
        SockAddr     _addr;
        
        ASocket();
        ASocket(const int fd, const SockAddr& addr);
        ASocket(const ASocket& copy);
        ASocket& operator=(const ASocket& assign);

};


template <typename SockAddr>
ASocket<SockAddr>::~ASocket() {};

 template <typename SockAddr>       
ASocket<SockAddr>::ASocket() :
    FileDescriptor(-1),
    _addr() {}
        
 template <typename SockAddr>           
ASocket<SockAddr>::ASocket(const int fd, const SockAddr& addr) :
    FileDescriptor(fd),
    _addr(addr) {}

 template <typename SockAddr>       
ASocket<SockAddr>::ASocket(const ASocket& copy) :
    FileDescriptor(copy),
    _addr(copy._addr) {}

template <typename SockAddr>
ASocket<SockAddr>& ASocket<SockAddr>::operator=(const ASocket& assign)
{
    if (this == &assign)
        return (*this);
    FileDescriptor::operator=(assign);
    _addr = assign._addr;
    return (*this);
}

        //inherited from ISocketAddress
template <typename SockAddr>
struct sockaddr*            ASocket<SockAddr>::getSockAddr()       {return (_addr.getSockAddr());};

template <typename SockAddr>
socklen_t*                  ASocket<SockAddr>::getAddrLen()        {return (_addr.getAddrLen());};

template <typename SockAddr>
int                         ASocket<SockAddr>::getAddrFamily()     {return (_addr.getAddrFamily());};

template <typename SockAddr>
UniquePtr<ISocketAddress>   ASocket<SockAddr>::clone()             {return (_addr.clone());};


#endif