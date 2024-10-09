/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationSocket.tpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:07:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMUNICATIONSOCKET_TPP

# define COMMUNICATIONSOCKET_TPP

# include "../Abstract/CommunicationSocket/ICommunicationSocket.hpp"  
# include "../../FileDescriptor/IOnEvents.hpp"

template <
    typename SockAddr
>
class CommunicationSocket : 
    public ICommunicationSocket, 
    public IOnEvents
{
    public:
        CommunicationSocket() {m_fd = -1;};

        CommunicationSocket(const int fd, const SockAddr& addr) :
            
            m_addr(addr) {m_fd = fd;}

        ~CommunicationSocket()
        {
            if (m_fd != -1)
                ::close(m_fd);
        }

        // implementation of IOnEvents
        void            onClose() {};
        void            onRead() {};
        void            onWrite() {};        
        void            onError() {}; 
        
        // implementation  of ICommunicationFunctions
        void send()
        {
            write(this->m_fd, "Hello, World!", 13);
        }

        void receive() {};

        // implementation of ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (this->m_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (this->m_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (this->m_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (this->m_addr.clone()); }  

    private:
        SockAddr                            m_addr;


        
        CommunicationSocket(const CommunicationSocket& copy) : m_addr(copy.m_addr) {m_fd = copy.m_fd;}
        CommunicationSocket& operator=(const CommunicationSocket& assign)
        {   
            if (this == &assign)
                return (*this);
            m_fd = assign.m_fd;
            m_addr = assign.m_addr;
            return (*this);
        }

};


#endif