/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 16:45:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACLIENTSOCKET_HPP

# define ACLIENTSOCKET_HPP

# include "ASocket.hpp"
# include "IFunctionsClient.hpp"
# include "IFunctionsCommunication.hpp"

template <
    typename SockAddr
>
class AClientSocket : virtual public ASocket<SockAddr>, public ICommunicationFunctions, public IClientFunctions
{
    public:
        virtual ~AClientSocket() {}
};


#endif