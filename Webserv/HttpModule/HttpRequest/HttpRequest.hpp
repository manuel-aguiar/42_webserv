/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:17:04 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 11:06:45 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

// C++ Headers
# include <string>
# include <map>


class HttpConnection;
class HttpSession;


class HttpRequest
{

	public:
		HttpRequest();


	private:

		size_t				m_timeout;

		HttpConnection*		m_httpConn;
		HttpSession*		m_session;



		//setters forever

		//variables to be filed by parsing the request
		// ideas:    nginx-master/src/http/ngx_http_parse.c			//in reality they store pointers to where components start and end to avoid copying everything

		std::string 						m_method;
		std::string 						m_uri;
		std::string 						m_httpVersion;
		std::map<std::string, std::string> 	m_headers;

			// URI components
		std::string							m_host;
		std::string							m_resource;
		std::string							m_query;
		std::string							m_fragment;

		std::string 						m_body;



		//other variables to connect to the structure
		// ...
};


#endif
