/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:04:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:09:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

# include "../Abstract/ClientSocket/AClientSocket.hpp"

template <
    typename SockAddr
>
class ClientSocket : public AClientSocket<SockAddr>
{
    public:
        ClientSocket();
        ClientSocket(const SockAddr& addr, int type, int protocol);
        ~ClientSocket();
        
        // implementation of ISocket Functions
        void            close();

        // implementation of FileDescriptor Functions
        void            onClose();
        void            onRead();
        void            onWrite();  
        void            onError(); 
        
        //implementation of IClientFunctions
        void            connect();
        void            disconnect();

        //implementation of ICommunicationFunctions
        void            send();
        void            receive();

    private:
        //no copying
        ClientSocket(const ClientSocket& copy);
        ClientSocket& operator=(const ClientSocket& assign);
};

# include "ClientSocket.tpp"

#endif