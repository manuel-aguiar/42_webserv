/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:10:13 by manuel            #+#    #+#             */
/*   Updated: 2024/11/28 16:55:21 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpModule.hpp"
# include "HttpConnection/HttpConnection.hpp"

HttpModule::HttpModule(ServerManager& serverManager) : manager(serverManager)
{

}

HttpModule::~HttpModule()
{

}

void HttpModule::initConnection(Connection* accepted)
{
	HttpConnection* conn = new HttpConnection(accepted);
}
