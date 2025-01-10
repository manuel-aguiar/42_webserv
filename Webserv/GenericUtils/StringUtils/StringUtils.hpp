#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

#include <string>
#include <sstream>
#include <vector>

std::string	strtrim(const std::string &str);
size_t	stoull(const std::string &str);
size_t	parse_size(const std::string &size_str);
std::vector<std::string> split(const std::string &str, char delimiter);
std::string					strtrim(const std::string &str);
size_t						stoull(const std::string &str);
size_t						parse_size(const std::string &size_str);
std::vector<std::string>	split(const std::string &str, char delimiter);
std::string					strToLower(const std::string& str);


class StringUtils
{
    public:
        static void move(std::string& dest, std::string& src);

        template <typename T>
        static std::string to_string(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
};

#endif
