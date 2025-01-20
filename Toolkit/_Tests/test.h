

#ifndef TEST_H

# define TEST_H

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>


std::string FileLineFunction(const char* file, const int line, const char* function);

#define TEST_FAIL_INFO() \
	std::cerr   << std::setw(10) << "	File: " << __FILE__ << "\n" \
				<< std::setw(10) << "	Line: " << __LINE__ << "\n" \
				<< std::setw(10) << "	Function: " << __FUNCTION__ << std::endl;



#endif