/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationSocket.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:39:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 12:43:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMUNICATIONSOCKET_HPP

# define COMMUNICATIONGSOCKET_HPP

# include "../Abstract/CommunicationSocket/ACommunicationSocket.hpp"  

template <
    typename SockAddr
>
class CommunicationSocket;

# include "CommunicationSocket.tpp"

#endif