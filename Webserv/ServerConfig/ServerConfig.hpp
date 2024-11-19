/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:50:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/19 13:23:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP

# define SERVERCONFIG_HPP

class DefaultConfig;
class ServerBlocks;
class Globals;

class ServerConfig
{
    public:
        ServerConfig(const char* configFile, const DefaultConfig& configDefault, Globals* globals);
        

    private:
        ServerConfig();
        

};




#endif