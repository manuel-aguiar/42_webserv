/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:14:02 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 17:06:20 by manuel           ###   ########.fr       */
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


class HttpConnection
{
	public:
		HttpConnection(Connection* newConnection);
		~HttpConnection();



		size_t							m_timeout;
		Connection*						m_connection;
		std::queue<HttpRequest>			m_requests;
		std::queue<HttpResponse>		m_responses;


	private:
		HttpConnection(const HttpConnection& copy);
		HttpConnection& operator=(const HttpConnection& assign);
};



#endif
