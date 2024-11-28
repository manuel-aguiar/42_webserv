/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:18:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 18:15:15 by manuel           ###   ########.fr       */
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

		HttpManager& 			accessManager();
		static void 			initConnection(Connection* accepted);

	private:
		HttpManager 	m_manager;

		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif
