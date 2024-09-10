/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv6Address.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:38:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 10:40:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPV6ADDRESS_HPP

# define IPV6ADDRESS_HPP

# include <netinet/in.h>

# include "ISocketAddress.hpp"


class IPv6Address : public ISocketAddress
{
    public:
        IPv6Address();
        IPv6Address(const IPv6Address& other);
        IPv6Address& operator=(const IPv6Address& other);
        ~IPv6Address();

        struct sockaddr*    getSockAddr() const;
        socklen_t           getSize() const;
        
    private:
        struct sockaddr_in6 _addr;
};


#endif