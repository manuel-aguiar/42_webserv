/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 12:52:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVERSOCKET_HPP

# define ASERVERSOCKET_HPP

# include "../ASocket.hpp"
# include "IServerSocketAccept.hpp"
# include "IServerSocket.hpp"

template <
    typename SockAddr
>
class AServerSocket : virtual public ASocket<SockAddr>, public IServerSocketAccept<SockAddr>, virtual public IServerSocket
{
    public:
        virtual                         ~AServerSocket() {}
       
};

#endif