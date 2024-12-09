/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:18:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/09 16:07:13 by mmaria-d         ###   ########.fr       */
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
		ServerManager& 			m_serverManager;
		HttpManager 			m_httpManager;

		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif
