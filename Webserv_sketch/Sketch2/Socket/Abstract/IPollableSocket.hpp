/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPollableSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:57:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:58:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLLABLESOCKET_HPP

# define IPOLLABLESOCKET_HPP

# include "ISocket.hpp"
# include "IOnEvents.hpp"

class IPollableSocket : public ISocket, public IOnEvents
{
    public:
        virtual ~IPollableSocket() {}
};


#endif