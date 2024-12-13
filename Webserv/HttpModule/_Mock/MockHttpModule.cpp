/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockHttpModule.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:10:13 by manuel            #+#    #+#             */
/*   Updated: 2024/12/13 16:59:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../HttpModule.hpp"
# include "../../Connection/Connection.hpp"
# include "../HttpConnection/HttpConnection.hpp"

HttpModule::HttpModule(ServerManager& serverManager) :
	m_serverManager(serverManager),
	m_httpManager(m_serverManager)
{

}

HttpModule::~HttpModule()
{

}


/*
	Connection has: pointer to the module.
	The init function:
	- allocates an HttpConnection class inside the Connection memory pool,
	- links Connection to HttpConnection,
	- tells the httpmanager to store that pointer (it may not be needed in the end if httpconnections are truly independent)
		(in which case, removing sessions the httpmanager state is rather useless and we let each Connection manage itself with even handlers....)
*/
void HttpModule::initConnection(Connection* accepted)
{
    (void)accepted;

    std::cout << "HttpModule::initConnection called: Hello!!" << std::endl;
}



//private
HttpModule::HttpModule(const HttpModule& copy) :
	m_serverManager(copy.m_serverManager),
	m_httpManager(m_serverManager)
{

}

HttpModule& HttpModule::operator=(const HttpModule& assign) {(void)assign; return (*this);}