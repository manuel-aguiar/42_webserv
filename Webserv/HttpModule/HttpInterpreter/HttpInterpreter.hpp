/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpInterpreter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:16:50 by codespace         #+#    #+#             */
/*   Updated: 2024/11/26 15:24:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPINTERPRETER_HPP

# define HTTPINTERPRETER_HPP

# include <map>

class HttpSession;

class HttpInterpreter
{
	public:
		HttpInterpreter();
		~HttpInterpreter();

	private:

		typedef std::string t_sessionID;

		std::map<t_sessionID, HttpSession>	m_sessions;


		HttpInterpreter(const HttpInterpreter& copy);
		HttpInterpreter& operator=(const HttpInterpreter& assign);

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