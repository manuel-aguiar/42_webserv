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

# include "ACommunicationSocket.hpp"  

class CommunicationSocket : public ACommunicationSocket
{
    public:
        CommunicationSocket();

        CommunicationSocket(const int fd, const ISocketAddress& addr);

        //move
        CommunicationSocket(CommunicationSocket& copy);
        CommunicationSocket& operator=(CommunicationSocket& assign);

        ~CommunicationSocket();

        // implementation of FileDescriptor Functions
        void            onClose();
        void            onRead();
        void            onWrite();        
        
        // implementation  of ICommunicationFunctions
        void send();
        void receive();

    private:
        CommunicationSocket(const CommunicationSocket& copy);
        CommunicationSocket& operator=(const CommunicationSocket& assign);

};


#endif