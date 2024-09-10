/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv4Address.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:36:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:08:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPV4ADDRESS_HPP

# define IPV4ADDRESS_HPP

// C++ headers
# include <string>
# include <cstring>
# include <stdexcept>

// C Headers
# include <netinet/in.h>
# include <arpa/inet.h>

//Project headers
# include "ISocketAddress.hpp"

class IPv4Address : public ISocketAddress
{
    public:
        
        IPv4Address(const sockaddr_in& addr);
        IPv4Address(const std::string& ip, uint16_t port);
        IPv4Address(uint32_t ip, uint16_t port);

        IPv4Address(const IPv4Address& other);
        IPv4Address& operator=(const IPv4Address& other);
        ~IPv4Address();

        struct sockaddr*    getSockAddr();
        socklen_t           getSize() const;
        
    private:
        struct sockaddr_in _addr;

        IPv4Address();
};

#endif