/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:09:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTSOCKET_HPP

# define ICLIENTSOCKET_HPP

# include "ISocket.hpp"
# include "ISendReceiveSocket.hpp"

class IClientSocket : public ISocket, public ISendReceiveSocket
{
    public:
        virtual ~IClientSocket() {}

        virtual void connect() = 0;
        virtual void disconnect() = 0;
};


#endif