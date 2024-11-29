/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:18:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 16:20:43 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

# include "HttpManager/HttpManager.hpp"

class Connection;

class HttpModule
{
	public:
		HttpModule(ServerManager& serverManager);
		~HttpModule();

		// Module entry point
		static void 			initConnection(Connection* accepted);

		// getters
		const HttpManager& 		getHttpManager() const;
		const ServerManager& 	getServerManager() const;

		//no setters needed, Module needs a serverManager to start

		// accessors
		HttpManager& 			accessHttpManager();
		ServerManager& 			accessServerManager();

	private:
		HttpManager 			m_httpmanager;
		ServerManager& 			m_serverManager;

		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif
