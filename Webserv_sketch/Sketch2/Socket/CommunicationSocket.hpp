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

#ifndef COMMUNICATINGSOCKET_HPP

# define COMMUNICATINGSOCKET_HPP

# include "ACommunicationSocket.hpp"  

class CommunicationSocket : public ACommunicationSocket
{
    public:
        CommunicationSocket(int fd, const ISocketAddress& addr);
        ~CommunicationSocket();


        // implementation  of ASocket
        void close();
        
        // implementation  of ICommunicationFunctions
        void send();
        void receive();
};


#endif