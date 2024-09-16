/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:21:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:58:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTSOCKET_HPP

# define ICLIENTSOCKET_HPP

#include "../../../FileDescriptor/FileDescriptor.hpp"

# include "../IPollableSocket.hpp"
# include "IClientMethods.hpp"
# include "../CommunicationSocket/ICommunicationMethods.hpp"

class IClientSocket : public IPollableSocket, public IClientMethods, public ICommunicationMethods
{
    public:
        virtual ~IClientSocket() {}

};


#endif