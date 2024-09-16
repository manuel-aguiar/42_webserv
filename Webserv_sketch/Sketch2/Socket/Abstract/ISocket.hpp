/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:59:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:13:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

# define ISOCKET_HPP

# include "../FileDescriptor/FileDescriptor.hpp"
# include "ISocketAddress.hpp"

class ISocket : virtual public FileDescriptor, public ISocketAddress
{
    public:
        virtual         ~ISocket() {}
};

#endif