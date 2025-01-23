

#ifndef TEST_H

# define TEST_H

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>

namespace TestHelpers
{
	template <typename T>
	std::string to_string(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}


	std::string FileLineFunction(const char* file, const int line, const char* function);

	template <typename T>
	void assertEqual(T actual, T expected, const std::string& message, const char* file, int line, const char* function)
	{
		if (actual != expected)
		{
			throw std::logic_error(
				message + " result was " + StringUtils::to_string(actual) +
				" but expected: " + StringUtils::to_string(expected) + "\n" +
				TestHelpers::FileLineFunction(file, line, function));
		}
	}

	template <typename T>
	void assertUnequal(T actual, T expected, const std::string& message, const char* file, int line, const char* function)
	{
		if (actual == expected)
		{
			throw std::logic_error(
				message + " result was " + StringUtils::to_string(actual) +
				" but expected: " + StringUtils::to_string(expected) + "\n" +
				TestHelpers::FileLineFunction(file, line, function));
		}
	}
}


#endif