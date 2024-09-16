/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationSocket.tpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:07:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:59:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMUNICATIONSOCKET_TPP

# define COMMUNICATIONSOCKET_TPP

# include "../Abstract/CommunicationSocket/ICommunicationSocket.hpp"  

template <
    typename SockAddr
>
class CommunicationSocket : public ICommunicationSocket
{
    public:
        CommunicationSocket() {_fd = -1;};

        CommunicationSocket(const int fd, const SockAddr& addr) :
            
            _addr(addr) {_fd = fd;}

        ~CommunicationSocket()
        {
            if (_fd != -1)
                ::close(_fd);
        }

        // implementation of IOnEvents
        void            onClose() {};
        void            onRead() {};
        void            onWrite() {};        
        void            onError() {}; 
        
        // implementation  of ICommunicationFunctions
        void send()
        {
            write(this->_fd, "Hello, World!", 13);
        }

        void receive() {};

        // implementation of ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (this->_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (this->_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (this->_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (this->_addr.clone()); }  

    private:
        SockAddr                            _addr;


        
        CommunicationSocket(const CommunicationSocket& copy) : _addr(copy._addr) {_fd = copy._fd;}
        CommunicationSocket& operator=(const CommunicationSocket& assign)
        {   
            if (this == &assign)
                return (*this);
            _fd = assign._fd;
            _addr = assign._addr;
            return (*this);
        }

};


#endif