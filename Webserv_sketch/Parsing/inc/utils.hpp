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
# define DEFAULT_PORT "8080"
# define DEFAULT_CLIENT_BODY_SIZE "1M" // 1MB

// Conversion
size_t		__stoull(const std::string &str);
std::string	strtrim(const std::string &str);
size_t		parse_size(const std::string &size_str);

std::vector<std::string> split(const std::string& str, char delimiter);

// Validation
bool			is_number(const std::string &str);
bool			validate_ipv4(const std::string &str);

#endif