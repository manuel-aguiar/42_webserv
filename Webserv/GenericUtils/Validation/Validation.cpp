#include "../../GenericUtils/Validation/Validation.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

bool	Validation::isNumber(const std::string &str)
{
	return (!str.empty() && (str.find_first_not_of("0123456789") == std::string::npos));
}

bool	Validation::validateIpv4(const std::string &str)
{
	std::vector<std::string> values = StringUtils::split(str, '.');

	if (values.size() != 4)
		return (0);
	for (int i = 0; i < 4; i++)
	{
		if (!Validation::isNumber(values[i]) || StringUtils::stoull(values[i]) > 255)
			return (0);
	}

	return (1);
}
