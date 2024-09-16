/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerHandle.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:28:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:32:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERHANDLE_HPP

# define ISERVERHANDLE_HPP

#include "../../FileDescriptorManager/IFileDescriptorManager.hpp"
#include "../../FileDescriptor/FileDescriptor.hpp"

class IServerHandle : virtual public FileDescriptor
{
    public:
        virtual ~IServerHandle() {}

        virtual void                                                bind() = 0;
        virtual void                                                listen() = 0;
        virtual void                                                setFileManager(IFileDescriptorManager* fdManager) = 0;
};

#endif