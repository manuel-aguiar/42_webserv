/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   VirtualHost.hpp									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/30 13:56:11 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 14:22:39 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef VIRTUALHOST_HPP

# define VIRTUALHOST_HPP

/*
	VirtualHost is what is considered to be a website (directory tree).
	A server that "serves" content from multiple websites (directory trees), sees individual
	websites as "VirtualHosts"
*/

# include <string>

class VirtualHost
{
	public:
		VirtualHost();
		~VirtualHost();
		VirtualHost(const VirtualHost& copy);
		VirtualHost& operator=(const VirtualHost& assign);
	

	private:
		std::string _hostname;
};



#endif