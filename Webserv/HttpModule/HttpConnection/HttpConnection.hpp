/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:14:02 by codespace         #+#    #+#             */
/*   Updated: 2024/12/09 16:13:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

//C++ headers
# include <vector>
# include <queue>
# include <cstddef>

class Connection;
class HttpRequest;
class HttpResponse;
class Event;


class HttpConnection
{
	public:
		HttpConnection(Connection* newConnection);
		~HttpConnection();



	private:

		size_t							m_timeout;
		Connection*						m_connection;
		std::queue<HttpRequest*>		m_requests;
		std::queue<HttpResponse*>		m_responses;


		HttpConnection(const HttpConnection& copy);
		HttpConnection& operator=(const HttpConnection& assign);
};



#endif
