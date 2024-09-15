/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFunctionsServer.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:14:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 16:34:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFUNCTIONSSERVER_HPP

# define IFUNCTIONSSERVER_HPP

# include "ACommunicationSocket.hpp"

template <
    typename SockAddr
>
class IServerFunctions
{
    public:
        virtual ~IServerFunctions() {}

        virtual void                                                bind() = 0;
        virtual void                                                listen() = 0;
        virtual UniquePtr<ACommunicationSocket<SockAddr> >          accept() = 0;
};


#endif