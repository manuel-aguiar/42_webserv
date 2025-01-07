#include "../../GenericUtils/Validation/Validation.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

bool	isNumber(const std::string &str)
{
	return (!str.empty() && (str.find_first_not_of("0123456789") == std::string::npos));
}

bool	validateIpv4(const std::string &str)
{
	std::vector<std::string> values = split(str, '.');

	if (values.size() != 4)
		return (0);
	for (int i = 0; i < 4; i++)
	{
		if (!isNumber(values[i]) || ::stoull(values[i]) > 255)
			return (0);
	}

	return (1);
}