
#ifndef TESTHELPERS_H

# define TESTHELPERS_H

#include <string>
#include <sstream>
#include <stdexcept>


namespace TestHelpers
{
	
	template <typename T>
	std::string to_string(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	template <typename T>
	std::string errorMsg(T actual, T expected, const std::string& message, const char* file, int line, const char* function)
	{
		std::string strLine = TestHelpers::to_string(line);
		std::string strActual = TestHelpers::to_string(actual);
		std::string strExpected = TestHelpers::to_string(expected);
		return 
			std::string("\t----------------------------------\n")
			+ "\tError:     '" + message 				+ "'\n"
			+ "\tResult:    '" + strActual 				+ "'\n"
			+ "\tExpected:  '" + strExpected 			+ "'\n"
			+ "\t----------------------------------" 	+ "\n"
			+ "\tFile:       " + file + ":" + strLine 	+ "\n"
			+ "\tLine:       " + strLine 				+ "\n"
			+ "\tFunction:   " + function 				+ "\n"
			+ "\t----------------------------------" 	+ "\n";
	}

	template <typename T>
	void assertEqual(T actual, T expected, const std::string& message, const char* file, int line, const char* function)
	{
		if (actual != expected)
			throw std::logic_error(std::string("\n\n") + TestHelpers::errorMsg(actual, expected, message, file, line, function));
	}

}

	#ifndef EXPECT_EQUAL
		#include <sstream>
		#define EXPECT_EQUAL(actual, expected, message)                                \
		do {                                                                           \
			if ((actual) != (expected)) {                                              \
				std::ostringstream oss;                                                \
				oss << "\n\n"                                                          \
					<< "\tEXPECT_EQUAL failed\n"                                       \
					<< "\t----------------------------------\n"                        \
					<< "\tError:     '" << (message)  << "'\n"                         \
					<< "\tResult:    '" << (actual)   << "' [" << #actual   << "]\n"   \
					<< "\tExpected:  '" << (expected) << "' [" << #expected << "]\n"   \
					<< "\t----------------------------------\n"                        \
					<< "\tFile:       " << __FILE__  << ":" << __LINE__ << "\n"        \
					<< "\tLine:       " << __LINE__  << "\n"                           \
					<< "\tFunction:   " << __FUNCTION__ << "\n"                        \
					<< "\t----------------------------------\n";                        \
				throw std::logic_error(oss.str());                                     \
			}                                                                          \
		} while (0)
	#endif

#endif