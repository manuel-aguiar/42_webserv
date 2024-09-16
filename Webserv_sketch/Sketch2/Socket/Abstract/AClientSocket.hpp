/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:18:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACLIENTSOCKET_HPP

# define ACLIENTSOCKET_HPP

# include "ASocket.hpp"
# include "IClientHandle.hpp"
# include "ICommunicationHandle.hpp"

template <
    typename SockAddr
>
class AClientSocket : virtual public ASocket<SockAddr>, public ICommunicationFunctions, public IClientSocket
{
    public:
        virtual ~AClientSocket() {}
};


#endif