#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <set>

# define DEFAULT_CONFIG_PATH "webserv.conf"
# define DEFAULT_HOST "127.0.0.1"
# define DEFAULT_CLIENT_BODY_SIZE "1M" // 1MB
<<<<<<< HEAD
=======
# define SERVER_AMOUNT 2 // ConfigHandler.countServers will count the sv's on a config file
>>>>>>> 07d7f4329404fa3ff0401817dcf7747980e9879d

// Conversion
size_t		__stoull(const std::string &str);
std::string	strtrim(const std::string &str);
size_t		parse_size(const std::string &size_str);

std::vector<std::string> split(const std::string& str, char delimiter);

// Validation
<<<<<<< HEAD
bool			is_number(const std::string &str);
bool			validate_ipv4(const std::string &str);
=======
int			is_number(const std::string &str);
int			validate_ipv4(const std::string &str);
>>>>>>> 07d7f4329404fa3ff0401817dcf7747980e9879d

#endif