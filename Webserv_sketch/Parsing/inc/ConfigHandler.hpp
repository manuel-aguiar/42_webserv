#ifndef CONFIGHANDLER_HPP
# define CONFIGHANDLER_HPP

# include "utils.hpp"
# include "ServerConfig.hpp"

class ConfigHandler
{
	private:
		std::string		_path;
		std::ifstream	_file;
		size_t			_serverCount;
		ServerConfig	*_servers;				/* 'servers' address would be shared with
															 * another class that will be the sv manager?
															 * same for server_count	*/						
		int countServers();

	public:
		ConfigHandler();
		~ConfigHandler();

		int				getServerCount() const;
		std::string		getConfigPath() const;
		int		        setConfigPath(std::string new_path);
		int		        updateFile() ;

		int		        parse_config_line(std::string &line, ServerConfig &server, size_t &current_line);
		int		        parse_config_file();
		
		void	        print_server_config(const ServerConfig &server);
		void	        print_servers_config();
};

#endif
