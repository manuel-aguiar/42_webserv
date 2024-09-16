/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerMethods.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:53:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:14:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERMETHODS_HPP

# define ISERVERMETHODS_HPP

#include "../../../FileDescriptorManager/IFileDescriptorManager.hpp"
#include "../CommunicationSocket/ICommunicationSocket.hpp"


class IServerMethods
{
    public:
        virtual ~IServerMethods() {}

        virtual void                                    bind() = 0;
        virtual void                                    listen() = 0;
        virtual UniquePtr<ICommunicationSocket>         accept() = 0;
        virtual void                                    setFdManager(IFileDescriptorManager* fdManager) = 0;
};


#endif