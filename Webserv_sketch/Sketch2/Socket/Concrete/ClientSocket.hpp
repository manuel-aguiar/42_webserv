/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:04:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:35:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

# include "../Abstract/ClientSocket/IClientSocket.hpp"
# include "../../FileDescriptor/IOnEvents.hpp"

template <
    typename SockAddr
>
class ClientSocket;

# include "ClientSocket.tpp"

#endif