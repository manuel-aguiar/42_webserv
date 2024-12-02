/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:16:50 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 10:43:23 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPINTERPRETER_HPP

# define HTTPINTERPRETER_HPP

# include <map>
# include <set>
# include <string>

class HttpConnection;
class HttpSession;
class ServerManager;

class HttpManager
{
	public:
		HttpManager(ServerManager& serverManager);
		~HttpManager();

		void	addConnection(HttpConnection* connection);
		void	removeConnection(HttpConnection* connection);

	private:

		//typedef std::string t_sessionID;								// still to figure out

		ServerManager&							m_serverManager;
		std::set<HttpConnection*>				m_connections;
		
		//std::map<t_sessionID, HttpSession>		m_sessions;				// still to figure out


		HttpManager(const HttpManager& copy);
		HttpManager& operator=(const HttpManager& assign);

};



#endif


/*

http
{
	server {
		listen 80;
	}
}

mail
{
	server {
		listen 80;
	}
}


*/
