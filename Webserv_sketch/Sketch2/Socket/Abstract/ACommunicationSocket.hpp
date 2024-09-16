/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:27:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:38:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMUNICATIONSOCKET_HPP

# define ACOMMUNICATIONSOCKET_HPP

# include "ASocket.hpp"
# include "ICommunicationHandle.hpp"

template <
    typename SockAddr
>
class ACommunicationSocket : virtual public ASocket<SockAddr>, public ICommunicationHandle
{
    public:
        virtual ~ACommunicationSocket() {}

};



#endif