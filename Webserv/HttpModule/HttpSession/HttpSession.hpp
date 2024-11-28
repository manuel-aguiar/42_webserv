/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSession.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:18:14 by codespace         #+#    #+#             */
/*   Updated: 2024/11/28 11:05:19 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSESSION_HPP

# define HTTPSESSION_HPP

//C++ headers
# include <string>
# include <cstddef>

class HttpSession
{
	public:
		HttpSession();
		~HttpSession();

	private:

		size_t	  			m_timeout;
		std::string         m_session_id;
		size_t      		m_logged_times;
};

#endif
