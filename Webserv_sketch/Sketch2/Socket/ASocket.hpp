/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 13:31:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

#define ISOCKET_HPP

# include "../FileDescriptor/FileDescriptor.hpp"
# include "SocketAddress/ISocketAddress.hpp"
# include "../Exceptions/ParameterException.hpp"

class ASocket : public FileDescriptor
{
    public:
        virtual ~ASocket() {}
        virtual void close() = 0;

    protected:
        ISocketAddress*     _addr;
        
        //bare minimum, an fd and an address structure
        ASocket(const int fd, const ISocketAddress& addr);
        ASocket();
        ASocket(const ASocket& copy);
        ASocket& operator=(const ASocket& assign);
    
    private:

};


#endif