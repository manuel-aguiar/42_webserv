/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:04:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 10:24:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP

# define CGI_HPP

//own headers
# include "CgiDefaults.h"

// C++ headers
# include <string>

class CgiModule
{
	public:
		CgiModule(const std::string& interpreterPath);
		~CgiModule();

		static const char*	const		m_defaultEnvp[ENV_CGI_VAR_COUNT];

	private:
		std::string     m_interpreterPath;



		CgiModule();
};

#endif