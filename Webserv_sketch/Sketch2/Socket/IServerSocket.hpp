/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:09:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERSOCKET_HPP

# define ISERVERSOCKET_HPP

# include "ISocket.hpp"
# include "ISendReceiveSocket.hpp"

class IServerSocket : public ISocket
{
    public:
        virtual                         ~IServerSocket() {}
        virtual void                    bind() = 0;
        virtual void                    listen() = 0;
        virtual ISendReceiveSocket*     accept() = 0;
};

#endif