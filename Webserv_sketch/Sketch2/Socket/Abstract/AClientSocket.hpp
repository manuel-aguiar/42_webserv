/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:21:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACLIENTSOCKET_HPP

# define ACLIENTSOCKET_HPP

# include "ASocket.hpp"
# include "IClientSocket.hpp"
# include "ICommunicationSocket.hpp"

template <
    typename SockAddr
>
class AClientSocket : virtual public ASocket<SockAddr>, public ICommunicationFunctions, public IClientSocket
{
    public:
        virtual ~AClientSocket() {}
};


#endif