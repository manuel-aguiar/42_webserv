/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:27:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:18:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMUNICATIONSOCKET_HPP

# define ACOMMUNICATIONSOCKET_HPP

# include "ASocket.hpp"
# include "ICommunicationSocket.hpp"

template <
    typename SockAddr
>
class ACommunicationSocket : virtual public ASocket<SockAddr>, public ICommunicationSocket
{
    public:
        virtual ~ACommunicationSocket() {}

};



#endif