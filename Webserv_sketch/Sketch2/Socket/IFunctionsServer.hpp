/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFunctionsServer.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:14:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 09:43:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFUNCTIONSSERVER_HPP

# define IFUNCTIONSSERVER_HPP

# include "ACommunicationSocket.hpp"

class IServerFunctions
{
    public:
        virtual ~IServerFunctions() {}

        virtual void                                    bind() = 0;
        virtual void                                    listen() = 0;
        virtual UniquePtr<ACommunicationSocket>         accept() = 0;
};


#endif