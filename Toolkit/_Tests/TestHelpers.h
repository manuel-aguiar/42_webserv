

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
}


#endif