/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationSocket.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:39:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 12:43:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMUNICATIONSOCKET_HPP

# define COMMUNICATIONGSOCKET_HPP

# include "../Abstract/CommunicationSocket/ACommunicationSocket.hpp"  

template <
    typename SockAddr
>
class CommunicationSocket : public ACommunicationSocket<SockAddr>
{
    public:
        CommunicationSocket();

        CommunicationSocket(const int fd, const SockAddr& addr) :
            ASocket<SockAddr>(fd, addr) {}

        ~CommunicationSocket()
        {
            if (this->_fd != -1)
                ::close(this->_fd);
        }
        // implementation of FileDescriptor Functions
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

    private:
        CommunicationSocket(const CommunicationSocket& copy) : ASocket<SockAddr>(copy) {}
        CommunicationSocket& operator=(const CommunicationSocket& assign)
        {
            ASocket<SockAddr>::operator=(assign);
            return (*this);
        }

};


#endif