

# include "test.h"

std::string FileLineFunction(const char* file, const int line, const char* function)
{
    return std::string("\tFile: ") + file + "\n\tLine: " + to_string(line) + "\n\tFunction: " + function + '\n';
}

namespace TestHelpers
{
    std::string FileLineFunction(const char* file, const int line, const char* function)
    {
        return std::string("\tFile: ") + file + "\n\tLine: " + to_string(line) + "\n\tFunction: " + function + '\n';
    }   
}