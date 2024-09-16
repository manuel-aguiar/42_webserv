/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:40:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET_HPP

#define ASOCKET_HPP

# include "../../../../LibftCpp/libftcpp.hpp"
# include "ISocket.hpp"

template <
    typename SockAddr
>
class ASocket : public ISocket
{
    public:
        virtual ~ASocket();

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


# include "ASocket.tpp"

#endif