/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv4Address.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:36:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 10:39:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPV4ADDRESS_HPP

# define IPV4ADDRESS_HPP

# include <netinet/in.h>

# include "ISocketAddress.hpp"

class IPv4Address : public ISocketAddress
{
    public:
        IPv4Address();
        IPv4Address(const IPv4Address& other);
        IPv4Address& operator=(const IPv4Address& other);
        ~IPv4Address();

        struct sockaddr*    getSockAddr() const;
        socklen_t           getSize() const;
        
    private:
        struct sockaddr_in _addr;
};

#endif