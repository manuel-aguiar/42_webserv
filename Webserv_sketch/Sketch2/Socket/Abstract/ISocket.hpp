/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:59:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:04:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

# define ISOCKET_HPP

# include "IPollableFileDescriptor.hpp"
# include "ISocketAddress.hpp"

class ISocket : public IPollableFileDescriptor, public ISocketAddress
{
    public:
        virtual         ~ISocket() {}
};

#endif