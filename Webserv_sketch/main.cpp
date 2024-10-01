/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 08:43:05 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 14:17:08 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Webserver_Definitions.h"
#include "Server/Server.hpp"
#include "Clock/Clock.hpp"

#define GRET_SUCKCESS EXIT_SUCCESS

int main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;

	LogFile log("log.txt");
	Server server(&log);

	server.createListeners(NULL, "8080", SOCK_STREAM, AF_UNSPEC, 10);

	Clock clock;

	clock.start();

	std::cout << clock.get_FormatedTime() << std::endl;

	std::cout << server._listeners.size() << std::endl;	
}