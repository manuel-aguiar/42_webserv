/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISendReceiveSocket.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:10:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISENDRECEIVESOCKET_HPP

# define ISENDRECEIVESOCKET_HPP

# include "ISocket.hpp"

class ISendReceiveSocket : public ISocket
{
    public:
        virtual ~ISendReceiveSocket() {}

        virtual void send() = 0;
        virtual void receive() = 0;
};


#endif