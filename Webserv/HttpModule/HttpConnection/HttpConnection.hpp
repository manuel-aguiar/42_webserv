/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:14:02 by codespace         #+#    #+#             */
/*   Updated: 2024/11/26 15:14:38 by mmaria-d         ###   ########.fr       */
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
		HttpConnection();
		~HttpConnection();

	private:
		std::queue<HttpRequest>			m_requests;
		std::queue<HttpResponse>		m_responses;

		HttpConnection(const HttpConnection& copy);
		HttpConnection& operator=(const HttpConnection& assign);
};



#endif
