/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:10:13 by manuel            #+#    #+#             */
/*   Updated: 2024/11/29 09:51:07 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpModule.hpp"
# include "../Connection/Connection.hpp"
# include "HttpConnection/HttpConnection.hpp"

HttpModule::HttpModule(ServerManager& serverManager) : m_manager(serverManager)
{

}

HttpModule::~HttpModule()
{

}

void HttpModule::initConnection(Connection* accepted)
{
	HttpManager& 	manager = (reinterpret_cast<HttpModule*>(accepted->accessProtoModule()))->accessManager();

	HttpConnection* conn = new HttpConnection(accepted);
	accepted->setProtoConnection(conn);
	manager.addConnection(conn);
}


