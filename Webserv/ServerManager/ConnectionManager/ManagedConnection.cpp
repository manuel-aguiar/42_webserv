/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManagedConnection.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 08:57:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 08:59:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConnectionManager.hpp"

ConnectionManager::ManagedConnection::ManagedConnection(Globals& globals) :
	Connection(globals)
{
}

ConnectionManager::ManagedConnection::~ManagedConnection()
{
}

// private
ConnectionManager::ManagedConnection::ManagedConnection(const ManagedConnection& other) : Connection(other)
{
	*this = other;
}

ConnectionManager::ManagedConnection& ConnectionManager::ManagedConnection::operator=(const ManagedConnection& other)
{
	if (this != &other)
		Connection::operator=(other);
	return (*this);
}