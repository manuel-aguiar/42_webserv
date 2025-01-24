

#ifndef TEST_H

# define TEST_H

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string FileLineFunction(const char* file, const int line, const char* function);

#define TEST_FAIL_INFO() \
	std::cerr   << std::setw(10) << "	File: " << __FILE__ << "\n" \
				<< std::setw(10) << "	Line: " << __LINE__ << "\n" \
				<< std::setw(10) << "	Function: " << __FUNCTION__ << std::endl;

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
			std::string strLine = TestHelpers::to_string(line);
			std::string strActual = TestHelpers::to_string(actual);
			std::string strExpected = TestHelpers::to_string(expected);
			throw std::logic_error(std::string("\n\n")
				+ "\t----------------------------------" 	+ "\n"
				+ "\tError:     '" + message 				+ "'\n"
				+ "\tResult:    '" + strActual 				+ "'\n"
				+ "\tExpected:  '" + strExpected 			+ "'\n"
				+ "\t----------------------------------" 	+ "\n"
				+ "\tFile:       " + file + ":" + strLine 	+ "\n"
				+ "\tLine:       " + strLine 				+ "\n"
				+ "\tFunction:   " + function 				+ "\n"
				+ "\t----------------------------------" 	+ "\n"
				);
		}
	}

}


#endif