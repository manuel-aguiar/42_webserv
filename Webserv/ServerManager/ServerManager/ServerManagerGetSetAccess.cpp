/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerGetSetAccess.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:03:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:50:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
# include "ServerManager.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"


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

const t_ptr_ProtoModule*	ServerManager::getProtoModules() const
{
	return (m_protoModules);
}

const t_func_initProtoConn*	ServerManager::getInitProtoConnections() const
{
	return (m_initProtoConnection);
}