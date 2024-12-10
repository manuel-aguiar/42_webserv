/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerGetSetAccess.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:03:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/10 10:19:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig/ServerConfig.hpp"

//C++ headers
# include <cassert>

//getters
const ServerConfig&	ServerManager::getConfig() const
{
	return (m_config);
}

size_t	ServerManager::getListenerCount() const
{
	return (m_listenerCount);
}

//accessors
t_ptr_ProtoModule		ServerManager::accessProtoModule(e_protoModules module)
{
	return (m_protoModules[module]);
}