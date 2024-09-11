/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:03:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERSOCKET_HPP

# define ISERVERSOCKET_HPP

# include "ASocket.hpp"
# include "IFunctionsServer.hpp"

class AServerSocket : virtual public ASocket, public IServerFunctions
{
    public:
        virtual                         ~AServerSocket() {}
       
};

#endif