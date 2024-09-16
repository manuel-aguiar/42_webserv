/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:30:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVERSOCKET_HPP

# define ASERVERSOCKET_HPP

# include "ASocket.hpp"
# include "IFunctionsServerAccept.hpp"
# include "IServerHandle.hpp"

template <
    typename SockAddr
>
class AServerSocket : virtual public ASocket<SockAddr>, public IServerAccept<SockAddr>, public IServerHandle
{
    public:
        virtual                         ~AServerSocket() {}
       
};

#endif