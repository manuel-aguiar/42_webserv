#include "utils.hpp"

size_t	__stoull(const std::string &str)
{
	std::stringstream	ss(str);
	size_t				value;

	ss >> value;
    if (ss.fail() || !ss.eof())
		throw (std::invalid_argument("Invalid value (not a number): " + str));
	return (value);
}

std::string	strtrim(const std::string &str)
{
	if (str.empty())
		return (str);

	size_t	first	= str.find_first_not_of(' ');
	size_t	last	= str.find_last_not_of(' ');

	return (str.substr(first, (last - first + 1)));
}

size_t	parse_size(const std::string &size_str)
{
	size_t	multiplier = 1;
	std::reverse_iterator<std::string::const_iterator>	it;
	std::string	value = size_str.substr(0, size_str.size() - 1);

	if (!is_number(value))
		throw (std::invalid_argument("Invalid value: " + size_str));

	it = size_str.rbegin();
	if (*it == 'K')
		multiplier = 1024;
	else if (*it == 'M')
		multiplier = 1024 * 1024;
	else if (*it == 'G')
		multiplier = 1024 * 1024 * 1024;
	else if (!isdigit(*it))
<<<<<<< HEAD
		throw (std::invalid_argument("Invalid unit "));
=======
		throw (std::invalid_argument("Invalid unit: " + *it));
>>>>>>> 07d7f4329404fa3ff0401817dcf7747980e9879d
	else
		return (__stoull(size_str) * multiplier);
	return (__stoull(value) * multiplier);
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::istringstream			tokenStream(str);

	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);

	return (tokens);
}

<<<<<<< HEAD
bool	is_number(const std::string &str)
=======
int	is_number(const std::string &str)
>>>>>>> 07d7f4329404fa3ff0401817dcf7747980e9879d
{
	return (!str.empty() && (str.find_first_not_of("[0123456789]") == std::string::npos));
}

<<<<<<< HEAD
bool	validate_ipv4(const std::string &str)
=======
int	validate_ipv4(const std::string &str)
>>>>>>> 07d7f4329404fa3ff0401817dcf7747980e9879d
{
	std::vector<std::string> values = split(str, '.');

	if (values.size() != 4)
		return (0);
	for (int i = 0; i < 4; i++)
	{
		if (!is_number(values[i]) || __stoull(values[i]) > 255)
			return (0);
	}

	return (1);
}