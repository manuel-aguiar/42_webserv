/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:18:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 16:56:40 by manuel           ###   ########.fr       */
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

		static void initConnection(Connection* accepted);

		// public, yes, omg :0
		HttpManager 	manager;

	private:
		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif
