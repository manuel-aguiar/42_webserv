/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:05:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONSOCKET_HPP

# define ICOMMUNICATIONSOCKET_HPP

# include "../../../FileDescriptor/FileDescriptor.hpp"
# include "../ISocket.hpp"
# include "ICommunicationMethods.hpp"

class ICommunicationSocket : public ISocket, public ICommunicationMethods
{
    public:
        virtual ~ICommunicationSocket() {}

};


#endif