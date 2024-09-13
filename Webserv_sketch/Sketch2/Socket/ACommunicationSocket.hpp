/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:27:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:42:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMUNICATIONSOCKET_HPP

# define ACOMMUNICATIONSOCKET_HPP

# include "ASocket.hpp"
# include "IFunctionsCommunication.hpp"

class ACommunicationSocket : virtual public ASocket, public ICommunicationFunctions
{
    public:
        virtual ~ACommunicationSocket() {}

};



#endif