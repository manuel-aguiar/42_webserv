/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 17:11:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVERSOCKET_HPP

# define ASERVERSOCKET_HPP

# include "ASocket.hpp"
# include "IFunctionsServer.hpp"

template <
    typename SockAddr
>
class AServerSocket : virtual public ASocket<SockAddr>, public IServerFunctions<SockAddr>
{
    public:
        virtual                         ~AServerSocket() {}
       
};

#endif