/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestDependencies.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:20:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/10 10:09:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTDEPENDENCIES_HPP

# define TESTDEPENDENCIES_HPP

//c++ headers
# include <string>

// TODO: remove temporary declerations
class ServerConfig {
	public:
		ServerConfig() {}
		~ServerConfig() {}
};

class ServerBlock {
	public:
		ServerBlock(const std::string& id) : m_id(id) {}
		~ServerBlock() {}

		ServerBlock&	operator=(const ServerBlock& other) {
			if (this == &other)
				return (*this);

			return (*this);
		}

		// used for testing
		std::string		id() const { return (m_id); }

	private:
		std::string		m_id;
};
// TODO: remove temporary declerations until here

#endif