

# include "TestHelpers.h"

std::string TestHelpers::FileLineFunction(const char* file, const int line, const char* function)
{
    return std::string("\tFile: ") + file + "\n\tLine: " + TestHelpers::to_string(line) + "\n\tFunction: " + function + '\n';
}