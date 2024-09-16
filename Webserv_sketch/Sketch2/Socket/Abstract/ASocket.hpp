/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:12:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET_HPP

#define ASOCKET_HPP

# include "../../../LibftCpp/libftcpp.hpp"
# include "ISocket.hpp"

template <
    typename SockAddr
>
class ASocket : public ISocket
{
    public:
        virtual ~ASocket() {};

    protected:
        SockAddr     _addr;
        
        ASocket() :
            FileDescriptor(-1),
            _addr() {}
            
        ASocket(const int fd, const SockAddr& addr) :
            FileDescriptor(fd),
            _addr(addr) {}
        
        ASocket(const ASocket& copy) :
            FileDescriptor(copy),
            _addr(copy._addr) {}

        ASocket& operator=(const ASocket& assign)
        {
            if (this == &assign)
                return (*this);
            FileDescriptor::operator=(assign);
            _addr = assign._addr;
            return (*this);
        }

        //implementation of ISocket         functions
        void                                close()             {if (this->_fd != 1) ::close(this->_fd);};

        //inherited from ISocketAddress
        struct sockaddr*                    getSockAddr()       {return (_addr.getSockAddr());};
        socklen_t*                          getAddrLen()        {return (_addr.getAddrLen());};
        int                                 getAddrFamily()     {return (_addr.getAddrFamily());};
        UniquePtr<ISocketAddress>           clone()             {return (_addr.clone());};

};


#endif