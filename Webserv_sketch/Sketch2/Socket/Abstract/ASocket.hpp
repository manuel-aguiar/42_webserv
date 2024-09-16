/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 07:52:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

#define ISOCKET_HPP

# include "../../../LibftCpp/libftcpp.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"
# include "ISocketAddress.hpp"
# include "../Exceptions/ParameterException.hpp"

template <
    typename SockAddr
>
class ASocket : public FileDescriptor
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

};


#endif