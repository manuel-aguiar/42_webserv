/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:18:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 17:11:12 by manuel           ###   ########.fr       */
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

		HttpManager& accessManager();
		static void initConnection(Connection* accepted);

	private:

		// public, yes, omg :0
		HttpManager 	m_manager;

		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif
