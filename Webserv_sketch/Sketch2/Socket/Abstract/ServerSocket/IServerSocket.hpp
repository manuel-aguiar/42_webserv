/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:28:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:08:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERSOCKET_HPP

# define ISERVERSOCKET_HPP

#include "../ISocket.hpp"
#include "IServerMethods.hpp"

class IServerSocket : public ISocket, public IServerMethods
{
    public:
        virtual ~IServerSocket() {}
};

#endif