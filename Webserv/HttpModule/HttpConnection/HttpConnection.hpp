/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:14:02 by codespace         #+#    #+#             */
/*   Updated: 2024/11/29 16:24:04 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

//C++ headers

# include <vector>
# include <queue>

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
		std::queue<HttpRequest>			m_requests;
		std::queue<HttpResponse>		m_responses;


		HttpConnection(const HttpConnection& copy);
		HttpConnection& operator=(const HttpConnection& assign);
};



#endif
