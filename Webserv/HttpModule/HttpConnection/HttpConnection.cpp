/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:14:12 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 17:08:40 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpConnection.hpp"

HttpConnection::HttpConnection(Connection* newConnection) :
	m_connection(newConnection)
{

}
