/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModuleGetSetAccess.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:21:04 by manuel            #+#    #+#             */
/*   Updated: 2024/12/09 16:06:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpModule.hpp"

// getters
const HttpManager& 		HttpModule::getHttpManager() const
{
	return (m_httpManager);
}

const ServerManager& 	HttpModule::getServerManager() const
{
	return (m_serverManager);
}

// accessors
HttpManager& 			HttpModule::accessHttpManager()
{
	return (m_httpManager);
}

ServerManager& 			HttpModule::accessServerManager()
{
	return (m_serverManager);
}
