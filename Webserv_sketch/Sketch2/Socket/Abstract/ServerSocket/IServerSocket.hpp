/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:28:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:18:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERSOCKET_HPP

# define ISERVERSOCKET_HPP

#include "../../FileDescriptorManager/IFileDescriptorManager.hpp"
#include "../../FileDescriptor/FileDescriptor.hpp"

class IServerSocket : virtual public FileDescriptor
{
    public:
        virtual ~IServerSocket() {}

        virtual void                                                bind() = 0;
        virtual void                                                listen() = 0;
        virtual void                                                setFdManager(IFileDescriptorManager* fdManager) = 0;
};

#endif