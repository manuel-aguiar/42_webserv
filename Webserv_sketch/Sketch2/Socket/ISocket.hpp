/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:42:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISOCKET_HPP

#define ISOCKET_HPP

class ISocket
{
    public:
        virtual ~ISocket() {}
        virtual void close() = 0;
};

#endif