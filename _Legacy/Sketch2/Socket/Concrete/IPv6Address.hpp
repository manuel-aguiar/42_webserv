/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv6Address.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:38:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#ifndef IPV6ADDRESS_HPP

# define IPV6ADDRESS_HPP



// C++ headers
# include <string>

// C Headers
# include <netinet/in.h>
# include <arpa/inet.h>

//Project headers
# include "../Abstract/ISocketAddress.hpp"


class IPv6Address : public ISocketAddress
{
    public:
        IPv6Address();

        IPv6Address(const sockaddr_in& addr);
        IPv6Address(const std::string& ip6, uint16_t port);
        IPv6Address(uint8_t[16], uint16_t port);

        IPv6Address(const IPv6Address& other);
        IPv6Address& operator=(const IPv6Address& other);
        ~IPv6Address();

        struct sockaddr*    getSockAddr() const;
        socklen_t           getSize() const;
        
    private:
        struct sockaddr_in6 m_addr;
};


#endif


