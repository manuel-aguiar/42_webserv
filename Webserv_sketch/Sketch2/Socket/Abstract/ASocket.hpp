/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:19 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:46:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET_HPP

#define ASOCKET_HPP

# include "../../../../LibftCpp/libftcpp.hpp"
# include "ISocket.hpp"

template <
    typename SockAddr
>
class ASocket;

# include "ASocket.tpp"

#endif