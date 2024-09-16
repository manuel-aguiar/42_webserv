/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:47:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACLIENTSOCKET_HPP

# define ACLIENTSOCKET_HPP

# include "../ASocket.hpp"
# include "IClientSocket.hpp"
# include "../CommunicationSocket/ICommunicationSocket.hpp"	

template <
    typename SockAddr
>
class AClientSocket : virtual public ASocket<SockAddr>, public ICommunicationSocket, public IClientSocket
{
    public:
        virtual ~AClientSocket() {}
};


#endif