/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:59:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONSOCKET_HPP

# define ICOMMUNICATIONSOCKET_HPP

# include "../../../FileDescriptor/FileDescriptor.hpp"
# include "../IPollableSocket.hpp"
# include "ICommunicationMethods.hpp"

class ICommunicationSocket : public IPollableSocket, public ICommunicationMethods
{
    public:
        virtual ~ICommunicationSocket() {}

};


#endif