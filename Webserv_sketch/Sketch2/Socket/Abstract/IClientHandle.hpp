/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IClientHandle.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:21:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:38:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTFUNCTIONS_HPP

# define ICLIENTFUNCTIONS_HPP

#include "../../FileDescriptor/FileDescriptor.hpp"

class IClientHandle : virtual public FileDescriptor
{
    public:
        virtual ~IClientHandle() {}

        virtual void connect() = 0;
        virtual void disconnect() = 0;
};


#endif