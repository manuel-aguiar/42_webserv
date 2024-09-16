/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFunctionsServerAccept.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:14:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:38:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFUNCTIONSSERVER_HPP

# define IFUNCTIONSSERVER_HPP

# include "ACommunicationSocket.hpp"

template <
    typename SockAddr
>
class IServerAccept
{
    public:
        virtual ~IServerAccept() {}

        virtual UniquePtr<ACommunicationSocket<SockAddr> >          accept() = 0;
};


#endif