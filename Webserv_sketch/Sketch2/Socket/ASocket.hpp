/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 17:35:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

#define ISOCKET_HPP

# include "../../../LibftCpp/libftcpp.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"
# include "SocketAddress/ISocketAddress.hpp"
# include "../Exceptions/ParameterException.hpp"

class ASocket : public FileDescriptor
{
    public:
        virtual ~ASocket();

    protected:
        UniquePtr<ISocketAddress>     _addr;
        
        //bare minimum, an fd and an address structure
        ASocket(const int fd, const ISocketAddress& addr);
        ASocket();

        //move
        ASocket(ASocket& copy);
        ASocket& operator=(ASocket& assign);

        //copy
        ASocket(const ASocket& copy);
        ASocket& operator=(const ASocket& assign);
    
    private:

};


#endif