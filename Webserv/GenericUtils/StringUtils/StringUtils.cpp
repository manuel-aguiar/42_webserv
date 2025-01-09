#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Validation/Validation.hpp"

std::string strtrim(const std::string &str)
{
	if (str.empty())
		return (str);

	size_t first = 0;
	while (first < str.size() && std::isspace(str[first]))
		++first;
	size_t last = str.size() - 1;
	while (last > first && std::isspace(str[last]))
		--last;

	return (str.substr(first, last - first + 1));
}

size_t	stoull(const std::string &str)
{
	std::stringstream	ss(str);
	size_t				value;

	ss >> value;
    if (ss.fail() || !ss.eof())
		throw (std::invalid_argument("Invalid value (not a number): " + str));
	return (value);
}

size_t	parse_size(const std::string &size_str)
{
	size_t	multiplier = 1;
	std::reverse_iterator<std::string::const_iterator>	it;
	std::string	value = size_str.substr(0, size_str.size() - 1);

	if (!isNumber(value))
		throw (std::invalid_argument("Invalid value: " + size_str));

	it = size_str.rbegin();
	if (*it == 'K')
		multiplier = 1024;
	else if (*it == 'M')
		multiplier = 1024 * 1024;
	else if (*it == 'G')
		multiplier = 1024 * 1024 * 1024;
	else if (!isdigit(*it))
		throw (std::invalid_argument("Invalid unit "));
	else
		return (stoull(size_str) * multiplier);
	return (stoull(value) * multiplier);
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

std::string	strToLower(const std::string& str)
{
	std::string lowercaseString = str;
	for (std::string::iterator it = lowercaseString.begin(); it != lowercaseString.end(); ++it)
		*it = static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
	
	return (lowercaseString);
}